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
    QList<int> irrelevantColumns;
    int numColumns;

public:
    ARFFParser();

    void setFile(QString filename) { this->filename = filename; }
    QPair<QPair<ItemNQHash, ItemCountHash>, unsigned int> parseItemProperties();
    void parseTransactions();

signals:
    void parsedTransaction(Transaction transaction);
};

#endif // ARFFPARSER_H
