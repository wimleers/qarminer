#include "arffparser.h"
#include "fpnode.h"
#include "fptree.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {


/*
a,b
b,c,d
a,c,d,e
a,d,e
a,b,c
a,b,c,d
a
a,b,c
a,b,d
b,c,e
*/
FPNode * root = new FPNode(2);
FPNode * firstChild = new FPNode(3);
FPNode * secondChild = new FPNode(4);
root->addChild(firstChild);
root->addChild(secondChild);
firstChild->setSibling(secondChild);
delete secondChild;
delete firstChild;
delete root;

cout << "yarrd";

}




