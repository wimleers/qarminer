#ifndef RULEMINER_H
#define RULEMINER_H

#include "typedefs.h"
#include "fpgrowth.h"
#include <QList>

class RuleMiner {
protected:
    static QList<AssociationRule> generateAssociationRulesForFrequentItemset(ItemList frequentItemset, QList<ItemList> consequents, QList<ItemList> frequentItemsets, QList<SupportCount> frequentItemsetsSupportCounts, float minimumConfidence);
    static ItemList getAntecedent(ItemList frequentItemset, ItemList consequent);
    static QList<ItemList> generateCandidateItemsets(QList<ItemList> frequentItemsubsets);
public:
    static QList<AssociationRule> generateAssociationRules(QList<ItemList> frequentItemsets, QList<SupportCount> frequentItemsetsSupportCounts, float minimumConfidence);
};

#endif // RULEMINER_H
