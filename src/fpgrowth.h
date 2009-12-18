#ifndef FPGROWTH_H
#define FPGROWTH_H

#include <QObject>
#include <QHash>
#include <QSet>
#include <QString>
#include "arffparser.h"
#include "fpnode.h"
#include "fptree.h"

class FPGrowth : public QObject {
    Q_OBJECT
protected:
    ARFFParser parser;
    ItemNameHash itemNames;
    ItemCountHash totalSupportCounts;
    ItemIDList itemsSortedByTotalSupportCount;
    FPTree* tree;
    ItemCount minimumSupport;
    int numberTransactions;

    Transaction optimizeTransaction(Transaction transaction) const;
    void calculateItemsSortedBySupportCount();
    ItemIDList determineSuffixOrder() const;

    QList<ItemList> generateFrequentItemsets(FPTree* tree, ItemList suffix = ItemList());

public:
    FPGrowth(QString filename, ItemCount minimumSupport);

protected slots:
    void parsedTransaction(Transaction transaction);
};

#endif // FPGROWTH_H
