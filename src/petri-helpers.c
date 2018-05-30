#include <string.h>
#include <sylvan.h>
#include <hashmap.h> 
#include  <andl.h>

int compare_arcs_fn (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

int petri_sort_arcs_fn(char* item, transitions_struct_t* transition) {
  qsort(transition->in_arcs, transition->num_in_arcs, sizeof(int), compare_arcs_fn);
  qsort(transition->out_arcs, transition->num_out_arcs, sizeof(int), compare_arcs_fn);
  return MAP_OK;
}

void petri_sort_arcs (map_t transitions) {
  void* item;
  item = malloc(sizeof(void));
  hashmap_iterate(transitions, *petri_sort_arcs_fn, (void**)(&item));
}

int place_key_lookup () {
  
  return MAP_OK;
}
BDD petri_fireable_transition (map_t transitions, char* name, int num_variables) {
  LACE_ME;
  BDD bdd = sylvan_true;
  sylvan_protect(&bdd);

  transitions_struct_t* transition;
  transition = malloc(sizeof(transitions_struct_t));

  
  int error = hashmap_get(transitions, name, (void**)(&transition));
  /* We assume sorted arcs! Sort the arcs before running this function */

  int n = 0;
  int m = 0;
  
  for(int i = 0; i < num_variables; i++) { //i=i+2: only go over non-prime vars.
    int p = i*2;
    if( transition->in_arcs[n] == p ) {
      BDD x_i = sylvan_ithvar(p);
      BDD x_i_prime = sylvan_nithvar(p+1);
      bdd = sylvan_and(bdd, x_i);
      bdd = sylvan_and(bdd, x_i_prime);
      // i is an in arc of the transition
      // so i should be marked currently
      // and it's prime variable should not be marked
      n++;
    }
    else if( transition->out_arcs[m] == p ) {
      BDD x_i = sylvan_nithvar(p);
      BDD x_i_prime = sylvan_ithvar(p+1);
      bdd = sylvan_and(bdd, x_i);
      bdd = sylvan_and(bdd, x_i_prime);
      // i is an out arc of the transition
      // so i-prime should e marked
      m++;
    }
    else {
      BDD x_i = sylvan_ithvar(p);
      BDD x_i_prime = sylvan_ithvar(p+1);
      bdd = sylvan_and(bdd, sylvan_equiv(x_i, x_i_prime));
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
