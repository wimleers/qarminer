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
typedef uint8_t Quantity; // Supports quantities up to 256. Change to uint16_t, uint32_t or uint64_t to support more.
struct ItemStruct { ItemID id; SupportCount supportCount; };
typedef ItemStruct Item;
struct QuantitativeItemStruct { ItemID id; Quantity quantity; };
typedef QuantitativeItemStruct QuantitativeItem;
struct NameQuantityStruct { ItemName name; Quantity quantity; };
typedef NameQuantityStruct NameQuantity;


// Generic data mining container types.
typedef QHash<ItemID, NameQuantity> ItemNQHash;
typedef QHash<ItemID, SupportCount> ItemCountHash;
typedef QList<ItemID> ItemIDList;
typedef QList<Item> ItemList;
typedef QList<SupportCount> ItemCountList;
typedef QList<QuantitativeItem> QuantitativeTransaction;
typedef QList<Item> Transaction;
struct AssociationRuleStruct { ItemList antecedent; ItemList consequent; float confidence; };
typedef AssociationRuleStruct AssociationRule;

// Generic data mining types that support named output.
struct NamedItemStruct { Item item; ItemNQHash itemNQs; };
typedef NamedItemStruct NamedItem;
struct NamedItemIDStruct { ItemID itemID; ItemNQHash itemNQs; };
typedef NamedItemIDStruct NamedItemID;
struct NamedItemListStruct { ItemList items; ItemNQHash itemNQs; };
typedef NamedItemListStruct NamedItemList;
struct NamedItemIDListStruct { ItemIDList itemIDs; ItemNQHash itemNQs; };
typedef NamedItemIDListStruct NamedItemIDList;
struct NamedTransactionStruct { Transaction transaction; ItemNQHash itemNQs; };
typedef NamedTransactionStruct NamedTransaction;
struct NamedAssociationRuleStruct { ItemList antecedent; ItemList consequent; float confidence; ItemNQHash itemNQs; };
typedef NamedAssociationRuleStruct NamedAssociationRule;


// FP-growth specific types.
class FPNode; // Work around cyclical dependency.

// FP-growth specific container types.
typedef QHash<ItemID, FPNode*> ItemIDFPNodeHash;
typedef QList<FPNode*> FPNodeList;


// Comparison operators to support Qt's container types' contains() method.
inline bool operator==(const Item &i1, const Item &i2) {
    // Important! We don't require a match on the supportCount attribute!
    return i1.id == i2.id;
}
inline bool operator!=(const Item &i1, const Item &i2) {
    return !(i1 == i2);
}


// QDebug() streaming output operators.
QDebug operator<<(QDebug dbg, const NamedItemID &namedItem);
QDebug operator<<(QDebug dbg, const Item &item);
QDebug operator<<(QDebug dbg, const NamedItem &namedItem);
QDebug operator<<(QDebug dbg, const Transaction &transaction);
QDebug operator<<(QDebug dbg, const NamedTransaction &namedTransaction);
QDebug operator<<(QDebug dbg, const NamedItemIDList &namedItemIDList);
QDebug operator<<(QDebug dbg, const NamedItemList &namedItemList);
QDebug operator<<(QDebug dbg, const AssociationRule &associationRule);
QDebug operator<<(QDebug dbg, const NamedAssociationRule &namedAssociationRule);

#endif // TYPEDEFS_H
