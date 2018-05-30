#include <sylvan.h>

BDD MYprev(BDD in, BDD relations[], int n_relations, BDD x, BDDMAP map);
BDD checkEU(BDD left, BDD right, BDD relations[], int n_relations, BDD x, BDDMAP map);
BDD checkEG(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map);
BDD checkEX(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map);

// Converters
BDD checkEF(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map);
BDD checkAF(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map);
BDD checkAG(BDD left, BDD relations[], int n_relations, BDD x, BDDMAP map);
