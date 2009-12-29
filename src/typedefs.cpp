#include "typedefs.h"

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const NamedItemID &namedItemID) {
    dbg.nospace() << namedItemID.itemNQs[namedItemID.itemID].name.toStdString().c_str() << ":" << namedItemID.itemNQs[namedItemID.itemID].quantity;

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const Item &item) {
    return dbg.space() << item.id << ":" << item.supportCount;
}

QDebug operator<<(QDebug dbg, const NamedItem &namedItem) {
    QString itemOutput;

    NamedItemID tmp;
    tmp.itemID = namedItem.item.id;
    tmp.itemNQs = namedItem.itemNQs;
    QDebug(&itemOutput) << tmp;

    dbg.nospace() << itemOutput.toStdString().c_str() << "=" << namedItem.item.supportCount;

    return dbg.space();
}

QDebug operator<<(QDebug dbg, const Transaction &transaction) {
    dbg.nospace() << "[size=" << transaction.size() << "] {";

    for (int i = 0; i < transaction.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << transaction[i].id;
    }
    dbg.nospace() << "}";

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const NamedTransaction &namedTransaction) {
    Transaction transaction = namedTransaction.transaction;
    QString itemOutput;
    NamedItem tmp;
    tmp.itemNQs = namedTransaction.itemNQs;

//    dbg.nospace() << "[size=" << transaction.size() << "] ";
    dbg.nospace() << "{";

    for (int i = 0; i < transaction.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";

        // Generate output for item.
        tmp.item.supportCount = namedTransaction.transaction[i].supportCount;
        tmp.item.id = namedTransaction.transaction[i].id;
        itemOutput.clear();
        QDebug(&itemOutput) << tmp;

        dbg.nospace() << itemOutput.toStdString().c_str();
    }
    dbg.nospace() << "}";

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const NamedItemIDList &namedItemIDList) {
    QString itemOutput;
    NamedItemID tmp;
    tmp.itemNQs = namedItemIDList.itemNQs;

//    dbg.nospace() << "[size=" << namedItemIDList.itemIDs.size() << "] ";
    dbg.nospace() << "{";

    for (int i = 0; i < namedItemIDList.itemIDs.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";

        // Generate output for item.
        tmp.itemID = namedItemIDList.itemIDs[i];
        itemOutput.clear();
        QDebug(&itemOutput) << tmp;

        dbg.nospace() << itemOutput.toStdString().c_str();
    }
    dbg.nospace() << "}";

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const NamedItemList &namedItemList) {
    QString itemOutput;
    NamedItem tmp;
    tmp.itemNQs = namedItemList.itemNQs;

//    dbg.nospace() << "[size=" << namedItemList.items.size() << "] ";
    dbg.nospace() << "{";

    for (int i = 0; i < namedItemList.items.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";

        // Generate output for item.
        tmp.item.supportCount = namedItemList.items[i].supportCount;
        tmp.item.id = namedItemList.items[i].id;
        itemOutput.clear();
        QDebug(&itemOutput) << tmp;

        dbg.nospace() << itemOutput.toStdString().c_str();
    }
    dbg.nospace() << "}";

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const AssociationRule &associationRule) {
    dbg.nospace() << "{";
    for (int i = 0; i < associationRule.antecedent.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << associationRule.antecedent[i].id;
    }
    dbg.nospace() << "} => {";
    for (int i = 0; i < associationRule.consequent.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << associationRule.consequent[i].id;
    }
    dbg.nospace() << "}";

    dbg.nospace() << " (conf=" << associationRule.confidence << ")";

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const NamedAssociationRule &namedAssociationRule) {
    QString itemListOutput;
    NamedItemList tmp;
    tmp.itemNQs = namedAssociationRule.itemNQs;

    dbg.nospace() << "{";

    // Generate output for the antecedent item list.
    tmp.items = namedAssociationRule.antecedent;
    itemListOutput.clear();
    QDebug(&itemListOutput) << tmp;
    dbg.nospace() << itemListOutput.toStdString().c_str();

    dbg.nospace() << "} => {";

    // Generate output for the consequent item list.
    tmp.items = namedAssociationRule.consequent;
    itemListOutput.clear();
    QDebug(&itemListOutput) << tmp;
    dbg.nospace() << itemListOutput.toStdString().c_str();

    dbg.nospace() << "}";

    dbg.nospace() << " (conf=" << namedAssociationRule.confidence << ")";

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const IntervalAssociationRuleItem &intervalAssociationRuleItem) {
    dbg.nospace() << intervalAssociationRuleItem.itemName << " " << intervalAssociationRuleItem.quantities << " = " << intervalAssociationRuleItem.supportCounts;

    return dbg.nospace();
}

#endif
