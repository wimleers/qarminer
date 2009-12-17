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
    ItemCountHash supportCounts;
    ItemList itemsSortedBySupportCount;
    FPTree* tree;
    int minimumSupport;

    Transaction optimizeTransaction(Transaction transaction) const;
    void calculateItemsSortedBySupportCount();
    ItemList determineSuffixOrder() const;

    QList<ItemList> generateFrequentItemsets(FPTree* tree, ItemList suffix = ItemList());

public:
    FPGrowth(QString filename, int minimumSupport);

protected slots:
    void parsedTransaction(Transaction transaction);
};

#endif // FPGROWTH_H
