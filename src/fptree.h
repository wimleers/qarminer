#ifndef FPTREE_H
#define FPTREE_H

#include <QHash>
#include <QPair>
#include <QList>
#include <QDebug>
#include <QMetaType>
#include <QString>

#include "fpnode.h"

class FPTree {
protected:
    FPNode * root;
    // Hashmap of list of FPNode*s.
    QHash<int, QList<FPNode*> > itemPaths;

    void updateItemPath(FPNode * node);

public:
    FPTree();
    ~FPTree();

    FPNode * getRoot() const { return this->root; }

    void addTransaction(QList<int> transaction);
};

Q_DECLARE_METATYPE(FPTree);

QDebug operator<<(QDebug dbg, const FPTree &tree);
QString dumpHelper(const FPNode &node, QString prefix = "");

#endif // FPTREE_H
