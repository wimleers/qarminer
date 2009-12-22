#include "fptree.h"

//------------------------------------------------------------------------------
// Public methods.

FPTree::FPTree() {
    Item rootItem;
    rootItem.id = ROOT_ITEMID;
    rootItem.supportCount = 0;
    root = new FPNode(rootItem);
}

FPTree::~FPTree() {
    delete root;
}

bool FPTree::hasItemPath(ItemID itemID) const {
    return this->itemPaths.contains(itemID);
}

FPNodeList FPTree::getItemPath(ItemID itemID) const {
    if (this->itemPaths.contains(itemID))
        return this->itemPaths[itemID];
    else {
        FPNodeList empty;
        return empty;
    }
}

bool FPTree::itemPathContains(ItemID itemID, FPNode* node) const {
    return (this->itemPaths.contains(itemID) && this->itemPaths[itemID].contains(node));
}

SupportCount FPTree::getItemSupport(ItemID itemID) const {
    SupportCount supportCount = 0;
    foreach (FPNode* node, this->itemPaths[itemID])
        supportCount += node->getCount();
    return supportCount;
}

/**
 * Calculate prefix paths that end with a node that has the given ItemID. These
 * nodes can be retrieved very quickly using the FPTree's itemPaths.
 * A prefix path is a list of Items that reflects a path from the bottom of the
 * tree to the root (but excluding the root), following along the path of an
 * FPNode that has the ItemID itemID. Because it is a list of Items, it also
 * includes both the ItemID and the ItemCount. The original ItemCount of the
 * FPNode is erased and replaced by the ItemCount of the FPNode we began from,
 * i.e. a node that has the ItemID itemID, because we're looking at only the
 * paths that include this node.
 */
QList<ItemList> FPTree::calculatePrefixPaths(ItemID itemID) const {
    QList<ItemList> prefixPaths;
    ItemList prefixPath;
    FPNode* node;
    SupportCount supportCount;
    Item item;

    FPNodeList leafNodes = this->getItemPath(itemID);
    foreach (FPNode* leafNode, leafNodes) {
        // Build the prefix path starting from the given leaf node, by
        // traversing up the tree.
        // Don't copy the item's original count, but the count of the leaf node
        // instead, because we're looking at only the paths that include this
        // leaf node.
        node = leafNode;
        supportCount = leafNode->getCount();
        prefixPath.prepend(node->getItem());
        while ((node = node->getParent()) != NULL && node->getItemID() != ROOT_ITEMID) {
            item = node->getItem();
            item.supportCount = supportCount;
            prefixPath.prepend(item);
        }

        // Store the built prefix path & clear it, so we can calculate the next.
        prefixPaths.append(prefixPath);
        prefixPath.clear();
    }

    return prefixPaths;
}

ItemCountHash FPTree::calculateSupportCountsForPrefixPaths(QList<ItemList> prefixPaths) {
    ItemCountHash supportCounts;

    foreach (ItemList prefixPath, prefixPaths)
        foreach (Item item, prefixPath)
            supportCounts[item.id] = (supportCounts.contains(item.id)) ? supportCounts[item.id] + item.supportCount : item.supportCount;

    return supportCounts;
}

void FPTree::addTransaction(Transaction transaction) {
    // The initial current node is the root node.
    FPNode* currentNode = root;

    FPNode* nextNode;

    foreach (Item item, transaction) {
        if (currentNode->hasChild(item.id)) {
            // There is already a node in the tree for the current transaction
            // item, so reuse it: increase its count.
            nextNode = currentNode->getChild(item.id);
            qDebug() << "\t\t\t\t============increasing support count by " << item.supportCount;
            nextNode->increaseCount(item.supportCount);
        }
        else {
            // Create a new node and add it as a child of the current node.
            nextNode = new FPNode(item, currentNode);
#ifdef DEBUG
            nextNode->setItemNQs(this->getItemNQs());
#endif
            // Update the item path to include the new node.
            this->addNodeToItemPath(nextNode);
        }

        // We've processed this item in the transaction, time to move on to the
        // next!
        currentNode = nextNode;
        nextNode = NULL;
    }
}


//------------------------------------------------------------------------------
// Protected methods.

void FPTree::addNodeToItemPath(FPNode* node) {
    FPNodeList itemPath;

    ItemID itemID = node->getItemID();

    // If there already is an item path for this item, load it so it can be
    // updated.
    if (this->itemPaths.contains(itemID))
        itemPath = this->itemPaths[itemID];

    itemPath.append(node);
    this->itemPaths.insert(itemID, itemPath);
}

/**
 * Should be called when a node is being removed from the tree. This ensures
 * item paths stay up-to-date.
 */
void FPTree::removeNodeFromItemPath(FPNode* node) {
    ItemID itemID = node->getItemID();

    if (this->itemPaths.contains(itemID) && this->itemPaths[itemID].contains(node)) {
        // Get the current item path.
        FPNodeList itemPath = this->itemPaths[itemID];
        // Update it.
        itemPath.removeAll(node);
        // And insert it (will overwrite the existing value).
        this->itemPaths.insert(itemID, itemPath);
    }
}


//------------------------------------------------------------------------------
// Other.

#ifdef DEBUG
QDebug operator<<(QDebug dbg, const FPTree &tree) {
    // Tree.
    dbg.nospace() << "TREE" << endl;
    dbg.nospace() << dumpHelper(*(tree.getRoot())).toStdString().c_str();

    // Item paths.
    dbg.nospace() << "ITEM PATHS" << endl;
    FPNodeList itemPath;
    ItemNQHash* itemNQs = tree.getItemNQs();
    foreach (ItemID itemID, tree.getItemIDs()) {
        itemPath = tree.getItemPath(itemID);
        dbg.nospace() << " - item path for "
                      << (*itemNQs)[itemID].name.toStdString().c_str()
                      << ":"
                      << (*itemNQs)[itemID].quantity
                      << "= " << itemPath << endl;
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
        foreach (FPNode* child, node.getChildren()) {
            if (firstChild)
                s += prefix;
            else
                firstChild = false;
            s += "->" + dumpHelper(*child, prefix + suffix);
        }
    }

    return s;
}
#endif
