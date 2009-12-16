#include "fpgrowth.h"

FPGrowth::FPGrowth(QString filename) {
    this->parser.setFile(filename);

    qDebug() << "Preprocessing stage 1: parsing support counts";
    this->supportCount = this->parser.parseSupportCounts();

    for (int i = 0; i < this->supportCount.size(); i++)
        qDebug() << this->supportCount[i].first << ":" << this->supportCount[i].second;

    qDebug() << "Preprocessing stage 2: parsing support counts";
    //this->parser.parseTransactions(this->transactionParserCallback);
}


void transactionParserCallback(int transaction) {//Transaction transaction) {
    qDebug() << "parsed" << transaction;
}
