#ifndef ANDL_H
#define ANDL_H

typedef enum {
    ARC_IN,
    ARC_OUT,
} arc_dir_t;

typedef struct {
    char *name;
    char *current_trans;
    int num_places;
    int num_transitions;
    int num_in_arcs;
    int num_out_arcs;
    int error;
} andl_context_t;

#endif
