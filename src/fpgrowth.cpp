#include "fpgrowth.h"

FPGrowth::FPGrowth(QString filename, int minimumSupport) {
    this->parser.setFile(filename);
    this->minimumSupport = minimumSupport;
    this->tree = new FPTree();

    qDebug() << "Preprocessing stage 1: parsing item names and support counts";
    QPair< QHash<Item, ItemName>, QHash<Item, ItemCount> > result;
    result = this->parser.parseItemNamesAndSupportCounts();
    this->itemNames = result.first;
    this->supportCounts = result.second;
    // Let the tree know the names of the items, so it can print those instead
    // of the corresponding integers when printing the tree.
    this->tree->setItemNames(&this->itemNames);

    qDebug() << "Preprocessing stage 2: parsing transactions";
    QObject::connect(&(this->parser), SIGNAL(parsedTransaction(Transaction)), this, SLOT(parsedTransaction(Transaction)));
    // The slot that parser.parseTransactions() will call, will fill the FPTree.
    this->parser.parseTransactions();

    qDebug() << *this->tree;

//    qDebug() << "Calculating stage 1: not yet implemented";
}


//------------------------------------------------------------------------------
// Private slots.

/**
  * Slot that receives a Transaction, optimizes it and adds it to the tree.
  */
void FPGrowth::parsedTransaction(Transaction transaction) {
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
        return;

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

    // Add the optimized transaction to the FP-tree.
    this->tree->addTransaction(optimizedTransaction);
}
