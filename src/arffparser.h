#ifndef ARFFPARSER_H
#define ARFFPARSER_H

#include <QObject>
#include <QHash>
#include <QString>
#include "fptree.h"
#include "fpnode.h"

class ARFFParser : public QObject {
    Q_OBJECT
protected:
    QString filename;

public:
    ARFFParser();

    void setFile(QString filename) { this->filename = filename; }
    QPair<QHash<ItemID, NameQuantity>, ItemCountHash> parseItemProperties();
    void parseTransactions();

signals:
    void parsedTransaction(Transaction transaction);
};

#endif // ARFFPARSER_H
