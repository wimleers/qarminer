#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "stdint.h"
#include <QDebug>
#include <QHash>
#include <QList>
#include <QString>

// Generic data mining types.
typedef uint16_t ItemID; // Supports 65536 *different* items. Change to uint32_t or uint64_t to support more.
#define ROOT_ITEMID 65535 // Largest supported value for uint16_t.
typedef QString ItemName;
typedef uint16_t SupportCount; // Supports 65536 *total* items. Change to uint32_t or uint64_t to support more.
typedef uint8_t Amount; // Supports amounts up to 256. Change to uint16_t, uint32_t or uint64_t to support more.
struct ItemStruct { ItemID id; SupportCount supportCount; };
typedef ItemStruct Item;

// Generic data mining container types.
typedef QHash<ItemID, ItemName> ItemNameHash;
typedef QHash<ItemID, SupportCount> ItemCountHash;
typedef QList<ItemID> ItemIDList;
typedef QList<Item> ItemList;
typedef QList<SupportCount> ItemCountList;
typedef QList<Item> Transaction;

// Generic data mining types that support named output.
struct NamedItemStruct { Item item; ItemNameHash itemNames; };
typedef NamedItemStruct NamedItem;
struct NamedItemIDStruct { ItemID itemID; ItemNameHash itemNames; };
typedef NamedItemIDStruct NamedItemID;
struct NamedItemListStruct { ItemList items; ItemNameHash itemNames; };
typedef NamedItemListStruct NamedItemList;
struct NamedItemIDListStruct { ItemIDList itemIDs; ItemNameHash itemNames; };
typedef NamedItemIDListStruct NamedItemIDList;
struct NamedTransactionStruct { Transaction transaction; ItemNameHash itemNames; };
typedef NamedTransactionStruct NamedTransaction;


// FP-growth specific types.
class FPNode; // Work around cyclical dependency.

// FP-growth specific container types.
typedef QHash<ItemID, FPNode*> ItemIDFPNodeHash;
typedef QList<FPNode*> FPNodeList;



// QDebug() streaming output operators.
QDebug operator<<(QDebug dbg, const NamedItemID &namedItem);
QDebug operator<<(QDebug dbg, const NamedItem &namedItem);
QDebug operator<<(QDebug dbg, const Transaction &transaction);
QDebug operator<<(QDebug dbg, const NamedTransaction &namedTransaction);
QDebug operator<<(QDebug dbg, const NamedItemIDList &namedItemIDList);
QDebug operator<<(QDebug dbg, const NamedItemList &namedItemList);

#endif // TYPEDEFS_H
