#include "intervaltransformer.h"

/**
 * Transform the given frequent itemsets, which contain item-IDs-per-combination
 * -of-name-and-quantity, to frequent itemsets that are suitable for interval-
 * based quantitative association rules.
 * Steps:
 *   1) filter away frequent itemsets of size 1 (they can't result in any rules)
 *   2) find all unique item name combinations (per frequent itemset) and keep
 *      pointers to all frequent itemsets that have this combination
 *   3) generate each possible combination of each set of frequent itemsets with
 *      the same item names
 *   4) merge all frequent itemset combinations (remember all quantities and
 *      supportCounts so we can validate this in the next step)
 *   5) filter merged frequent itemset combinations according to coalescing
 *      intervals.
 *   6) generate an ItemID for each item in one of the interval (coalescent)
 *      frequent itemsets and store the mapping to item name and quantities
 */
QPair<QHash<ItemID, ItemName>, QHash<ItemID, QPair<Quantity, Quantity> > > IntervalTransformer::transform(QList<ItemList>* frequentItemsets, ItemNQHash itemNamesAndQuantities, float minimumConfidence) {
    QList<IntervalAssociationRule> intervalAssociationRules;
    QList<ItemList> originalFrequentItemsets = *frequentItemsets;

    // Filter away frequent itemsets of size 1.
    foreach (ItemList frequentItemset, originalFrequentItemsets)
        if (frequentItemset.size() == 1)
            originalFrequentItemsets.removeAll(frequentItemset);


    // Find all frequent itemsets with the same set of item names.
    QList<QList<ItemName> > uniqueItemNameCombinations;
    QList<QList<ItemList*> > correspondingFrequentItemsets;
    QList<ItemName> itemNames;
    for (int i = 0; i < originalFrequentItemsets.size(); i++) {
        itemNames.clear();
        // Collect all item names for this frequent itemset.
        foreach (Item item, originalFrequentItemsets[i])
            itemNames.append(itemNamesAndQuantities[item.id].name);

        // If this set of item names does not yet exist, save it.
        if (!uniqueItemNameCombinations.contains(itemNames))
            uniqueItemNameCombinations.append(itemNames);

        // Store the corresponnding frequent itemset in a hash with multiple
        // values.
        int index = uniqueItemNameCombinations.indexOf(itemNames);
        QList<ItemList*> extended = correspondingFrequentItemsets.value(index);
        correspondingFrequentItemsets.removeAt(index);
        extended.append(&originalFrequentItemsets[i]);
        correspondingFrequentItemsets.insert(index, extended);
        qDebug() << itemNames << index << originalFrequentItemsets[i] << "count: " << correspondingFrequentItemsets[index].size();
    }
    qDebug() << uniqueItemNameCombinations << correspondingFrequentItemsets;

    // Generate each possible combination of each set of frequent itemsets with
    // the same item names.
    QList<QList<QList<ItemList*> > > correspondingCombinations;
    QList<QList<ItemList*> > combinationsForGrouping;
    QList<ItemList*> combination;
    for (int i = 0; i < uniqueItemNameCombinations.size(); i++) {
        combinationsForGrouping.clear();
        // Begin from every item name.
        for (int j = 0; j < correspondingFrequentItemsets[i].size(); j++) {
            // 2, then 3, ... then n frequent itemsets (a single frequent itemset
            // would not result in a quantitative association rule).
            for (int k = j + 1; k < correspondingFrequentItemsets[i].size(); k++) {
                combination.clear();
                // Store these 2, 3, ..., n frequent itemsets.
                for (int m = j; m <= k; m++)
                    combination.append(correspondingFrequentItemsets[i][m]);
                combinationsForGrouping.append(combination);
            }
        }
        correspondingCombinations.append(combinationsForGrouping);
    }
#ifdef DEBUG
    for (int i = 0; i < uniqueItemNameCombinations.size(); i++) {
        qDebug() << uniqueItemNameCombinations[i];
        qDebug() << "\tcombos:" << correspondingCombinations[i];
        foreach (QList<ItemList*> fisList, correspondingCombinations[i]) {
            qDebug() << "\t\tcombo:";
            foreach (ItemList* fis, fisList) {
                qDebug() << "\t\t\t" << *fis;
            }
        }
    }
#endif

    // Merge all frequent itemset combinations (remember all quantities and
    // supportCounts so we can validate this in the next step).
    QList<QList<IntervalAssociationRuleItem> > mergedFrequentItemsets;
    QList<IntervalAssociationRuleItem> mergedFrequentItemset;
    // Iterate over the list of combinations of frequent itemsets.
    for (int i = 0; i < correspondingCombinations.size(); i++) {
        qDebug() << "-------all combos for " << uniqueItemNameCombinations[i];
        // Iterate over each combination of frequent itemsets.
        for (int j = 0; j < correspondingCombinations[i].size(); j++) {
            qDebug() << "---one combo";
            mergedFrequentItemset.clear();
            // Iterate over each frequent itemset in this combination.
            for (int k = 0; k < correspondingCombinations[i][j].size(); k++) {
                qDebug() << "items in this combo:";
                // Iterate over each item in this frequent itemset.
                for (int m = 0; m < correspondingCombinations[i][j][k]->size(); m++) {
                    IntervalAssociationRuleItem item = mergedFrequentItemset.value(m);
                    mergedFrequentItemset.removeAt(m);
                    ItemID itemID = (*correspondingCombinations[i][j][k])[m].id;
                    item.itemName = uniqueItemNameCombinations[i][m];
                    item.supportCounts.append((*correspondingCombinations[i][j][k])[m].supportCount);
                    item.quantities.append(itemNamesAndQuantities[itemID].quantity);
                    qDebug() << item.itemName << item.quantities << item.supportCounts;
                    mergedFrequentItemset.insert(m, item);
                }
            }
            mergedFrequentItemsets.append(mergedFrequentItemset);
        }
    }
    qDebug() << "merged: " << mergedFrequentItemsets;

    // Filter merged frequent itemset combinations according to coalescing
    // intervals.
    QList<QList<IntervalAssociationRuleItem> > contiguousFrequentItemsets;
    QList<IntervalAssociationRuleItem> contiguousFrequentItemset;
    bool contiguous;
    for (int i = 0; i < mergedFrequentItemsets.size(); i++) {
        qDebug() << "mfis:" << mergedFrequentItemsets[i];
        contiguous = true;
        contiguousFrequentItemset = mergedFrequentItemsets[i];
        // For each item ...
        for (int j = 0; contiguous && j < mergedFrequentItemsets[i].size(); j++) {
            // ... sort the quantities, and ...
            qSort(contiguousFrequentItemset[j].quantities);
            // ... remove the non-contiguous quantities.
            for (int k = 1; k < contiguousFrequentItemset[j].quantities.size(); k++) {
                if (contiguousFrequentItemset[j].quantities[k] - contiguousFrequentItemset[j].quantities[k-1] != 1) {
                    contiguous = false;
                    break;
                }
            }
        }

        if (contiguous)
            contiguousFrequentItemsets.append(contiguousFrequentItemset);
    }
    qDebug() << "contiguous: " << contiguousFrequentItemsets;


    // Generate an ItemID for each item in one of the interval (coalescent)
    // frequent itemsets  and store the mapping to item name and quantities.
    ItemID nextItemID = 0;
    QHash<ItemID, ItemName> idNameMapping;
    QHash<ItemID, QPair<Quantity, Quantity> > idQuantitiesMapping;
    QList<ItemList> transformedFrequentItemsets;
    ItemList transformedFrequentItemset;
    foreach (QList<IntervalAssociationRuleItem> contiguousFrequentItemset, contiguousFrequentItemsets) {
        foreach (IntervalAssociationRuleItem intervalItem, contiguousFrequentItemset) {
            // Store the id-name mapping.
            idNameMapping.insert(nextItemID, intervalItem.itemName);
            // Store the id-quantities mapping.
            idQuantitiesMapping.insert(nextItemID, qMakePair(intervalItem.quantities.first(), intervalItem.quantities.last()));

            // Generate the transformed frequent itemset, i.e. with a single
            // item ID to identify the item (the itemID which we just generated)
            // and the support count.
            Item item;
            item.id = nextItemID;
            SupportCount totalSupportCount = 0;
            foreach (SupportCount supportCount, intervalItem.supportCounts)
                totalSupportCount += supportCount;
            item.supportCount = totalSupportCount;
            transformedFrequentItemset.append(item);
            nextItemID++;
        }
        transformedFrequentItemsets.append(transformedFrequentItemset);
        transformedFrequentItemset.clear();
    }

    *frequentItemsets = transformedFrequentItemsets;
    return qMakePair(idNameMapping, idQuantitiesMapping);
}
