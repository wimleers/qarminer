#include "typedefs.h"


QDebug operator<<(QDebug dbg, const NamedItem &namedItem) {
    return dbg.space() << namedItem.itemNames[namedItem.item].toStdString().c_str();
}

QDebug operator<<(QDebug dbg, const Transaction &transaction) {
    dbg.nospace() << "[size=" << transaction.size() << "] {";

    for (int i = 0; i < transaction.size(); i++) {
        if (i > 0)
            dbg.nospace() << ", ";
        dbg.nospace() << transaction[i];
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
        dbg.nospace() << itemNames[transaction[i]].toStdString().c_str();
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
        dbg.nospace() << itemNames[itemList[i]].toStdString().c_str();
    }
    dbg.nospace() << "}";

    return dbg.nospace();
}
