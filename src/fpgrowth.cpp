#include "fpgrowth.h"

FPGrowth::FPGrowth(QString filename) {
    this->parser.setFile(filename);
    this->tree = new FPTree();

    qDebug() << "Preprocessing stage 1: parsing item names and support counts";
    QPair< QHash<Item, ItemName>, QHash<Item, ItemCount> > result;
    result = this->parser.parseItemNamesAndSupportCounts();
    this->itemNames = result.first;
    this->supportCounts = result.second;
    // Let the tree know the names of the items, so it can print those instead
    // of the corresponding integers when printing the tree.
    this->tree->setItemNames(&this->itemNames);

    qDebug() << "Preprocessing stage 2: parsing transactions";
    QObject::connect(&(this->parser), SIGNAL(parsedTransaction(Transaction)), this, SLOT(parsedTransaction(Transaction)));
    // The slot that parser.parseTransactions() will call, will fill the FPTree.
    this->parser.parseTransactions();


    qDebug() << "Calculating stage 1: not yet implemented";
    qDebug() << *this->tree;
}


void transactionParserCallback(int transaction) {//Transaction transaction) {
    qDebug() << "parsed" << transaction;
}


//------------------------------------------------------------------------------
// Private slots.

void FPGrowth::parsedTransaction(Transaction transaction) {
    // Add the transaction that was parsed to the tree.
    this->tree->addTransaction(transaction);
}
