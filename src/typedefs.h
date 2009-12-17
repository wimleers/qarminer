#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "stdint.h"
#include <QDebug>
#include <QHash>
#include <QList>
#include <QString>


// Generic data mining types.
typedef uint16_t Item; // Supports 65536 *different* items. Change to uint32_t or uint64_t to support more.
#define ROOT_ITEM 65535 // Largest supported value for uint16_t.
typedef QString ItemName;
typedef uint16_t ItemCount; // Supports 65536 *total* items. Change to uint32_t or uint64_t to support more.

// Generic data mining container types.
typedef QHash<Item, ItemName> ItemNameHash;
typedef QHash<Item, ItemCount> ItemCountHash;
typedef QList<Item> ItemList;
typedef QList<ItemCount> ItemCountList;
typedef QList<Item> Transaction;

// Generic data mining types that support named output.
struct NamedItemStruct { Item item; ItemNameHash itemNames; };
typedef NamedItemStruct NamedItem;
struct NamedItemListStruct { ItemList items; ItemNameHash itemNames; };
typedef NamedItemListStruct NamedItemList;
struct NamedTransactionStruct { Transaction transaction; ItemNameHash itemNames; };
typedef NamedTransactionStruct NamedTransaction;


// FP-growth specific types.
class FPNode; // Work around cyclical dependency.

// FP-growth specific container types.
typedef QHash<Item, FPNode*> ItemFPNodeHash;
typedef QList<FPNode*> FPNodeList;



// QDebug() streaming output operators.
QDebug operator<<(QDebug dbg, const NamedItem &namedItem);
QDebug operator<<(QDebug dbg, const Transaction &transaction);
QDebug operator<<(QDebug dbg, const NamedTransaction &namedTransaction);
QDebug operator<<(QDebug dbg, const NamedItemList &namedItemList);

#endif // TYPEDEFS_H
