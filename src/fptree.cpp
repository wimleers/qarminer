#include "fptree.h"

//------------------------------------------------------------------------------
// Public methods.

FPTree::FPTree() {
    root = new FPNode(ROOT_ITEM);
}

FPTree::~FPTree() {
    delete root;
}

bool FPTree::hasItemPath(Item item) const {
    return this->itemPaths.contains(item);
}

FPNodeList FPTree::getItemPath(Item item) const {
    if (this->itemPaths.contains(item))
        return this->itemPaths[item];
    else {
        FPNodeList empty;
        return empty;
    }
}

bool FPTree::itemPathContains(Item item, FPNode* node) const {
    return (this->itemPaths.contains(item) && this->itemPaths[item].contains(node));
}

void FPTree::addTransaction(Transaction transaction) {
    // The initial current node is the root node.
    FPNode* currentNode = root;

    FPNode* nextNode;
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
            nextNode->setItemNames(this->getItemNames());
            // Update the item path to include the new node.
            this->addNodeToItemPath(nextNode);
        }

        // We've processed this item in the transaction, time to move on to the
        // next!
        currentNode = nextNode;
        nextNode = NULL;
    }
}

FPTree* FPTree::getConditionalFPTreeFor(Item item) {
/*
    // When this node is being deleted, let the tree it is part of let this know
    // so the tree can update its item paths.
    if (this->tree != NULL)
        this->tree->removeNodeFromItemPath(this);
*/

}


//------------------------------------------------------------------------------
// Protected methods.

void FPTree::addNodeToItemPath(FPNode* node) {
    FPNodeList itemPath;

    int item = node->getItem();

    // If there already is an item path for this item, load it so it can be
    // updated.
    if (this->itemPaths.contains(item))
        itemPath = this->itemPaths[item];

    itemPath.append(node);
    this->itemPaths.insert(item, itemPath);
}

/**
 * Should be called when a node is being removed from the tree. This ensures
 * item paths stay up-to-date.
 */
void FPTree::removeNodeFromItemPath(FPNode* node) {
    Item item = node->getItem();

    if (this->itemPaths.contains(item) && this->itemPaths[item].contains(node)) {
        // Get the current item path.
        FPNodeList itemPath = this->itemPaths[item];
        // Update it.
        itemPath.removeAll(node);
        // And insert it (will overwrite the existing value).
        this->itemPaths.insert(item, itemPath);
    }
}


//------------------------------------------------------------------------------
// Other.

QDebug operator<<(QDebug dbg, const FPTree &tree) {
    // Tree.
    dbg.nospace() << "TREE" << endl;
    dbg.nospace() << dumpHelper(*(tree.getRoot())).toStdString().c_str();

    // Item paths.
    dbg.nospace() << "ITEM PATHS" << endl;
    FPNodeList itemPath;
    ItemNameHash* itemNames = tree.getItemNames();
    foreach (Item item, tree.getItems()) {
        itemPath = tree.getItemPath(item);
        dbg.nospace() << " - item path for "
                      << (*itemNames)[item].toStdString().c_str()
                      << ": " << itemPath << endl;
    }

    return dbg.nospace();
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

QDebug operator<<(QDebug dbg, const FPNodeList &itemPath) {
    dbg.nospace() << "[size=" << itemPath.size() << "]";

    for (int i = 0; i < itemPath.size(); i++) {
        if (i > 0)
            dbg.nospace() << "->";
        dbg.nospace() << *(itemPath[i]);
    }

    return dbg.nospace();
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
