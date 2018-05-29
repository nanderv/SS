#include <sylvan.h>

// TODO: Prev function
BDD MYprev(BDD in, BDD startState, BDD relation){
    LACE_ME;

    return in;
}

BDD checkEU(BDD left, BDD right, BDD startState, BDD relation)
{
    LACE_ME;
    BDD z = right;
    BDD old = sylvan_false;
    while ( z != old) {
        old = z;
        z = sylvan_or(z, sylvan_and(left, MYprev(z, startState, relation)));
    }
    return z;
}

BDD checkEG(BDD left, BDD startState, BDD relation)
{
    LACE_ME;
    BDD z = left;
    BDD old = sylvan_false;
    while ( z != old) {
        old = z;
        z = sylvan_and(z, MYprev(z, startState, relation));
    }
    return z;}

BDD checkEX(BDD left, BDD startState, BDD relation)
{
    LACE_ME;
    return MYprev(left, startState, relation);
}

BDD checkEF(BDD left, BDD startState, BDD relation){
 return checkEU(sylvan_true, left, startState, relation);
}

BDD checkAF(BDD left, BDD startState, BDD relation){
    BDD im1 = sylvan_not(left);
    im1 = checkEG(im1, startState, relation);
    return sylvan_not(im1);
}
BDD checkAG(BDD left, BDD startState, BDD relation){
    BDD im1 = sylvan_not(left);
    im1 = checkEF(im1, startState, relation);
    return sylvan_not(im1);
}