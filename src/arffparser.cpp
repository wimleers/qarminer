#include "arffparser.h"

ARFFParser::ARFFParser() {
    // Nothing to do.
}

/**
 * Parses all item properties in the dataset:
 *   - names
 *   - quantities
 *   - support counts
 * A name and a quantity are necessary to restore the original representation
 * of an item. Support counts are necessary to calculate frequent itemsets and
 * to generate basic quantitative rules. Quantities are also necessary for
 * generating advanced quantitative rules.
 */
QPair<ItemNQHash, ItemCountHash> ARFFParser::parseItemProperties() {
    QList<ItemName> columnNames;
    QHash<ItemID, NameQuantity> nqs;
    QHash<ItemID, SupportCount> supportCounts;
    bool dataSection = false;

    QFile file(this->filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        exit(1);

    QTextStream in(&file);
    QString line;
    QStringList parts;
    ItemID nextItemID = 0, itemID;
    ItemName itemName;
    Quantity quantity;
    NameQuantity nq;
    unsigned int columnNameIndex;

    while (!in.atEnd()) {
        line = in.readLine();

        // Skip empty lines.
        if (line.trimmed().size() == 0)
            continue;

        // Detect if we're entering the data section.
        if (!dataSection && line.trimmed().compare("@data") == 0)
            dataSection = true;

        // Parse the header section.
        if (!dataSection) {
            parts = line.split(" ");
            // Parse "@attribute BoughtI1 numeric" lines and keep the "BoughtI1".
            if (parts.size() >= 3 && parts[2].compare("numeric") == 0) {
                columnNames.append(parts[1]);
                QHash<Quantity, ItemID> hash;
                this->itemIDMapping.insert(columnNames.size() - 1, hash);
            }
        }
        else {
            parts = line.split(",");
            // Parse "T100,1,5,0,7,?" lines.
            if (parts.size() == columnNames.size() + 1) { // Only look at valid lines.
                for (int i = 1; i < parts.size(); i++) {
                    // Trim each part.
                    parts[i] = parts[i].trimmed();

                    // Check if there is no value for this column.
                    if (parts[i].compare("?") == 0)
                        continue;

                    // Retrieve the item name for the current column and parse
                    // the quantity for it.
                    columnNameIndex = i - 1;
                    itemName = columnNames[columnNameIndex];
                    quantity = atoi(parts[i].toStdString().c_str());

                    // Check if an item ID has already been created for this
                    // (ItemName, Quantity) combination. Create one if necessary.
                    if (itemIDMapping[columnNameIndex].contains(quantity))
                        itemID = itemIDMapping[columnNameIndex][quantity];
                    else {
                        itemID = nextItemID++;
                        // Name and quantity.
                        nq.name = itemName;
                        nq.quantity = quantity;
                        nqs.insert(itemID, nq);
                        // Store the itemID: it will be necessary when parsing
                        // transactions.
                        this->itemIDMapping[columnNameIndex].insert(quantity, itemID);
                    }

                    // Increase the support count.
                    supportCounts[itemID]++;
                }
            }
        }
    }

    return qMakePair(nqs, supportCounts);
}

void ARFFParser::parseTransactions() {
    bool dataSection = false;

    QFile file(this->filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        exit(1);

    QTextStream in(&file);
    QString line;
    QStringList parts;
    Quantity quantity;
    Transaction transaction;
    Item item;
    item.supportCount = 1; // The support count for each parsed item is 1: it can never be >1 within a single transaction.
    unsigned int columnNameIndex;

    while (!in.atEnd()) {
        line = in.readLine();

        // Skip empty lines.
        if (line.trimmed().size() == 0)
            continue;

        // Detect if we're entering the data section.
        if (!dataSection && line.trimmed().compare("@data") == 0)
            dataSection = true;

        // Parse the data section.
        if (dataSection) {
            parts = line.split(",");
            // Parse "T100,1,5,0,7,?" lines.
            if (parts.size() == this->itemIDMapping.size() + 1) { // Only look at valid lines.
                for (int i = 1; i < parts.size(); i++) {
                    // Trim each part.
                    parts[i] = parts[i].trimmed();

                    // Check if there is no value for this column.
                    if (parts[i].compare("?") == 0)
                        continue;

                    columnNameIndex = i - 1;
                    quantity = atoi(parts[i].toStdString().c_str());
                    item.id = this->itemIDMapping[columnNameIndex][quantity];
                    transaction.append(item);
                }

                // A transaction has been parsed, emit it and then clear it.
                emit parsedTransaction(transaction);
                transaction.clear();
            }
        }
    }
}
