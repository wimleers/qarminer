#ifndef FPGROWTH_H
#define FPGROWTH_H

#include <QHash>
#include <QString>
#include "arffparser.h"
#include "fptree.h"
#include "fpnode.h"

class FPGrowth {
protected:
    ARFFParser parser;
    QHash<Item, QPair<ItemName, ItemCount> > supportCount;

    void transactionParserCallback(int transaction);

public:
    FPGrowth(QString filename);
};

#endif // FPGROWTH_H
