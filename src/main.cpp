#include <QTextStream>
#include <QTime>

#include "typedefs.h"
#include "arffparser.h"
#include "fpnode.h"
#include "fptree.h"
#include "fpgrowth.h"
#include "ruleminer.h"


int main(int argc, char *argv[]) {
    QTextStream cout(stdout);
    QTime phaseTimer;
    FPGrowth* fpgrowth;
    int duration;
    QList<ItemList> frequentItemsets;
    QList<SupportCount> frequentItemsetsSupportCounts;
    QList<int> durations;

    QString filename;
    float minimumSupport;
    float minimumConfidence;

    if (argc < 2) {
        QTextStream cerr(stderr);
        cerr << "Usage: DMP <inputfile> <minsup> <minconf>" << endl
             << "\t- inputfile must be a .arff file with numeric attributes" << endl
             << "\t- minsup must be a float (percentage)" << endl
             << "\t- minconf must be a float (percentage)";


        cerr.flush();
        exit(1);
    }
    else {
        filename = QString(argv[1]);
        minimumSupport = atof(argv[2]);
        minimumConfidence = atof(argv[3]);
    }


    fpgrowth = new FPGrowth(filename, minimumSupport);


    // Preprocessing stages.
    cout << "|- PREPROCESSING" << endl;

    // Stage 1.
    cout << "  |- Preprocessing stage 1: parsing item names, quantities and support counts." << endl;
    phaseTimer.start();
    fpgrowth->preprocessingPhase1();
    durations.append(phaseTimer.elapsed());
    cout << QString("    |- Duration: %1 ms.").arg(durations.last()) << endl;


    // Stage 2.
    cout << "  |- Preprocessing stage 2: parsing transactions and building an FP-tree." << endl;
    phaseTimer.start();
    fpgrowth->preprocessingPhase2();
    durations.append(phaseTimer.elapsed());
    cout << QString("    |- Duration: %1 ms.").arg(durations.last()) << endl;


    // Calculating stages.
    cout << "|- CALCULATING" << endl;

    // Stage 3.
    cout << "  |- Calculating stage 1: frequent itemset generation." << endl;
    phaseTimer.start();
    frequentItemsets = fpgrowth->calculatingPhase1();
    durations.append(phaseTimer.elapsed());
    cout << QString("    |- Duration: %1 ms.").arg(durations.last()) << endl;

    // Stage 4.
    cout << "  |- Calculating stage 2: calculate support for frequent itemsets." << endl;
    phaseTimer.start();
    frequentItemsetsSupportCounts = fpgrowth->calculatingPhase2(frequentItemsets);
    durations.append(phaseTimer.elapsed());
    cout << QString("    |- Duration: %1 ms.").arg(durations.last()) << endl;


    // Stage 5.
    cout << "  |- Calculating stage 3: rule generation" << endl;
    phaseTimer.start();
    QList<AssociationRule> associationRules = RuleMiner::generateAssociationRules(frequentItemsets, frequentItemsetsSupportCounts, minimumConfidence);
    durations.append(phaseTimer.elapsed());
    cout << QString("    |- Duration: %1 ms.").arg(durations.last()) << endl;


    // Actual processing time.
    duration = 0;
    for (int i = 1; i < durations.size(); i++)
        duration += durations[i];
    cout << QString("ACTUAL PROCESSING TIME: %1 ms (excludes preprocesing stage 1)").arg(duration) << endl;

    // Total time.
    duration = 0;
    foreach (int d, durations)
        duration += d;
    cout << QString("TOTAL PROCESSING TIME: %1 ms (all stages)").arg(duration) << endl;

    // Output rules of type A.
    cout << endl << "Rules of type A:" << endl;
    ItemNQHash itemNamesAndQuantities = fpgrowth->getItemNQs();
    foreach (AssociationRule rule, associationRules) {
        // Prefix.
        cout << "\t";

        // Antecedent.
        cout << "{";
        for (int i = 0; i < rule.antecedent.size(); i++) {
            if (i > 0)
                cout << ", ";
            Item item = rule.antecedent[i];
            cout << itemNamesAndQuantities[item.id].name.toStdString().c_str();
            cout << "=";
            cout << itemNamesAndQuantities[item.id].quantity;
        }
        cout << "}";

        // Arrow.
        cout << " => ";

        // Consequent;
        cout << "{";
        for (int i = 0; i < rule.consequent.size(); i++) {
            if (i > 0)
                cout << ", ";
            Item item = rule.consequent[i];
            cout << itemNamesAndQuantities[item.id].name.toStdString().c_str();
            cout << "=";
            cout << itemNamesAndQuantities[item.id].quantity;
        }
        cout << "}";

        // Confidence.
        cout << " (conf=" << rule.confidence << ")" << endl;
    }

    delete fpgrowth;

    return 0;
}




