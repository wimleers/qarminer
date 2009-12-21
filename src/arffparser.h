#ifndef ARFFPARSER_H
#define ARFFPARSER_H

#include <QObject>

#include <QHash>
#include <QString>
#include <QStringList>
#include <QFile>
#include "fptree.h"
#include "fpnode.h"

class ARFFParser : public QObject {
    Q_OBJECT
protected:
    QString filename;
    QHash<unsigned int, QHash<Quantity, ItemID> > itemIDMapping;

public:
    ARFFParser();

    void setFile(QString filename) { this->filename = filename; }
    QPair<ItemNQHash, ItemCountHash> parseItemProperties();
    void parseTransactions();

signals:
    void parsedTransaction(Transaction transaction);
};

#endif // ARFFPARSER_H
