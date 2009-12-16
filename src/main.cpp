#include <QCoreApplication>

#include "arffparser.h"
#include "fpnode.h"
#include "fptree.h"
#include "fpgrowth.h"

#include <iostream>
#include <QDebug>
using namespace std;

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
/*
    qDebug() << "FPNODE TEST";
    FPNode * root = new FPNode(ROOT_ITEM);
    FPNode * firstChild = new FPNode(3, root);
    FPNode * secondChild = new FPNode(4, root);
    qDebug() << "Root has child with value 4?" << root->hasChild(4);
    qDebug() << *root << *firstChild << *secondChild;
    delete root;

    qDebug() << "FPROOT TEST";
    FPTree * tree = new FPTree();
    Transaction t1, t2, t3;
    t1 << 1 << 4 << 8;
    t2 << 2 << 37 << 345 << 4;
    t3 << 1 << 4 << 48;
    tree->addTransaction(t1);
    tree->addTransaction(t2);
    tree->addTransaction(t3);
    qDebug() << *tree;
    qDebug() << "transaction 1:" << t1;
    qDebug() << "transaction 2:" << t2;
    qDebug() << "transaction 3:" << t3;
    qDebug() << "item path for 1:" << tree->getItemPath(1);
    qDebug() << "item path for 4:" << tree->getItemPath(4);
    delete tree;
*/

    FPGrowth * growth = new FPGrowth(QString("input"), 2);
    delete growth;

    return 0;
}




