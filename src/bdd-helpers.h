#include <sylvan.h>

/* quickly export export a bdd to a dot-file
*/
void export_bdd(BDD bdd, int fnumber);
BDD rel_prod(BDD states, BDD relation, BDD x, BDDMAP map);