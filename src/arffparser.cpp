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
QPair<QPair<ItemNQHash, ItemCountHash>, unsigned int> ARFFParser::parseItemProperties() {
    QHash<int, ItemName> columnNames;
    QHash<ItemID, NameQuantity> nqs;
    QHash<ItemID, SupportCount> supportCounts;
    bool dataSection = false;
    this->numColumns = 0;

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
    int columnIndex = 0;
    int columnNameIndex = 0;
    unsigned int numberTransactions = 0;

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
            if (parts[0].compare("@attribute") == 0) {
                // Parse "@attribute BoughtI1 numeric" lines and store the "BoughtI1".
                if (parts.size() >= 3 && parts[2].compare("numeric") == 0) {
                    columnNames.insert(columnIndex, parts[1]);
                    QHash<Quantity, ItemID> hash;
                    this->itemIDMapping.insert(columnNameIndex, hash);
                    columnNameIndex++;
                }
                else {
                    // Remember which columns are irrelevant to us.
                    this->irrelevantColumns.append(columnIndex);
                }
                columnIndex++;
                this->numColumns++;
            }
        }
        else {
            parts = line.split(",");
            // Parse "T100,1,5,0,7,?" lines.
            if (parts.size() == this->numColumns) { // Only look at valid lines.
                numberTransactions++;
                for (int i = 0; i < parts.size(); i++) {
                    // Skip non-numerical columns.
                    if (this->irrelevantColumns.contains(i))
                        continue;

                    // Trim each part.
                    parts[i] = parts[i].trimmed();

                    // Check if there is no value for this column.
                    if (parts[i].compare("?") == 0)
                        continue;

                    quantity = atoi(parts[i].toStdString().c_str());

                    // A quantity of zero is essentially the same as "no value"
                    // (i.e. the question mark). Skip this item.
                    if (quantity == 0)
                        continue;

                    itemName = columnNames[i];

                    // Check if an item ID has already been created for this
                    // (ItemName, Quantity) combination. Create one if necessary.
                    if (itemIDMapping[i].contains(quantity))
                        itemID = itemIDMapping[i][quantity];
                    else {
                        itemID = nextItemID++;
                        // Name and quantity.
                        nq.name = itemName;
                        nq.quantity = quantity;
                        nqs.insert(itemID, nq);
                        // Store the itemID: it will be necessary when parsing
                        // transactions.
                        this->itemIDMapping[i].insert(quantity, itemID);
                    }

                    // Increase the support count.
                    supportCounts[itemID]++;
                }
            }
        }
    }

    return qMakePair(qMakePair(nqs, supportCounts), numberTransactions);
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
            if (parts.size() == this->numColumns) { // Only look at valid lines.
                for (int i = 0; i < parts.size(); i++) {
                    // Skip non-numerical columns.
                    if (this->irrelevantColumns.contains(i))
                        continue;

                    // Trim each part.
                    parts[i] = parts[i].trimmed();

                    // Check if there is no value for this column.
                    if (parts[i].compare("?") == 0)
                        continue;

                    quantity = atoi(parts[i].toStdString().c_str());

                    // A quantity of zero is essentially the same as "no value"
                    // (i.e. the question mark). Skip this item.
                    if (quantity == 0)
                        continue;

                    item.id = this->itemIDMapping[i][quantity];
                    transaction.append(item);
                }

                // A transaction has been parsed, emit it and then clear it.
                emit parsedTransaction(transaction);
                transaction.clear();
            }
        }
    }
}
