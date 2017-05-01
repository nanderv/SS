#include <config.h>

#include <stdio.h>

#include <sylvan.h>

#include <andl.h>
#include <andl-lexer.h>
#include <macs2-andl-parser.h>
#include <util.h>

/**
 * Load the andl file in \p name.
 * \p andl_context: The user context available when paring the andl file.
 * \p name: the name of the andl file to parse.
 * \return: 0 on success, 1 on failure.
 */
int
load_andl(andl_context_t *andl_context, const char *name)
{
    int res;
    FILE *f = fopen(name, "r");
    if (f == NULL) {
        warn("Could not open file '%s'", name);
        res = 1;
    } else {
        yyscan_t scanner;
        andl_lex_init(&scanner);
        andl_set_in(f, scanner);

        memset(andl_context, 0, sizeof(andl_context_t));

        const int pres = andl_parse(scanner, andl_context);
        andl_lex_destroy(scanner);
        fclose(f);
        res = andl_context->error || pres;
    }

    return res;
}

/**
 * Initializes Sylvan. The number of lace workers will be automatically
 * detected. The size of the node table, and cache are set to sensible
 * defaults. We initialize the BDD package (not LDD, or MTBDD).
 */
void
init_sylvan()
{
    int n_workers = 0; // auto-detect
    lace_init(n_workers, 0);
    lace_startup(0, NULL, NULL);

    sylvan_init_package(1LL<<20,1LL<<25,1LL<<20,1LL<<25);
    sylvan_init_bdd();
}

/**
 * Deinialize Sylvan. If Sylvan is compiled with 
 * -DSYLVAN_STATS=ON, then statistics will be print,
 * such as the number of nodes in the node table.
 */
void
deinit_sylvan()
{
    sylvan_stats_report(stderr);
    sylvan_quit();
    lace_exit();
}

/**
 * Here you should implement whatever is required for the MACS2 lab class.
 * \p andl_context: The user context that is used while parsing
 * the andl file. 
 * The default implementation right now, is to print several
 * statistics of the parsed Petri net.
 */
void
do_macs2_things(andl_context_t *andl_context)
{
    warn("The name of the Petri net is: %s", andl_context->name);
    warn("There are %d transitions", andl_context->num_transitions);
    warn("There are %d places", andl_context->num_places);
    warn("There are %d in arcs", andl_context->num_in_arcs);
    warn("There are %d out arcs", andl_context->num_out_arcs);
}

/**
 * \brief main.
 *
 * \returns 0 on success, 1 on failure.
 */
int main(int argc, char** argv)
{
    int res;
    if (argc == 2) {
        andl_context_t andl_context; 
        const char *name = argv[1];
        res = load_andl(&andl_context, name);
        if (res) warn("Unable to parse file '%s'", name);
        else {
            warn("Successful parse of file '%s' :)", name);
            init_sylvan();
            do_macs2_things(&andl_context);
            deinit_sylvan();
        }
    } else {
        warn("Usage: %s <petri-net>.andl", argv[0]);
        res = 1;
    }

    return res;
}

