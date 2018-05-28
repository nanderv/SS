#include <sylvan.h>
#include <hashmap.h> 

/* */
void andl_context_init_keys (andl_context_t *andl_context);


/* Returns a BDD array of transitions. Each transition
   represents the fireability of a petrinet transition.
   
   Pass an andl_context with key
*/


BDD petri_get_transitions (andl_context_t *andl_context);


BDD petri_fireable_transition (map_t transitions, char name);

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

