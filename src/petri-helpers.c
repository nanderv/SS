#include <string.h>
#include <sylvan.h>
#include <hashmap.h> 
#include  <andl.h>

/*
BDD transition_in_arcs()
{
  return sylvan_false;
  }
*/
void andl_context_init_keys (andl_context_t *andl_context) {

  

}

int place_key_lookup () {
  
  return MAP_OK;
}

int transition_key_lookup () {
  
  return MAP_OK;
}


BDD petri_fireable_transition (andl_context_t *andl_context, char name) {
  BDD bdd = sylvan_false;
  sylvan_protect(&bdd);

  transitions_struct_t* transition;
  transition = malloc(sizeof(transitions_struct_t));
  hashmap_get(andl_context->transitions, name, (void**)(&transition));

  int in_arcs[transition->num_in_arcs];
  int out_arcs[transition->num_out_arcs];
  // sort in and out arcs
  // do some qsort
  int n = 0;
  int m = 0;

  for(int i = 0; i < andl_context->num_places; i++) {
    if( in_arcs[n] == i ) {
      // i is an in arc of the transition
      // so i should be marked currently
      n++;
    }
    else if( out_arcs[m] == i ) {
      // i is an out arc of the transition
      // so i-prime should not be unmarked
      m++;
    }
    else {
    // i is not an arc in the transition
    // so i-prime should equal i
    }
  }
  
  sylvan_unprotect(&bdd);
  return bdd;
}

int get_marking_fn (BDD* bdd, places_struct_t* i) {
  LACE_ME;
  BDD var;
  sylvan_protect(&var);
  if(i->marking==1) {
     var = sylvan_ithvar(i->bddvar);
     sylvan_protect(&var);    
  }
  else {
    var = sylvan_ithvar(i->bddvar);
    var = sylvan_not(var);
  }
  *bdd = sylvan_and(*bdd, var);

  sylvan_unprotect(&var);
  return MAP_OK;
}


BDD petri_get_marking (map_t places) {
  BDD marking = sylvan_true;
  sylvan_protect(&marking);
  hashmap_iterate(places, *get_marking_fn, (void**)(&marking));
  sylvan_unprotect(&marking);
  return marking;
}

BDD petri_get_transitions (andl_context_t *andl_context)
{

}



int petri_get_transitions_fn (BDD* r[], transitions_struct_t* i)
{
  LACE_ME;
  BDD t = sylvan_false;

  
  return MAP_OK;
}


/*
places_struct_t* item; // perhaps this is something that the list_places can use to remember stuf
        item = malloc(sizeof(places_struct_t));
        hashmap_iterate(andl_context->places, *list_places, (void**)(&item));
        hashmap_iterate(andl_context->transitions, *list_transitions, (void**)(&item));
*/

int petri_list_places_fn(char* item, places_struct_t* i)
{
  printf("%s@bddvar(%i): %i\n", i->place_name, i->bddvar, i->marking);
  return MAP_OK;
}

/* debug function that prints transitionnames and number of in and out arcs to stdout.
   usage: hashmap_iterate(transition_map, *list_transitions, (void**)(&item))
*/
int petri_list_transitions_fn(char* item, transitions_struct_t* t)
{
  printf("%i->%s->%i\n", t->num_in_arcs, t->transition_name, t->num_out_arcs);
  for(int i=0; i<t->num_in_arcs;i++) {
    int bddvar = t->in_arcs[i];
  }
    for(int i=0; i<t->num_out_arcs;i++) {
    int bddvar = t->out_arcs[i];
  }
  return MAP_OK;
}


void petri_list_places(map_t places) {
  void* item;
  item = malloc(sizeof(void));
  hashmap_iterate(places, *petri_list_places_fn, (void**)(&item));
}
void petri_list_transitions(map_t transitions) {
  void* item;
  item = malloc(sizeof(void));
  hashmap_iterate(transitions, *petri_list_transitions_fn, (void**)(&item));

}
