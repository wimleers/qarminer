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
    ItemNQHash* itemNQs;

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
    ItemNQHash* getItemNQs() const { return this->itemNQs; }
    SupportCount getItemSupport(ItemID item) const;
    QList<ItemList> calculatePrefixPaths(ItemID itemID) const;

    // Modifiers.
    void addTransaction(Transaction transaction);
    void setItemNQs(ItemNQHash* itemNQs) { this->itemNQs = itemNQs; }

    // Static (class) methods.
    static ItemCountHash calculateSupportCountsForPrefixPaths(QList<ItemList> prefixPaths);
};

Q_DECLARE_METATYPE(FPTree);

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const FPTree &tree);
QString dumpHelper(const FPNode &node, QString prefix = "");
#endif

#endif // FPTREE_H
