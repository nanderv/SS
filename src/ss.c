#include <config.h>

#include <stdio.h>

#include <sylvan.h>

#include <andl.h>
#include <andl-lexer.h>
#include <ss-andl-parser.h>
#include <util.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <hashmap.h>

#include <petri-helpers.h>
#include <bdd-helpers.h>

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
        // initialize the lexer
        yyscan_t scanner;
        andl_lex_init(&scanner);
        // make the lexer read the file f
        andl_set_in(f, scanner);

        // zero the andl_context
        memset(andl_context, 0, sizeof(andl_context_t));


        // initialize the andl_context
        andl_context->transitions = hashmap_new();
        andl_context->places = hashmap_new();
        // mmalloc?
        
        // parse the andl file
        const int pres = andl_parse(scanner, andl_context);

        // destroy the lexer
        andl_lex_destroy(scanner);
        fclose(f);
        res = andl_context->error || pres;

        petri_sort_arcs(andl_context->transitions);
        petri_list_places(andl_context->places);
        petri_list_transitions(andl_context->transitions);

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

    /* initialize the node table and cache with minimum size 2^20 entries, and
     * maximum 2^25 entries */
    sylvan_init_package(1LL<<20,1LL<<25,1LL<<20,1LL<<25);

    // initialize Sylvan's BDD sub system
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
 * Here you should implement whatever is required for the Software Science lab class.
 * \p andl_context: The user context that is used while parsing
 * the andl file.
 * The default implementation right now, is to print several
 * statistics of the parsed Petri net.
 */
void
do_ss_things(andl_context_t *andl_context)
{
    warn("The name of the Petri net is: %s", andl_context->name);
    warn("There are %d transitions", andl_context->num_transitions);
    warn("There are %d places", andl_context->num_places);
    warn("There are %d in arcs", andl_context->num_in_arcs);
    warn("There are %d out arcs", andl_context->num_out_arcs);

    BDD bdd = sylvan_true;
    sylvan_protect(&bdd);
    BDDSET set = sylvan_set_empty();
    sylvan_protect(&set);
    BDDMAP map = sylvan_map_empty();
    sylvan_protect(&map);

    BDD initial_marking = petri_get_marking(andl_context->places);
    sylvan_protect(&initial_marking);

    export_bdd(initial_marking, 0);

    BDD transitions[andl_context->num_transitions];

    for(int i = 0; i < andl_context->num_transitions; i++) {
      char key[512];
      transitions_struct_t* value = malloc(sizeof(transitions_struct_t));
      //      sprintf(key, 512, "%i", i);
      //      hashmap_get(andl_context->transitions, key, (void**)(&value));
      //      sprintf(stdout, 512, "%s\n", value->transition_name);
    }
    //    hashmap_iterate(andl_context->places, *get_transitions, (void**)(&transitions));



    
    //    printf("transitions=%i\n", transitions);
    
    // build the map used for substitions in the sylvan_compose function
    // remember that all prime variables are at i+1
    for(int i = 0; i < andl_context->num_places; i++) {
      map = sylvan_map_add(map, i+1, sylvan_ithvar(i));
    }

    
    
    //        places_struct_t* item; // perhaps this is something that the list_places can use to remember stuff
    //  item = malloc(sizeof(places_struct_t));


    
    // do stuff
    sylvan_unprotect(&bdd);
    sylvan_unprotect(&set);
    sylvan_unprotect(&map);
}

/**
 * \brief An in-order parser of the given XML node.
 *
 * The default implementation is to print the temporal logic formula
 * on stderr.
 */
static int
parse_formula(xmlNode *node)
{
    int res = 0;
    // first check if the node is not a NULL pointer.
    if (node == NULL) {
        res = 1;
        warn("Invalid XML");
    // only parse xml nodes, skip other parts of the XML file.
    } else if (node->type != XML_ELEMENT_NODE) res = parse_formula(xmlNextElementSibling(node));
    // parse forAll
    else if (xmlStrcmp(node->name, (const xmlChar*) "all-paths") == 0) {
        fprintf(stderr, "A ");
        res = parse_formula(xmlFirstElementChild(node));
    // parse Exists
    } else if (xmlStrcmp(node->name, (const xmlChar*) "exists-path") == 0) {
        fprintf(stderr, "E ");
        res = parse_formula(xmlFirstElementChild(node));
    // parse Globally
    } else if (xmlStrcmp(node->name, (const xmlChar*) "globally") == 0) {
        fprintf(stderr, "G ");
        res = parse_formula(xmlFirstElementChild(node));
    // parse Finally
    } else if (xmlStrcmp(node->name, (const xmlChar*) "finally") == 0) {
        fprintf(stderr, "F ");
        res = parse_formula(xmlFirstElementChild(node));
    // parse neXt
    } else if (xmlStrcmp(node->name, (const xmlChar*) "next") == 0) {
        fprintf(stderr, "X ");
        res = parse_formula(xmlFirstElementChild(node));
    // parse Until
    } else if (xmlStrcmp(node->name, (const xmlChar*) "until") == 0) {
        fprintf(stderr, "(");
        res = parse_formula(xmlFirstElementChild(node));
        fprintf(stderr, ") U (");
        res |= parse_formula(xmlNextElementSibling(xmlFirstElementChild(node)));
        fprintf(stderr, ")");
    // parse before
    } else if (xmlStrcmp(node->name, (const xmlChar*) "before") == 0) {
        res = parse_formula(xmlFirstElementChild(node));
    // parse reach
    } else if (xmlStrcmp(node->name, (const xmlChar*) "reach") == 0) {
        res = parse_formula(xmlFirstElementChild(node));
    // parse negation
    } else if (xmlStrcmp(node->name, (const xmlChar*) "negation") == 0) {
        fprintf(stderr, "!(");
        res = parse_formula(xmlFirstElementChild(node));
        fprintf(stderr, ")");
    // parse conjunction
    } else if (xmlStrcmp(node->name, (const xmlChar*) "conjunction") == 0) {
        fprintf(stderr, "(");
        res = parse_formula(xmlFirstElementChild(node));
        fprintf(stderr, ") && (");
        res |= parse_formula(xmlNextElementSibling(xmlFirstElementChild(node)));
        fprintf(stderr, ")");
    // parse disjunction
    } else if (xmlStrcmp(node->name, (const xmlChar*) "disjunction") == 0) {
        fprintf(stderr, "(");
        res = parse_formula(xmlFirstElementChild(node));
        fprintf(stderr, ") || (");
        res |= parse_formula(xmlNextElementSibling(xmlFirstElementChild(node)));
        fprintf(stderr, ")");
    // parse is-fireable: atomic predicate!
    } else if (xmlStrcmp(node->name, (const xmlChar*) "is-fireable") == 0) {
        fprintf(stderr, "is-fireable(");
        res = parse_formula(xmlFirstElementChild(node));
        fprintf(stderr, ")");
    // parse transition (part of the atomic predicate)
    } else if (xmlStrcmp(node->name, (const xmlChar*) "transition") == 0) {
        for (xmlNode *transition = node; transition != NULL;
                transition = xmlNextElementSibling(transition)) {
            fprintf(stderr, "%s,", xmlNodeGetContent(transition));
        }
    } else {
        res = 1;
        warn("Invalid xml node '%s'", node->name);
    }
    return res;
}

/**
* CTL CHECKERS
*/

BDD* checkEU(BDD* left, BDD* right, BDD* model)
{
    return sylvan_true;
}

BDD* checkEG(BDD* left, BDD* model)
{
    return sylvan_true;
}

BDD* checkEX(BDD* left, BDD* model)
{
    return sylvan_true;
}

BDD* checkEF(BDD* left, BDD* model){
 return checkEU(sylvan_true, left, model);
}


/**
* Recursive descent for formula parsing, thus validating the formula.
*/
BDD*
parse_formula_BU(xmlNode *node, map_t *transitions, int isAll, BDD* model)
{
      printf("HERE");

  LACE_ME;

    // first check if the node is not a NULL pointer.
    if (node == NULL) {       
        warn("Invalid XML");
    // only parse xml nodes, skip other parts of the XML file.
    } else if (node->type != XML_ELEMENT_NODE) return parse_formula_BU(xmlNextElementSibling(node), transitions,0, model);
    // parse forAll
    else if (xmlStrcmp(node->name, (const xmlChar*) "all-paths") == 0) {
        BDD* res = parse_formula_BU(xmlFirstElementChild(node), transitions, 1, model);
    // parse Exists
    } else if (xmlStrcmp(node->name, (const xmlChar*) "exists-path") == 0) {
        BDD* res = parse_formula_BU(xmlFirstElementChild(node), transitions, 0, model);
    // parse Globally
    } else if (xmlStrcmp(node->name, (const xmlChar*) "globally") == 0) {
        if (!isAll) // EG
        {
            BDD* res = checkEG(parse_formula_BU(xmlFirstElementChild(node), transitions,0, model), model);
        } else { // AG
            BDD* im1 = sylvan_not(parse_formula_BU(xmlFirstElementChild(node), transitions,0, model));
            im1 = checkEF(im1, model);
            return sylvan_not(im1);
        }
    // parse Finally
    } else if (xmlStrcmp(node->name, (const xmlChar*) "finally") == 0) {
        if (!isAll) // EF
        {
            return checkEF(parse_formula_BU(xmlFirstElementChild(node), transitions, 0, model), model);
        } else { // AF
            BDD* im1 = sylvan_not(parse_formula_BU(xmlFirstElementChild(node), transitions,0, model));
            im1 = checkEG(im1, model);
            return sylvan_not(im1);
        }
    // parse neXt
    } else if (xmlStrcmp(node->name, (const xmlChar*) "next") == 0) {
        return checkEX(parse_formula_BU(xmlFirstElementChild(node), transitions,0, model), model);
    // parse Until
    } else if (xmlStrcmp(node->name, (const xmlChar*) "until") == 0) {
        BDD* res = parse_formula_BU(xmlFirstElementChild(node), transitions,0, model);
        BDD* res2 = parse_formula_BU(xmlNextElementSibling(xmlFirstElementChild(node)), transitions,0, model);
        return checkEU(res, res2, model);
    // parse before
    } else if (xmlStrcmp(node->name, (const xmlChar*) "before") == 0) {
        return parse_formula_BU(xmlFirstElementChild(node), transitions,0, model);
    // parse reach
    } else if (xmlStrcmp(node->name, (const xmlChar*) "reach") == 0) {
        return  parse_formula_BU(xmlFirstElementChild(node), transitions,0, model);
    // parse negation
    } else if (xmlStrcmp(node->name, (const xmlChar*) "negation") == 0) {
        return sylvan_not (parse_formula_BU(xmlFirstElementChild(node), transitions,0, model));
        fprintf(stderr, ")");
    // parse conjunction
    } else if (xmlStrcmp(node->name, (const xmlChar*) "conjunction") == 0) {
        BDD* res = parse_formula_BU(xmlFirstElementChild(node),transitions,0, model);
        BDD* res2 = parse_formula_BU(xmlNextElementSibling(xmlFirstElementChild(node)), transitions,0, model);
        return sylvan_and(res, res2);
    // parse disjunction
    } else if (xmlStrcmp(node->name, (const xmlChar*) "disjunction") == 0) {
        BDD* res = parse_formula_BU(xmlFirstElementChild(node), transitions,0, model);
        BDD* res2 = parse_formula_BU(xmlNextElementSibling(xmlFirstElementChild(node)), transitions,0, model);
        return sylvan_or(res, res2);
    // parse is-fireable: atomic predicate!
    } else if (xmlStrcmp(node->name, (const xmlChar*) "is-fireable") == 0) {
        return parse_formula_BU(xmlFirstElementChild(node), transitions,0, model);
    // parse transition (part of the atomic predicate)
    } else if (xmlStrcmp(node->name, (const xmlChar*) "transition") == 0) {
        BDD* res = sylvan_false;
        for (xmlNode *transition = node; transition != NULL;
                transition = xmlNextElementSibling(transition)) {
            transitions_struct_t transitionD;
            hashmap_get(transitions, xmlNodeGetContent(transition), (void**) (&transitionD));
            BDD* intermediate = sylvan_true;
            for (int i=0;i<transitionD.num_in_arcs; i++) {
                intermediate = sylvan_and(intermediate, sylvan_ithvar(transitionD.in_arcs[i]));
            }
            for (int i=0;i<transitionD.num_out_arcs; i++) {
                intermediate = sylvan_and(intermediate, sylvan_nithvar(transitionD.out_arcs[i]));
            }
            res = sylvan_or(res, intermediate);
            fprintf(stderr, "%s,", xmlNodeGetContent(transition));
        }
        return res;
    } else {
        warn("Invalid xml node '%s'", node->name);
    }
    return sylvan_true;
}


/**
 * \brief recursively parse the given XML node.
 */
static int
parse_xml(xmlNode *node, map_t *transitions, int isAll, BDD* model)
{
    int res = 0;
    // first check if the node is not a NULL pointer.
    if (node == NULL) {
        res = 1;
        warn("Invalid XML");
    // only parse xml nodes, skip other parts of the XML file.
    } else if (node->type != XML_ELEMENT_NODE) res = parse_xml(xmlNextElementSibling(node), transitions,  isAll,  model);
    // parse property-set
    else if (xmlStrcmp(node->name, (const xmlChar*) "property-set") == 0) {
        // loop over all children that are property nodes
        for (xmlNode *property = xmlFirstElementChild(node);
                property != NULL && !res;
                property = xmlNextElementSibling(property)) {
            res = parse_xml(property, transitions,  isAll,  model);
        }
    // parse property
    } else if (xmlStrcmp(node->name, (const xmlChar*) "property") == 0) {
        warn("parsing property");
        res = parse_xml(xmlFirstElementChild(node), transitions,  isAll,  model);
    // parse id of property
    } else if (xmlStrcmp(node->name, (const xmlChar*) "id") == 0) {
        warn("Property id is: %s", xmlNodeGetContent(node));
        res = parse_xml(xmlNextElementSibling(node), transitions,  isAll,  model);
    // parse description of property
    } else if (xmlStrcmp(node->name, (const xmlChar*) "description") == 0) {
        warn("Property description is: %s", xmlNodeGetContent(node));
        res = parse_xml(xmlNextElementSibling(node), transitions,  isAll,  model);
    // parse the formula
    } else if (xmlStrcmp(node->name, (const xmlChar*) "formula") == 0) {
        warn("Parsing formula...");
        parse_formula(xmlFirstElementChild(node));
        parse_formula_BU(xmlFirstElementChild(node), transitions,  isAll,  model);
        res = 1;
        printf("\n");
    // node not recognized
    } else {
        res = 1;
        warn("Invalid xml node '%s'", node->name);
    }

    return res;
}

/**
 * \brief parses the XML file name.
 *
 * \returns 0 on success, 1 on failure.
 */
static int
load_xml(const char* name, map_t *transitions, int isAll, BDD* model)
{
    int res;

    LIBXML_TEST_VERSION
    warn("parsing formulas file: %s", name);
    xmlDoc *doc = xmlReadFile(name, NULL, 0);
    if (doc == NULL) res = 1;
    else {
        xmlNode *node = xmlDocGetRootElement(doc);
        res = parse_xml(node, transitions, isAll, model);
    }

    return res;
}

/**
 * \brief main. First parse the .andl file is parsed. And optionally parse the
 * XML file next.
 *
 * \returns 0 on success, 1 on failure.
 */
int main(int argc, char** argv)
{
    int res;
    if (argc >= 2) {
        andl_context_t andl_context;
        const char *name = argv[1];
        res = load_andl(&andl_context, name);
        if (res) warn("Unable to parse file '%s'", name);
        else {
            warn("Successful parse of file '%s' :)", name);
            if (argc == 3) {
                const char *formulas = argv[2];
                res = load_xml(formulas, andl_context.transitions, 0, sylvan_true);
                if (res) warn("Unable to load xml '%s'", formulas);
            }
            init_sylvan();
            // execute the main body of code
            do_ss_things(&andl_context);
            deinit_sylvan();
        }
    } else {
        warn("Usage: %s <petri-net>.andl [<CTL-formulas>.xml]", argv[0]);
        res = 1;
    }

    return res;
}

