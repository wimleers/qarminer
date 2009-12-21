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
    ItemNQHash itemNQs;
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
    QList<AssociationRule> generateAssociationRules(QList<ItemList> frequentItemsets, QList<SupportCount> frequentItemsetsSupportCounts);
    QList<AssociationRule> generateAssociationRulesForFrequentItemset(ItemList frequentItemset, QList<ItemList> consequents, QList<ItemList> frequentItemsets, QList<SupportCount> frequentItemsetsSupportCounts);

    // Static (class) methods.
    static QList<SupportCount> calculateSupportCountsForFrequentItemsets(QList<ItemList> frequentItemsets);
    static SupportCount calculateSupportCountForFrequentItemset(ItemList frequentItemset);
    static ItemList getAntecedent(ItemList frequentItemset, ItemList consequent);
    static QList<ItemList> generateCandidateItemsets(QList<ItemList> frequentItemsubsets);

public:
    FPGrowth(QString filename, SupportCount minimumSupport, float minimumConfidence);

protected slots:
    void parsedTransaction(Transaction transaction);
};

#endif // FPGROWTH_H
