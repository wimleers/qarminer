#include <QCoreApplication>

#include "arffparser.h"
#include "fpnode.h"
#include "fptree.h"

#include <iostream>
#include <QDebug>
using namespace std;

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    qDebug() << "FPNODE TEST";
    FPNode * root = new FPNode(ROOT_ITEM);
    FPNode * firstChild = new FPNode(3, root);
    FPNode * secondChild = new FPNode(4, root);
    qDebug() << "Root has child with value 4?" << root->hasChild(4);
    qDebug() << *root << *firstChild << *secondChild;
    delete root;

    qDebug() << "FPROOT TEST";
    FPTree * tree = new FPTree();
    QList<int> t1, t2, t3;
    t1 << 1 << 4 << 8;
    t2 << 2 << 37 << 345;
    t3 << 1 << 4 << 48;
    tree->addTransaction(t1);
    tree->addTransaction(t2);
    tree->addTransaction(t3);
    qDebug() << *tree;
    delete tree;

    cout << "FINISHED" << endl;

    return 0;
}




