#include <string.h>
#include <sylvan.h>

BDD rel_prod(BDD states, BDD relation, BDD x, BDDMAP map) {
  LACE_ME;
  /*
  MTBDDMAP map = sylvan_map_empty();

  map = sylvan_map_add(map, 1, sylvan_ithvar(0) );
  map = sylvan_map_add(map, 3, sylvan_ithvar(2) );
  map = sylvan_map_add(map, 5, sylvan_ithvar(4) );*/
  
  return sylvan_compose( sylvan_exists( sylvan_and( states, relation) , x), map);
}


BDD
reachable_states(BDD initial_states, int n, BDD transitions[], BDDMAP map)
{
  LACE_ME;
  // I: initial states
  // N: number of subtransitions
  // R: subtransitions
  // return V: reachable states

  BDDSET x = sylvan_set_empty();

  
  BDD v_prev = sylvan_false;
  BDD v = initial_states;

  while (v_prev != v){
    v_prev = v;
    for(int i=0; i<n; i++) {
      BDD r_i = transitions[i];
      BDD v = sylvan_or( v, rel_prod(v, r_i, x, map) );
    }
    

  }
  
  
  
  // have a set of reachable states v_old
  // next is the relational product of elements in v_old with transitions applied.
  // keep expanding v_old until you reach a fixpoint

  // 
  // states: a marking of the net. so for every place there is a symbol- set to true if it is marked, or false if it isnt.
  // transitions: possible next markings of the net, given a current marking
  // so there is one for every transition. 
  
}


/* quickly export export a bdd to a dot-file
*/
void export_bdd(BDD bdd, int fnumber) {
  int i = 0;
  char b[256];


  char fnumber_str[12];
  sprintf(fnumber_str, "%d", fnumber);
  
  char fname[12];
  strcpy(fname, "bdd_");
  strcat(fname, fnumber_str);
  strcat(fname, ".dot");
  snprintf(b, 256, fname, i);
  FILE *f = fopen(b, "w+");
  //sylvan_fprintdot(f, bdd);
  mtbdd_fprintdot_nc(f, bdd);
  fclose(f);
}


