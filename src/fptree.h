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
    FPNode * root;
    // Hashmap of list of FPNode*s.
    QHash<int, ItemPath> itemPaths;

    void addNodeToItemPath(FPNode* node);
    void removeNodeFromItemPath(FPNode* node);

public:
    FPTree();
    ~FPTree();

    FPNode* getRoot() const { return this->root; }

    bool hasItemPath(int item) const;
    ItemPath getItemPath(int item) const;
    bool itemPathContains(int item, FPNode* node) const;

    void addTransaction(Transaction transaction);

    FPTree* getConditionalFPTreeFor(int item);
};

Q_DECLARE_METATYPE(FPTree);

QDebug operator<<(QDebug dbg, const FPTree &tree);
QString dumpHelper(const FPNode &node, QString prefix = "");
QDebug operator<<(QDebug dbg, const ItemPath &itempath);
QDebug operator<<(QDebug dbg, const Transaction &transaction);

#endif // FPTREE_H
