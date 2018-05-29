#include <sylvan.h>
#include <hashmap.h> 

/* */
void petri_sort_arcs (map_t transitions);

/* Returns a BDD array of transitions. Each transition
   represents the fireability of a petrinet transition.
   
   Pass an andl_context with key
*/


BDD petri_get_transitions (andl_context_t *andl_context);

/* Constructs a BDD that expresses the firability of some petri transition with
   a certain name. 
   Precondition for a fireable transition: all in-arcs are marked, no out-arcs is marked.
   Postcondition for a fireable transition: no in-arcs are marked, all out-arcs are marked. All nodes without an arc in this transition should have equal marking after the transition.

   @param transitions: hashmap containing transtions with in and out arcs
   @param name: name of the key in the transtion hashmap
   @param num_variables: number of variables in the bdd. 
 */
BDD petri_fireable_transition (map_t transitions, char* name, int num_variables);

BDD petri_get_marking (map_t places);



//int petri_get_marking (BDD* bdd, places_struct_t* i);
//int petri_get_transitions (BDD* r[], transitions_struct_t* i);

                            

/* debug function that prints placenames and associated bddvars to stdout
   usage: hashmap_iterate(transition_map, *list_transitions, (void**)(&item))
*/
void petri_list_places(map_t places);

/* debug function that prints transitionnames and number of in and out arcs to stdout.
   usage: hashmap_iterate(transition_map, *list_transitions, (void**)(&item))
*/
void petri_list_transitions(map_t transitions);

