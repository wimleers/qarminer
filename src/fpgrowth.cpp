#include "fpgrowth.h"

FPGrowth::FPGrowth(QString filename, float minimumSupport) {
    this->parser.setFile(filename);
    this->minimumSupport = minimumSupport;
    this->minimumSupportAbsolute = 0;
    this->numberTransactions = 0;
    this->tree = new FPTree();
}

FPGrowth::~FPGrowth() {
    delete this->tree;
}

void FPGrowth::preprocessingPhase1() {
    QPair<QPair<ItemNQHash, ItemCountHash>, unsigned int> result;
    result = this->parser.parseItemProperties();
    this->itemNQs = result.first.first;
    this->totalSupportCounts = result.first.second;
    this->numberTransactions = result.second;
    this->minimumSupportAbsolute = ceil(this->minimumSupport * this->numberTransactions);
    qDebug() << "min sup abs" << this->minimumSupportAbsolute << this->minimumSupport << this->numberTransactions;
    this->calculateItemsSortedBySupportCount();

#ifdef DEBUG
    // Let the tree know the names and quantities of the items, so it can print
    // those instead of the corresponding integers when printing the tree.
    this->tree->setItemNQs(&this->itemNQs);
#endif
}

void FPGrowth::preprocessingPhase2() {
    QObject::connect(&(this->parser), SIGNAL(parsedTransaction(Transaction)), this, SLOT(parsedTransaction(Transaction)));
    // The slot that parser.parseTransactions() will call, will fill the FPTree.
    this->parser.parseTransactions();
    QObject::disconnect(&(this->parser), SIGNAL(parsedTransaction(Transaction)), this, SLOT(parsedTransaction(Transaction)));

#ifdef DEBUG
    qDebug() << "Parsed" << this->numberTransactions << "transactions.";
    qDebug() << *this->tree;
#endif
}


QList<ItemList> FPGrowth::calculatingPhase1() {
    QList<ItemList> frequentItemsets = this->generateFrequentItemsets(this->tree);

#ifdef DEBUG
    // Debug output.
    qDebug() << "frequent itemsets:" << frequentItemsets.size();
    NamedItemList nfis;
    foreach (ItemList fis, frequentItemsets) {
        nfis.items = fis;
        nfis.itemNQs = this->itemNQs;
        qDebug() << nfis;
    }
#endif

    return frequentItemsets;
}


QList<SupportCount> FPGrowth::calculatingPhase2(QList<ItemList> frequentItemsets) {
    QList<SupportCount> frequentItemsetsSupportCounts = this->calculateSupportCountsForFrequentItemsets(frequentItemsets);

#ifdef DEBUG
    // Debug output.
    NamedItemList nfis;
    foreach (ItemList frequentItemset, frequentItemsets) {
        nfis.items = frequentItemset;
        nfis.itemNQs = this->itemNQs;
        qDebug() << "support(" << nfis << ") =" << (frequentItemsetsSupportCounts[frequentItemsets.indexOf(frequentItemset)] * 1.0 / this->numberTransactions);
    }
#endif DEBUG

    return frequentItemsetsSupportCounts;
}


//------------------------------------------------------------------------------
// Public static methods.

/**
 * Runs FPGrowth::calculateSupportForFrequentItemset() on a list of frequent
 * itemsets.
 *
 * @see FPGrowth::calculateSupportForFrequentItemset()
 */
QList<SupportCount> FPGrowth::calculateSupportCountsForFrequentItemsets(QList<ItemList> frequentItemsets) {
    QList<SupportCount> supportCounts;
    foreach (ItemList frequentItemset, frequentItemsets)
        supportCounts.append(FPGrowth::calculateSupportCountForFrequentItemset(frequentItemset));
    return supportCounts;
}

/**
 * Given a frequent itemsets, calculate its support count.
 * Do this by finding the minimum support count of all items in the frequent
 * itemset.
 */
SupportCount FPGrowth::calculateSupportCountForFrequentItemset(ItemList frequentItemset) {
    SupportCount supportCount = 65535;
    foreach (Item item, frequentItemset)
        if (item.supportCount < supportCount)
            supportCount = item.supportCount;
    return supportCount;
}


//------------------------------------------------------------------------------
// Protected methods.

// TODO: make this method use this->itemsSortedBySupportCount, which removes a
// lot of per-transaction calculations, at the cost of comparing with a possibly
// long list of items to find the proper order.
Transaction FPGrowth::optimizeTransaction(Transaction transaction) const {
    Transaction optimizedTransaction;
    QHash<ItemID, Item> itemIDToItem;
    QHash<SupportCount, ItemID> totalSupportToItemID;
    QSet<SupportCount> supportSet;
    QList<SupportCount> supportList;
    SupportCount totalSupportCount;

    // Fill the following variables:
    // - itemIDToItem, which maps itemIDs (key) to their corresponding items
    //   (values).
    // - supportToItemID, which maps supports (key) to all item IDs with that
    //   support (values).
    // - supportSet, which is a set of all different supports. This will be used
    //   to sort by support.
    // But discard infrequent items (i.e. if their support is smaller than the
    // minimum support).
    foreach (Item item, transaction) {
        itemIDToItem.insert(item.id, item);
        totalSupportCount = this->totalSupportCounts[item.id];

        // Discard items that are total infrequent.
        if (totalSupportCount < this->minimumSupportAbsolute)
            continue;

        // Fill itemsBySupport by using QHash::insertMulti(), which allows for
        // multiple values for the same key.
        totalSupportToItemID.insertMulti(totalSupportCount, item.id);

        // Fill supportSet.
        supportSet.insert(totalSupportCount);
    }

    // It's possible that none of the items in this transaction meet or exceed
    // the minimum support.
    if (supportSet.size() == 0)
        return optimizedTransaction;

    // Sort supportSet from greater to smaller. But first convert supportSet to
    // a list supportList, because sets cannot have an order by definition.
    supportList = supportSet.toList();
    qSort(supportList.begin(), supportList.end(), qGreater<SupportCount>());

    // Store items with largest support first in the optimized transaction:
    // - first iterate over all supports in supportList, which are now sorted
    //   from greater to smaller
    // - then retrieve all items that have this support
    // - then sort these items to ensure the same order is applied to all
    //   itemsets with the same support, which minimizes the size of the FP-tree
    //   even further (i.e. maximizes density)
    // - finally, append the item to the transaction optimizedTransaction.
    foreach (SupportCount support, supportList) {
        ItemIDList itemIDs = totalSupportToItemID.values(support);
        qSort(itemIDs);
        foreach (ItemID itemID, itemIDs)
            optimizedTransaction << itemIDToItem[itemID];
    }

    return optimizedTransaction;
}

void FPGrowth::calculateItemsSortedBySupportCount() {
    QHash<SupportCount, ItemID> itemsByTotalSupport;
    QSet<SupportCount> supportSet;
    QList<SupportCount> supportList;
    SupportCount totalSupport;

    Q_ASSERT_X(this->itemsSortedByTotalSupportCount.size() == 0, "FPGrowth::getItemsSortedBySupportCount", "Should only be called once.");

    foreach (ItemID itemID, this->totalSupportCounts.keys()) {
        totalSupport = this->totalSupportCounts[itemID];

        // Fill itemsBySupport by using QHash::insertMulti(), which allows for
        // multiple values for the same key.
        itemsByTotalSupport.insertMulti(totalSupport, itemID);

        // Fill supportSet.
        supportSet.insert(totalSupport);
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
    foreach (SupportCount support, supportList) {
        ItemIDList itemIDs = itemsByTotalSupport.values(support);
        qSort(itemIDs);
        this->itemsSortedByTotalSupportCount.append(itemIDs);
    }
}

QList<ItemList> FPGrowth::generateFrequentItemsets(FPTree* ctree, ItemList suffix) {
#ifdef DEBUG
    NamedItemList namedSuffix;
    namedSuffix.items = suffix;
    namedSuffix.itemNQs = this->itemNQs;
    qDebug() << "---------------------------------generateFrequentItemsets()" << namedSuffix;
#endif

    QList<ItemList> frequentItemsets;
    QList<ItemList> prefixPaths;

    // Build a variation of the suffix, storing a mapping of item IDs to their
    // corresponding items. We'll need this in some of the calculations.
    QHash<ItemID, Item> suffixItemIDToItem;
    foreach (Item item, suffix)
        suffixItemIDToItem.insert(item.id, item);

    // First determine the suffix order for the items in this particular tree,
    // based on the list that contains *all* items in this data set, sorted by
    // support count.
    ItemIDList orderedSuffixItemIDs;
    ItemIDList itemIDsInTree = ctree->getItemIDs();
    foreach (ItemID itemID, this->itemsSortedByTotalSupportCount)
        if (itemIDsInTree.contains(itemID))
            orderedSuffixItemIDs.append(itemID);

    // Now iterate over each of the ordered suffix items and generate frequent
    // itemsets!
    foreach (ItemID suffixItemID, orderedSuffixItemIDs) {
#ifdef DEBUG
        // Debug output.
        NamedItemID namedSuffixItemID;
        namedSuffixItemID.itemID = suffixItemID;
        namedSuffixItemID.itemNQs = this->itemNQs;
        if (suffix.size() == 0)
            qDebug() << "==========ROOT==========";
        qDebug() << "suffix item" << namedSuffixItemID << ctree->getItemSupport(suffixItemID) << (ctree->getItemSupport(suffixItemID) >= this->minimumSupport);
#endif

        // Only if this suffix item's support meets or exceeds the minim
        // support, it will be added as a frequent itemset (appended with the
        // received suffix of course).
        SupportCount suffixItemSupport = ctree->getItemSupport(suffixItemID);
        if (suffixItemSupport >= this->minimumSupportAbsolute) {
            // The current suffix item, when prepended to the received suffix,
            // is the next frequent itemset. Additionally, it will serve as the
            // next suffix.
            Item suffixItem = { suffixItemID, suffixItemSupport };
            ItemList frequentItemset;
            frequentItemset.append(suffixItem);
            frequentItemset.append(suffix);

#ifdef DEBUG
            // Debug output.
            NamedItemList namedFrequentItemSet;
            namedFrequentItemSet.items = frequentItemset;
            namedFrequentItemSet.itemNQs = this->itemNQs;
            qDebug() << "\t\t\t\t new frequent itemset:" << namedFrequentItemSet;
#endif

            // Add the new frequent itemset to the list of frequent itemsets.
            frequentItemsets.append(frequentItemset);

            // Calculate the prefix paths for the current suffix item.
            prefixPaths = ctree->calculatePrefixPaths(suffixItemID);

#ifdef DEBUG
            // Debug output.
            qDebug() << "prefix paths:";
            foreach (ItemList prefixPath, prefixPaths) {
                NamedItemList namedPrefixPath;
                namedPrefixPath.items = prefixPath;
                namedPrefixPath.itemNQs = this->itemNQs;
                qDebug() << "\t" << namedPrefixPath;
            }
#endif

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
                    if (prefixPathsSupportCounts[item.id] >= this->minimumSupportAbsolute)
                        filteredPrefixPath.append(item);
                }
                if (filteredPrefixPath.size() > 0)
                    filteredPrefixPaths.append(filteredPrefixPath);
                filteredPrefixPath.clear();
            }

#ifdef DEBUG
            // Debug output.
            qDebug() << "filtered prefix paths: ";
            foreach (ItemList prefixPath, filteredPrefixPaths) {
                NamedItemList namedPrefixPath;
                namedPrefixPath.items = prefixPath;
                namedPrefixPath.itemNQs = this->itemNQs;
                qDebug() << "\t" << namedPrefixPath;
            }
#endif

            // If no prefix paths remain after filtering, we won't be able to
            // generate any further frequent item sets.
            if (filteredPrefixPaths.size() > 0) {
                // Build the conditional FP-tree for these prefix paths, by creating
                // a new FP-tree and pretending the prefix paths are transactions.
                FPTree* cfptree = new FPTree();
#ifdef DEBUG
                cfptree->setItemNQs(&(this->itemNQs));
#endif
                foreach (ItemList prefixPath, filteredPrefixPaths)
                    cfptree->addTransaction(prefixPath);
#ifdef DEBUG
                qDebug() << *cfptree;
#endif

                // Attempt to generate more frequent itemsets, with the current
                // frequent itemset as the suffix.
                frequentItemsets.append(this->generateFrequentItemsets(cfptree, frequentItemset));

                delete cfptree;
            }
        }
#ifdef DEBUG
        else
            qDebug() << "support count of" << suffixItemID << "in the initial tree is less than minimum support";
#endif
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
    // Keep track of the number of transactions.
    this->numberTransactions++;

    Transaction optimizedTransaction;
    optimizedTransaction = this->optimizeTransaction(transaction);

    // It's possible that the optimized transaction has become empty if none of
    // the items in the given transaction meet or exceed the minimum support.
    if (optimizedTransaction.size() > 0)
        this->tree->addTransaction(optimizedTransaction);
}
