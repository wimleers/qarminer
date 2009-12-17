#include "fpgrowth.h"

FPGrowth::FPGrowth(QString filename, int minimumSupport) {
    this->parser.setFile(filename);
    this->minimumSupport = minimumSupport;
    this->tree = new FPTree();

    qDebug() << "Preprocessing stage 1: parsing item names and support counts."
             << "-" << "Time complexity: O(n).";
    QPair< QHash<Item, ItemName>, QHash<Item, ItemCount> > result;
    result = this->parser.parseItemNamesAndSupportCounts();
    this->itemNames = result.first;
    this->supportCounts = result.second;
    this->calculateItemsSortedBySupportCount();
    // Let the tree know the names of the items, so it can print those instead
    // of the corresponding integers when printing the tree.
    this->tree->setItemNames(&this->itemNames);

    qDebug() << "Preprocessing stage 2: parsing transactions"
             << "-" << "Time complexity: O(n).";
    QObject::connect(&(this->parser), SIGNAL(parsedTransaction(Transaction)), this, SLOT(parsedTransaction(Transaction)));
    // The slot that parser.parseTransactions() will call, will fill the FPTree.
    this->parser.parseTransactions();

    qDebug() << *this->tree;

    qDebug() << "Calculating stage"
             << "-" << "Time complexity: [not yet available].";
    ItemList orderedSuffixes = this->determineSuffixOrder();
    qDebug() << "ordered suffixes:" << orderedSuffixes;

    QList<ItemList> frequentItemsets = this->generateFrequentItemsets(this->tree);
    qDebug() << "frequent itemsets:" << frequentItemsets.size();
    NamedItemList nfis;
    foreach (ItemList fis, frequentItemsets) {
        nfis.items = fis;
        nfis.itemNames = this->itemNames;
        qDebug() << nfis;
    }
}


//------------------------------------------------------------------------------
// Protected methods.

// TODO: make this method use this->itemsSortedBySupportCount, which removes a
// lot of per-transaction calculations, at the cost of comparing with a possibly
// long list of items to find the proper order.
Transaction FPGrowth::optimizeTransaction(Transaction transaction) const {
    Transaction optimizedTransaction;
    QHash<ItemCount, Item> itemsBySupport;
    QSet<ItemCount> supportSet;
    QList<ItemCount> supportList;
    ItemCount support;

    // Fill the following variables:
    // - itemsBySupport, which maps supports (key) to all items with that
    //   support (values).
    // - supportSet, which is a set of all different supports. This will be used
    //   to sort by support.
    // But discard infrequent items (i.e. if their support is smaller than the
    // minimum support).
    foreach (Item item, transaction) {
        support = this->supportCounts[item];

        // Discard infrequent items.
        if (support < this->minimumSupport)
            continue;

        // Fill itemsBySupport by using QHash::insertMulti(), which allows for
        // multiple values for the same key.
        itemsBySupport.insertMulti(support, item);

        // Fill supportSet.
        supportSet.insert(support);
    }

    // It's possible that none of the items in this transaction meet or exceed
    // the minimum support.
    if (supportSet.size() == 0)
        return optimizedTransaction;

    // Sort supportSet from greater to smaller. But first convert supportSet to
    // a list supportList, because sets cannot have an order by definition.
    supportList = supportSet.toList();
    qSort(supportList.begin(), supportList.end(), qGreater<ItemCount>());

    // Store items with largest support first in the optimized transaction:
    // - first iterate over all supports in supportList, which are now sorted
    //   from greater to smaller
    // - then retrieve all items that have this support
    // - then sort these items to ensure the same order is applied to all
    //   itemsets with the same support, which minimizes the size of the FP-tree
    //   even further (i.e. maximizes density)
    // - finally, append the item to the transaction optimizedTransaction.
    foreach (ItemCount support, supportList) {
        QList<Item> items = itemsBySupport.values(support);
        qSort(items);
        foreach (Item item, items)
            optimizedTransaction << item;
    }

    return optimizedTransaction;
}

void FPGrowth::calculateItemsSortedBySupportCount() {
    QHash<ItemCount, Item> itemsBySupport;
    QSet<ItemCount> supportSet;
    QList<ItemCount> supportList;
    ItemCount support;

    Q_ASSERT_X(this->itemsSortedBySupportCount.size() == 0, "FPGrowth::getItemsSortedBySupportCount", "Should only be called once.");

    foreach (Item item, this->supportCounts.keys()) {
        support = this->supportCounts[item];

        // Fill itemsBySupport by using QHash::insertMulti(), which allows for
        // multiple values for the same key.
        itemsBySupport.insertMulti(support, item);

        // Fill supportSet.
        supportSet.insert(support);
    }

    // Sort supportSet from smaller to greater. But first convert supportSet to
    // a list supportList, because sets cannot have an order by definition.
    supportList = supportSet.toList();
    qSort(supportList);

    // Store items with largest support first in the optimized transaction:
    // - first iterate over all supports in supportList, which are now sorted
    //   from greater to smaller
    // - then retrieve all items that have this support
    // - then sort these items to ensure the same order is applied to all
    //   itemsets with the same support, which minimizes the size of the FP-tree
    //   even further (i.e. maximizes density)
    // - finally, append the item to the transaction optimizedTransaction.
    foreach (ItemCount support, supportList) {
        QList<Item> items = itemsBySupport.values(support);
        qSort(items);
        this->itemsSortedBySupportCount.append(items);
    }
}

ItemList FPGrowth::determineSuffixOrder() const {
    return this->itemsSortedBySupportCount;
}

QList<ItemList> FPGrowth::generateFrequentItemsets(FPTree* ctree, ItemList suffix) {
    NamedItemList namedSuffix;
    namedSuffix.items = suffix;
    namedSuffix.itemNames = this->itemNames;
    qDebug() << "---------------------------------generateFrequentItemsets()" << namedSuffix;
    QList<ItemList> frequentItemsets;
    QList<ItemList> prefixPaths;

    // First determine the suffix order for the items in this particular tree,
    // based on the list that contains *all* items in this data set, sorted by
    // support count.
    ItemList orderedSuffixItems;
    ItemList itemsInTree = ctree->getItems();
    foreach (Item item, this->itemsSortedBySupportCount)
        if (itemsInTree.contains(item))
            orderedSuffixItems.append(item);

    // Now iterate over each of the ordered suffix items and generate frequent
    // itemsets!
    foreach (Item suffixItem, orderedSuffixItems) {
        NamedItem namedSuffixItem;
        namedSuffixItem.item = suffixItem;
        namedSuffixItem.itemNames = this->itemNames;
        if (suffix.size() == 0)
            qDebug() << "==========ROOT==========";
        qDebug() << "suffix item" << namedSuffixItem << ctree->getItemSupport(suffixItem) << (ctree->getItemSupport(suffixItem) >= this->minimumSupport);
        if (ctree->getItemSupport(suffixItem) >= this->minimumSupport) {
            // The current suffix item, when prepended to the received suffix,
            // is the next frequent itemset. Additionally, it will serve as the
            // next suffix.
            ItemList frequentItemset;
            frequentItemset.append(suffixItem);
            frequentItemset.append(suffix);

            // Add the new frequent itemset to the list of frequent itemsets.
            frequentItemsets.append(frequentItemset);
            NamedItemList namedBlah;
            namedBlah.items = frequentItemset;
            namedBlah.itemNames = this->itemNames;
            qDebug() << "\t\t\t\t new frequent itemset:" << namedBlah;

            // Calculate the prefix paths for the current suffix item.
            prefixPaths = ctree->calculatePrefixPaths(suffixItem);
            qDebug() << "prefix paths:";
            foreach (ItemList blah, prefixPaths) {
                NamedItemList namedBlah;
                namedBlah.items = blah;
                namedBlah.itemNames = this->itemNames;
                qDebug() << "\t" << namedBlah;
            }


            // Calculate the support counts for the prefix paths.
            ItemCountHash prefixPathsSupportCounts = FPTree::calculateSupportCountsForPrefixPaths(prefixPaths);

            // Remove items from the prefix paths based on the support counts of
            // the items *within* the prefix paths. Also remove the suffix item.
            QList<ItemList> filteredPrefixPaths;
            ItemList prefixPath, filteredPrefixPath;
            Item item;
            for (int i = 0; i < prefixPaths.size(); i++) {
                prefixPath = prefixPaths[i];
                for (int j = 0; j < prefixPath.size() - 1; j++) {
                    item = prefixPath[j];
                    if (prefixPathsSupportCounts[item] >= this->minimumSupport)
                        filteredPrefixPath.append(item);
                }
                if (filteredPrefixPath.size() > 0)
                    filteredPrefixPaths.append(filteredPrefixPath);
                filteredPrefixPath.clear();
            }
            qDebug() << "filtered prefix paths: ";
            foreach (ItemList blah, filteredPrefixPaths) {
                NamedItemList namedBlah;
                namedBlah.items = blah;
                namedBlah.itemNames = this->itemNames;
                qDebug() << "\t" << namedBlah;
            }

            // If no prefix paths remain after filtering, we won't be able to
            // generate any further frequent item sets.
            if (filteredPrefixPaths.size() > 0) {
                // Build the conditional FP-tree for these prefix paths, by creating
                // a new FP-tree and pretending the prefix paths are transactions.
                FPTree* cfptree = new FPTree();
                cfptree->setItemNames(&this->itemNames);
                foreach (ItemList prefixPath, filteredPrefixPaths)
                    cfptree->addTransaction(prefixPath);
                //            qDebug() << *cfptree;

                // Attempt to generate more frequent itemsets, with the current
                // frequent itemset as the suffix.
                frequentItemsets.append(this->generateFrequentItemsets(cfptree, frequentItemset));

                delete cfptree;
            }
        }
        else
            qDebug() << "support count of" << suffixItem << "in the initial tree is less than minimum support";
    }
    qDebug() << "------END------------------------generateFrequentItemsets()" << suffix;

    return frequentItemsets;
}

//------------------------------------------------------------------------------
// Protected slots.

/**
  * Slot that receives a Transaction, optimizes it and adds it to the tree.
  */
void FPGrowth::parsedTransaction(Transaction transaction) {
    Transaction optimizedTransaction;

    optimizedTransaction = this->optimizeTransaction(transaction);

    // It's possible that the optimized transaction has become empty if none of
    // the items in the given transaction meet or exceed the minimum support.
    if (optimizedTransaction.size() > 0)
        this->tree->addTransaction(optimizedTransaction);
}
