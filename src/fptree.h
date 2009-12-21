#ifndef FPTREE_H
#define FPTREE_H

#include <QHash>
#include <QPair>
#include <QDebug>
#include <QMetaType>
#include <QString>

#include "typedefs.h"
#include "fpnode.h"

class FPTree {
protected:
    FPNode* root;
    QHash<ItemID, FPNodeList> itemPaths;
#ifdef DEBUG
    ItemNQHash* itemNQs;
#endif

    void addNodeToItemPath(FPNode* node);
    void removeNodeFromItemPath(FPNode* node);

public:
    FPTree();
    ~FPTree();

    // Accessors.
    FPNode* getRoot() const { return this->root; }
    bool hasItemPath(ItemID itemID) const;
    QList<ItemID> getItemIDs() const { return this->itemPaths.keys(); }
    FPNodeList getItemPath(ItemID itemID) const;
    bool itemPathContains(ItemID itemID, FPNode* node) const;
    SupportCount getItemSupport(ItemID item) const;
    QList<ItemList> calculatePrefixPaths(ItemID itemID) const;

    // Modifiers.
    void addTransaction(Transaction transaction);

    // Static (class) methods.
    static ItemCountHash calculateSupportCountsForPrefixPaths(QList<ItemList> prefixPaths);

#ifdef DEBUG
    // Debug output.
    ItemNQHash* getItemNQs() const { return this->itemNQs; }
    void setItemNQs(ItemNQHash* itemNQs) { this->itemNQs = itemNQs; }
#endif
};

Q_DECLARE_METATYPE(FPTree);

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const FPTree &tree);
QString dumpHelper(const FPNode &node, QString prefix = "");
#endif

#endif // FPTREE_H
