#include <hashmap.h>

#ifndef ANDL_H
#define ANDL_H

/**
 * Stores information while parsing .andl files.
 * Feel free to modify this file as you like.
 */

/**
 * \brief The direction of an arc:
 *  - ARC_IN is a place to transition arc,
 *  - ARC_OUT is a transition to place arc.
 */
typedef enum {
    ARC_IN,
    ARC_OUT,
} arc_dir_t;

/**
 * \brief A struct to store information while parsing
 * an andl file.
 */
typedef struct {
    // the name of the Petri net
    char *name;

    // the name of the current transition being parsed
    char *current_trans;

    // the number of places in the Petri net
    int num_places;

    // the number of transitions in the Petri net
    int num_transitions;

    // the number of place-transition arcs in the Petri net
    int num_in_arcs;

    // the number of transition-place arcs in the Petri net
    int num_out_arcs;

    // whether an error has occured during parsing
    int error;

    map_t transitions;
    map_t places;
    map_t in_arcs;
    map_t out_arcs;
} andl_context_t;

typedef struct {
  char key[8];
  int bddvar;
  char transition_name[512];
  arc_dir_t direction;
} arc_struct_t;

typedef struct {
  char place_name[512];
  int bddvar;
  int marking;
} places_struct_t;

typedef struct {
  char transition_name[512];
  int number;
  int in_arcs[512];
  int out_arcs[512];
  int num_in_arcs;
  int num_out_arcs;
} transitions_struct_t;


#endif
