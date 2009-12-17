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

QPair< QHash<ItemID, ItemName>, QHash<ItemID, ItemCount> > ARFFParser::parseItemNamesAndSupportCounts() {
    QHash<ItemID, ItemName> names;
    QHash<ItemID, ItemCount> supportCounts;

    /*
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
    */


    names.insert(0, QString("bread"));
    names.insert(1, QString("milk"));
    names.insert(2, QString("diapers"));
    names.insert(3, QString("beer"));
    names.insert(4, QString("eggs"));
    names.insert(5, QString("coke"));

    supportCounts.insert(0, 4);
    supportCounts.insert(1, 4);
    supportCounts.insert(2, 4);
    supportCounts.insert(3, 3);
    supportCounts.insert(4, 1);
    supportCounts.insert(5, 2);
    return qMakePair(names, supportCounts);
}

void ARFFParser::parseTransactions() {
    /*
    Transaction* transactions = new Transaction[10];
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
    */

    Transaction* transactions = new Transaction[5];
    Item i0 = {0, 1};
    Item i1 = {1, 1};
    Item i2 = {2, 1};
    Item i3 = {3, 1};
    Item i4 = {4, 1};
    Item i5 = {5, 1};
    transactions[0] << i0 << i1;
    transactions[1] << i0 << i2 << i3 << i4;
    transactions[2] << i1 << i2 << i3 << i5;
    transactions[3] << i0 << i1 << i2 << i3;
    transactions[4] << i0 << i1 << i2 << i5;

    for (int i = 0; i < 5; i++)
        emit parsedTransaction(transactions[i]);
}

