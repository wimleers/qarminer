#include "arffparser.h"

ARFFParser::ARFFParser() {
    // Nothing to do.
}

/*
  Mocked transaction data.
a,b
b,c,d
a,c,d,e
a,d,e
a,b,c
a,b,c,d
a
a,b,c
a,b,d
b,c,e
*/

QPair< QHash<Item, ItemName>, QHash<Item, ItemCount> > ARFFParser::parseItemNamesAndSupportCounts() {
    QHash<Item, ItemName> names;
    QHash<Item, ItemCount> supportCounts;

    names.insert(0, QString("a"));
    names.insert(1, QString("b"));
    names.insert(2, QString("c"));
    names.insert(3, QString("d"));
    names.insert(4, QString("e"));

    supportCounts.insert(0, 8);
    supportCounts.insert(1, 7);
    supportCounts.insert(2, 6);
    supportCounts.insert(3, 5);
    supportCounts.insert(4, 2);

    return qMakePair(names, supportCounts);
}

void ARFFParser::parseTransactions() {
    Transaction * transactions = new Transaction[10];
    transactions[0] << 0 << 1;
    transactions[1] << 1 << 2 << 3;
    transactions[2] << 0 << 2 << 3 << 4;
    transactions[3] << 0 << 3 << 4;
    transactions[4] << 0 << 1 << 2;
    transactions[5] << 0 << 1 << 2 << 3;
    transactions[6] << 0;
    transactions[7] << 0 << 1 << 2;
    transactions[8] << 0 << 1 << 3;
    transactions[9] << 1 << 2 << 4;

    for (int i = 0; i < 10; i++)
        emit parsedTransaction(transactions[i]);
}

