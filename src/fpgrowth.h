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
    ItemCountHash totalSupportCounts;
    ItemIDList itemsSortedByTotalSupportCount;
    FPTree* tree;
    SupportCount minimumSupport;
    int numberTransactions;
    ItemNQHash itemNQs;

    Transaction optimizeTransaction(Transaction transaction) const;
    void calculateItemsSortedBySupportCount();
    ItemIDList determineSuffixOrder() const;

    QList<ItemList> generateFrequentItemsets(FPTree* tree, ItemList suffix = ItemList());

public:
    FPGrowth(QString filename, SupportCount minimumSupport);
    ~FPGrowth();
    ItemNQHash getItemNQs() const { return this->itemNQs; }
    void preprocessingPhase1();
    void preprocessingPhase2();
    QList<ItemList> calculatingPhase1();
    QList<SupportCount> calculatingPhase2(QList<ItemList> frequentItemsets);

    // Static (class) methods.
    static QList<SupportCount> calculateSupportCountsForFrequentItemsets(QList<ItemList> frequentItemsets);
    static SupportCount calculateSupportCountForFrequentItemset(ItemList frequentItemset);

protected slots:
    void parsedTransaction(Transaction transaction);
};

#endif // FPGROWTH_H
