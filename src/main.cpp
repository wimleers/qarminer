#include <QTextStream>
#include <QTime>

#include "typedefs.h"
#include "arffparser.h"
#include "fpnode.h"
#include "fptree.h"
#include "fpgrowth.h"


int main(int argc, char *argv[]) {
    QTextStream cout(stdout);
    QTime timer;
    int duration;

    if (argc < 2) {
        QTextStream cerr(stderr);
        cerr << "Usage: dmp <inputfile> <minsup> <minconf>" << endl
             << "\t- inputfile must be a .arff file with numeric attributes" << endl
             << "\t- minsup must be an unsigned integer" << endl
             << "\t- minconf must be a float";


        cerr.flush();
        exit(1);
    }

    timer.start();
    FPGrowth* growth = new FPGrowth(QString(argv[1]), atoi(argv[2]), (float) atof(argv[3]));
    duration = timer.elapsed();
    delete growth;

    cout << QString("Time elapsed: %1 ms.").arg(duration);

    return 0;
}




