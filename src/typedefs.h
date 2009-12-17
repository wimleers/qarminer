#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include "stdint.h"
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


// FP-growth specific types.
class FPNode; // Work around cyclical dependency.

// FP-growth specific container types.
typedef QHash<Item, FPNode*> ItemFPNodeHash;
typedef QList<FPNode*> FPNodeList;


#endif // TYPEDEFS_H
