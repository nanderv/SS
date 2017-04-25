#include <config.h>

#include <stdio.h>

#include <sylvan.h>

#include <andl.h>
#include <andl-lexer.h>
#include <macs2-andl-parser.h>
#include <util.h>

int
load_andl(andl_context_t *andl_context, const char *name)
{
    FILE *f = fopen(name, "r");
    yyscan_t scanner;
    andl_lex_init(&scanner);
    andl_set_in(f, scanner);

    memset(andl_context, 0, sizeof(andl_context_t));

    const int pres = andl_parse(scanner, andl_context);
    andl_lex_destroy(scanner);
    fclose(f);
    return andl_context->error || pres;
}

void
init_sylvan()
{
    int n_workers = 0; // auto-detect
    lace_init(n_workers, 0);
    lace_startup(0, NULL, NULL);

    sylvan_init_package(1LL<<20,1LL<<25,1LL<<20,1LL<<25);
//    sylvan_set_granularity(6); // granularity 6 is decent default value - 1 means "use cache for every operation"
    sylvan_init_bdd();
}

void
deinit_sylvan()
{
    sylvan_stats_report(stderr);
    sylvan_quit();
    lace_exit();
}

void
do_macs2_things(andl_context_t *andl_context)
{
    warn("The name of the Petri net is: %s", andl_context->name);
    warn("There are %d transitions", andl_context->num_transitions);
    warn("There are %d places", andl_context->num_places);
    warn("There are %d in arcs", andl_context->num_in_arcs);
    warn("There are %d out arcs", andl_context->num_out_arcs);
}

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

