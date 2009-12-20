#include "typedefs.h"

QDebug operator<<(QDebug dbg, const NamedItemID &namedItemID) {
    return dbg.space() << namedItemID.itemNames[namedItemID.itemID].toStdString().c_str();
}

QDebug operator<<(QDebug dbg, const Item &item) {
    return dbg.space() << item.id << ":" << item.supportCount;
}

QDebug operator<<(QDebug dbg, const NamedItem &namedItem) {
    return dbg.space() << namedItem.itemNames[namedItem.item.id].toStdString().c_str() << ":" << namedItem.item.supportCount;
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
    ItemNameHash itemNames = namedTransaction.itemNames;

    dbg.nospace() << "[size=" << transaction.size() << "] {";

    for (int i = 0; i < transaction.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << itemNames[transaction[i].id].toStdString().c_str();
    }
    dbg.nospace() << "}";

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const NamedItemIDList &namedItemIDList) {
    ItemIDList itemIDList = namedItemIDList.itemIDs;
    ItemNameHash itemNames = namedItemIDList.itemNames;

    dbg.nospace() << "[size=" << itemIDList.size() << "] {";

    for (int i = 0; i < itemIDList.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << itemNames[itemIDList[i]].toStdString().c_str();
    }
    dbg.nospace() << "}";

    return dbg.nospace();
}

QDebug operator<<(QDebug dbg, const NamedItemList &namedItemList) {
    ItemList itemList = namedItemList.items;
    ItemNameHash itemNames = namedItemList.itemNames;

    dbg.nospace() << "[size=" << itemList.size() << "] {";

    for (int i = 0; i < itemList.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << itemNames[itemList[i].id].toStdString().c_str() << ":" << itemList[i].supportCount;
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
    dbg.nospace() << "{";
    for (int i = 0; i < namedAssociationRule.antecedent.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << namedAssociationRule.itemNames[namedAssociationRule.antecedent[i].id];
    }
    dbg.nospace() << "} => {";
    for (int i = 0; i < namedAssociationRule.consequent.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << namedAssociationRule.itemNames[namedAssociationRule.consequent[i].id];;
    }
    dbg.nospace() << "}";

    dbg.nospace() << " (conf=" << namedAssociationRule.confidence << ")";

    return dbg.nospace();
}
