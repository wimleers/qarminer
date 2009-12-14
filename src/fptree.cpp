#include "fptree.h"

FPTree::FPTree() {
    root = new FPNode(ROOT_ITEM);
}

FPTree::~FPTree() {
    delete root;
}

void FPTree::updateItemPath(FPNode *node) {
    QList<FPNode*> itemPath;

    int item = node->getItem();

    // If there already is an item path for this item, load it so it can be
    // updated.
    if (this->itemPaths.contains(item))
        QList<FPNode*> itemPath = this->itemPaths.value(item);

    itemPath.append(node);
    this->itemPaths.insert(item, itemPath);
}

void FPTree::addTransaction(QList<int> transaction) {
    // The initial current node is the root node.
    FPNode * currentNode = root;

    FPNode * nextNode;
    int item;

    for (int i = 0; i < transaction.size(); i++) {
        item = transaction.at(i);

        if (currentNode->hasChild(item)) {
            // There is already a node in the tree for the current transaction
            // item, so reuse it: increment its count.
            nextNode = currentNode->getChild(item);
            nextNode->increment();
        }
        else {
            // Create a new node and add it as a child of the current node.
            nextNode = new FPNode(item, currentNode);
            // Update the item path to include the new node.
            this->updateItemPath(nextNode);
        }

        // We've processed this item in the transaction, time to move on to the
        // next!
        currentNode = nextNode;
        nextNode = NULL;
    }
}

QDebug operator<<(QDebug dbg, const FPTree &tree) {
    return dbg.nospace() << dumpHelper(*(tree.getRoot())).toStdString().c_str() << endl;
}

QString dumpHelper(const FPNode &node, QString prefix) {
    static QString suffix = "\t";
    QString s;
    bool firstChild = true;

    // Print current node.
    QDebug(&s) << node << "\n";

    // Print all child nodes.
    if (node.numChildren() > 0) {
        foreach (FPNode * child, node.getChildren()) {
            if (firstChild)
                s += prefix;
            else
                firstChild = false;
            s += "->" + dumpHelper(*child, prefix + suffix);
        }
    }

    return s;
}
