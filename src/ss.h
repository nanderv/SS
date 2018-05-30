#ifndef UTIL_H
#define UTIL_H
/**
 * \brief prints a message on stderr, and ends it with a new line
 */
int
load_xml(const char* name, andl_context_t *transitions, int isAll, BDD relations[], int n_relations, BDD x, BDDMAP map, BDD initial_marking) ;

#endif
