#include <QDebug>
#include <QTextStream>

#include "typedefs.h"
#include "arffparser.h"
#include "fpnode.h"
#include "fptree.h"
#include "fpgrowth.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        QTextStream cerr(stderr);
        cerr << "Usage: dmp <inputfile> <minsup> <minconf>" << endl
             << "\t- inputfile must be a .arff file with numeric attributes" << endl
             << "\t- minsup must be an unsigned integer" << endl
             << "\t- minconf must be a float";


        cerr.flush();
        exit(1);
    }

    FPGrowth* growth = new FPGrowth(QString(argv[1]), atoi(argv[2]), (float) atof(argv[3]));
    delete growth;

    return 0;
}




