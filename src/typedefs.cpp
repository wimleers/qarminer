#include "typedefs.h"


QDebug operator<<(QDebug dbg, const NamedItemID &namedItemID) {
    return dbg.space() << namedItemID.itemNames[namedItemID.itemID].toStdString().c_str();
}

QDebug operator<<(QDebug dbg, const NamedItem &namedItem) {
    return dbg.space() << namedItem.itemNames[namedItem.item.id].toStdString().c_str() << ":" << namedItem.item.count;
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
        dbg.nospace() << itemNames[itemList[i].id].toStdString().c_str() << ":" << itemList[i].count;
    }
    dbg.nospace() << "}";

    return dbg.nospace();
}
