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
    SupportCount minimumSupport;
    float minimumConfidence;
    int numberTransactions;

    Transaction optimizeTransaction(Transaction transaction) const;
    void calculateItemsSortedBySupportCount();
    ItemIDList determineSuffixOrder() const;

    QList<ItemList> generateFrequentItemsets(FPTree* tree, ItemList suffix = ItemList());
    QList<SupportCount> calculateSupportForFrequentItemsets(QList<ItemList> frequentItemsets);

public:
    FPGrowth(QString filename, SupportCount minimumSupport, float minimumConfidence);

protected slots:
    void parsedTransaction(Transaction transaction);
};

#endif // FPGROWTH_H
