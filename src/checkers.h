#include <sylvan.h>

// TODO: Prev function
BDD MYprev(BDD in, BDD startState, BDD relation);
BDD checkEU(BDD left, BDD right, BDD startState, BDD relation);
BDD checkEG(BDD left, BDD startState, BDD relation);
BDD checkEX(BDD left, BDD startState, BDD relation);

// Converters
BDD checkEF(BDD left, BDD startState, BDD relation);
BDD checkAF(BDD left, BDD startState, BDD relation);
BDD checkAG(BDD left, BDD startState, BDD relation);
