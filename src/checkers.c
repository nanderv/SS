#include <sylvan.h>

// TODO: Prev function
BDD MYprev(BDD in, BDD relations[], int n_relations, BDD x, BDDMAP map) {
    LACE_ME;

    return in;
}

BDD checkEU(BDD left, BDD right, BDD relations[], int n_relations, BDD x, BDDMAP map) 
{
    LACE_ME;
    BDD z = right;
    BDD old = sylvan_false;
    while ( z != old) {
        old = z;
        z = sylvan_or(z, sylvan_and(left, MYprev(z, relations, n_relations,  x, map )));
    }
    return z;
}

BDD checkEG(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) 
{
    LACE_ME;
    BDD z = left;
    BDD old = sylvan_false;
    while ( z != old) {
        old = z;
        z = sylvan_and(z, MYprev(z, relations, n_relations,  x, map ));
    }
    return z;}

BDD checkEX(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) 
{
    LACE_ME;
    return MYprev(left, relations, n_relations,  x, map );
}

BDD checkEF(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) {
 return checkEU(sylvan_true, left, relations, n_relations,  x, map );
}

BDD checkAF(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) {
    BDD im1 = sylvan_not(left);
    im1 = checkEG(im1, relations, n_relations,  x, map );
    return sylvan_not(im1);
}
BDD checkAG(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) {
    BDD im1 = sylvan_not(left);
    im1 = checkEF(im1, relations, n_relations,  x, map );
    return sylvan_not(im1);
}