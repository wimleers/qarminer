#include "ruleminer.h"

/**
 * An exact implementation of algorithm 6.2 on page 351 in the textbook.
 */
QList<AssociationRule> RuleMiner::generateAssociationRules(QList<ItemList> frequentItemsets, QList<SupportCount> frequentItemsetsSupportCounts, float minimumConfidence) {
    QList<AssociationRule> associationRules;
    QList<ItemList> consequents;

    // Iterate over all frequent itemsets.
    foreach (ItemList frequentItemset, frequentItemsets) {
        // It's only possible to generate an association rule if there are at
        // least two items in the frequent itemset.
        if (frequentItemset.size() >= 2) {
            // Generate all 1-item consequents.
            consequents.clear();
            foreach (Item item, frequentItemset) {
                ItemList consequent;
                consequent.append(item);
                consequents.append(consequent);
            }

            // Debug output.
            qDebug() << "Generating rules for frequent itemset" << frequentItemset << " and consequents " << consequents;

            // Attempt to generate association rules for this frequent itemset
            // and store the results.
            associationRules.append(RuleMiner::generateAssociationRulesForFrequentItemset(frequentItemset, consequents, frequentItemsets, frequentItemsetsSupportCounts, minimumConfidence));
        }
    }
    return associationRules;
}


//------------------------------------------------------------------------------
// Protected static methods.

/**
 * A.k.a. "ap-genrules", but slightly different to fix a bug in that algorithm:
 * it accepts consequents of size 1, but doesn't generate antecedents for these,
 * instead it immediately generates consequents of size 2. Algorithm 6.3 on page
 * 352 in the textbook.
 * This variation of that algorithm fixes that.
 *
 * @param frequentItemsetsSupportCounts may contain an empty list, then the
 *        support counts will be calculated on-the-fly.
 */
QList<AssociationRule> RuleMiner::generateAssociationRulesForFrequentItemset(ItemList frequentItemset, QList<ItemList> consequents, QList<ItemList> frequentItemsets, QList<SupportCount> frequentItemsetsSupportCounts, float minimumConfidence) {
    QList<AssociationRule> associationRules;
    SupportCount frequentItemsetSupportCount, antecedentSupportCount;
    ItemList antecedent;
    float confidence;
    unsigned int k = frequentItemset.size(); // Size of the frequent itemset.
    unsigned int m = consequents[0].size(); // Size of a single consequent.

    // Iterate over all given consequents.
    foreach (ItemList consequent, consequents) {
        // Get the antecedent for the current consequent, so we effectively get
        // a candidate rule.
        antecedent = RuleMiner::getAntecedent(frequentItemset, consequent);

        // Calculate the confidence of this rule.
        frequentItemsetSupportCount = FPGrowth::calculateSupportCountForFrequentItemset(frequentItemset);
        // Take advantage of precalculated frequent itemset support counts when
        // they are available, and calculate the support count for the
        // antecedent on-the-fly otherwise.
        antecedentSupportCount = (frequentItemsetsSupportCounts.size() > 0) ? frequentItemsetsSupportCounts[frequentItemsets.indexOf(antecedent)] : FPGrowth::calculateSupportCountForFrequentItemset(antecedent);
        confidence = 1.0 * frequentItemsetSupportCount / antecedentSupportCount;

        // If the confidence is sufficiently high, we've found an association
        // rule that meets our requirements.
        qDebug () << "confidence" << confidence << ", frequent itemset support" << frequentItemsetSupportCount << ", antecedent support" << antecedentSupportCount << ", antecedent" << antecedent << ", consequent" << consequent;
        if (confidence > minimumConfidence) {
            AssociationRule rule = {antecedent, consequent, confidence};
            associationRules.append(rule);
        }
        // If the confidence is not sufficiently high, delete this consequent,
        // to prevent other consequents to be generated that build upon this one
        // since those consequents would have the same insufficiently high
        // confidence in the best case.
        else
            consequents.removeAll(consequent);
    }

    // If there are still consequents left (i.e. consequents with a sufficiently
    // high confidence), and the size of the consequents still alows them to
    // be expanded, expand the consequents and attempt to generate more
    // association rules with them.
    if (consequents.size() > 0 && k > m + 1) {
        QList<ItemList> candidateConsequents = RuleMiner::generateCandidateItemsets(consequents);
        associationRules.append(RuleMiner::generateAssociationRulesForFrequentItemset(frequentItemset, candidateConsequents, frequentItemsets, frequentItemsetsSupportCounts, minimumConfidence));
    }

    return associationRules;
}


/**
 * Build the antecedent for this candidate consequent, which are all items in
 * the frequent itemset except for those in the candidate consequent.
 */
ItemList RuleMiner::getAntecedent(ItemList frequentItemset, ItemList consequent) {
    ItemList antecedent;
    foreach (Item item, frequentItemset)
        if (!consequent.contains(item))
            antecedent.append(item);
    return antecedent;
}

/**
 * A.k.a. "apriori-gen", but without pruning because we're already working with
 * frequent itemsets, which were generated by the FP-growth algorithm. We solely
 * need this function for association rule mining, not for frequent itemset
 * generation.
 */
QList<ItemList> RuleMiner::generateCandidateItemsets(QList<ItemList> frequentItemsubsets) {
    // Phase 1: candidate generation.
    QList<ItemList> candidateItemsets;
    int allButOne = frequentItemsubsets[0].size() - 1;
    foreach (ItemList frequentItemsubset, frequentItemsubsets) {
        foreach (ItemList otherFrequentItemsubset, frequentItemsubsets) {
            if (allButOne == 0) {
                if (frequentItemsubset[0] == otherFrequentItemsubset[0]) {
                    break;
                }
            }
            else {
                // Iterate over all but the last item in this frequent itemsubset.
                for (int i = 0; i < allButOne; i++) {
                    if (frequentItemsubset[i] != otherFrequentItemsubset[i])
                        break;
                }
            }

            // The first all-but-one items of the two frequent itemsubsets
            // matched! Now generate a candidate itemset, whose:
            // - first k-1 == allButOne items are copied from the first
            //   frequent itemsubset (outer)
            // - last (k == allButOne + 1) item is copied from the second
            //   frequent itemsubset (inner)
            ItemList candidateItemset;
            candidateItemset.append(frequentItemsubset);
            candidateItemset.append(otherFrequentItemsubset[allButOne]);
            // Store this candidate set.
            candidateItemsets.append(candidateItemset);
        }
    }

    // Phase 2: candidate pruning.
    // Not necessary!

    return candidateItemsets;
}

