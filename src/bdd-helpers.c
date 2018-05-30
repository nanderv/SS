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


BDD reachable_states(BDD initial_states, BDDSET variables, BDD transitions[], int num_transitions) {
  LACE_ME;
  BDD v_prev = sylvan_false;
  BDD v = initial_states;
  sylvan_protect(&v_prev);
  sylvan_protect(&v);
  while (v_prev != v){
    v_prev = v;
    for(int i=0; i<num_transitions; i++) {
      BDD transition = transitions[i];
      BDD next_state = sylvan_relnext(v, transition, variables);
      v = sylvan_or( v, next_state);
    }
  }
  sylvan_unprotect(&v_prev);
  sylvan_unprotect(&v);
  return v;
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


