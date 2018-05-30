#include <sylvan.h>

// Prev function
BDD MYprev(BDD in, BDD relations[], int n_relations, BDD x, BDDMAP map) {
    LACE_ME;
    sylvan_protect(&in);
    sylvan_protect(&x);
    sylvan_protect(&map);
    BDD result = sylvan_false;
    sylvan_protect(&result);

    // Apply all relations
    for(int i=0; i < n_relations; i++) {
        BDD relation = relations[i];
        // execute relation check; simply try running relations in reverse.

        BDD relation_result  =  sylvan_relprev(relation, in, x);
        sylvan_protect(&relation_result);
        result = sylvan_or(result, relation_result);
        sylvan_unprotect(&relation_result);
    }
    sylvan_unprotect(&result);
    sylvan_unprotect(&x);
    sylvan_unprotect(&map);
    sylvan_unprotect(&in);
    return result;
}

BDD checkEU(BDD left, BDD right, BDD relations[], int n_relations, BDD x, BDDMAP map) 
{
    LACE_ME;

    BDD z = right;
    sylvan_protect(&left);
    sylvan_protect(&right);
    sylvan_protect(&z);

    BDD old = sylvan_false;
    sylvan_protect(&old);

    while ( z != old) {
        old = z;
            z = sylvan_or(z, sylvan_and(left, MYprev(z, relations, n_relations, x, map)));
    }

    sylvan_unprotect(&left);
    sylvan_unprotect(&right);
    sylvan_unprotect(&z);
    sylvan_unprotect(&old);
    return z;
}

BDD checkEG(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) 
{
    LACE_ME;
    BDD z = left;
    BDD old = sylvan_false;
    sylvan_protect(&left);
    sylvan_protect(&z);
    sylvan_protect(&old);
    while ( z != old) {
        old = z;
        z = sylvan_and(z, MYprev(z, relations, n_relations,  x, map ));
    }

    sylvan_unprotect(&left);
    sylvan_unprotect(&z);
    sylvan_unprotect(&old);
    return z;
}

BDD checkEX(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) 
{
    LACE_ME;
    return MYprev(left, relations, n_relations,  x, map );
}

BDD checkEF(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) {
    return checkEU(sylvan_true, left, relations, n_relations,  x, map );
}

BDD checkAF(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) {
    sylvan_protect(&left);

    BDD im1 = sylvan_not(left);
    sylvan_protect(&im1);

    im1 = checkEG(im1, relations, n_relations,  x, map );
    sylvan_unprotect(&im1);
    sylvan_unprotect(&left);

    return sylvan_not(im1);
}

BDD checkAG(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map) {
    sylvan_protect(&left);

    BDD im1 = sylvan_not(left);
    sylvan_protect(&im1);

    im1 = checkEF(im1, relations, n_relations,  x, map );
    
    sylvan_unprotect(&im1);
    sylvan_unprotect(&left);
    return sylvan_not(im1);
}