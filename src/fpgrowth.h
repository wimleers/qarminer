#ifndef FPGROWTH_H
#define FPGROWTH_H

#include <QObject>
#include <QHash>
#include <QSet>
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
    int minimumSupport;

public:
    FPGrowth(QString filename, int minimumSupport);

private slots:
    void parsedTransaction(Transaction transaction);
};

#endif // FPGROWTH_H
