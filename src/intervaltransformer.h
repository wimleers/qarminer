#ifndef INTERVALTRANSFORMER_H
#define INTERVALTRANSFORMER_H

#include <QPair>
#include <QHash>
#include <QList>
#include "typedefs.h"

class IntervalTransformer {
public:
    static QPair<QHash<ItemID, ItemName>, QHash<ItemID, QPair<Quantity, Quantity> > > transform(QList<ItemList>* frequentItemsets, ItemNQHash itemNamesAndQuantities, float minimumConfidence);
};

#endif // INTERVALTRANSFORMER_H
