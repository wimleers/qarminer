#ifndef FPGROWTH_H
#define FPGROWTH_H

#include <QObject>
#include <QHash>
#include <QString>
#include "arffparser.h"
#include "fptree.h"
#include "fpnode.h"

class FPGrowth : public QObject {
    Q_OBJECT
protected:
    ARFFParser parser;
    ItemNameHash itemNames;
    ItemCountHash supportCounts;
    FPTree* tree;

public:
    FPGrowth(QString filename);

private slots:
    void parsedTransaction(Transaction transaction);
};

#endif // FPGROWTH_H
