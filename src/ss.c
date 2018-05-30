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
#include <checkers.h>
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
    lace_init(n_workers, 40960000);
    lace_startup(0, NULL, NULL);

    /* initialize the node table and cache with minimum size 2^20 entries, and
     * maximum 2^25 entries */
    sylvan_init_package(1LL<<20,1LL<<25,1LL<<20,1LL<<25);

    // initialize Sylvan's BDD sub system
    sylvan_init_bdd();
    //sylvan_gc_disable();
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
do_ss_things(andl_context_t *andl_context,int argc, char** argv)
{
    LACE_ME;
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

    /*    char names[andl_context->num_places][512];
          names = petri_transition_names(andl_context->transitions, andl_context->num_places);*/
    BDD transitions[andl_context->num_transitions];
    
    for(int i = 0; i<andl_context->num_transitions; i++){
      transitions[i] = petri_fireable_transition(andl_context->transitions, andl_context->transition_names[i], andl_context->num_places);
      sylvan_protect(&transitions[i]);

    }


    for(int i = 0; i < andl_context->num_places; i++) {
      BDDVAR elem = i*2;
      set = sylvan_set_add(set, elem);
    }


    BDD v_prev = sylvan_false;
    BDD v = initial_marking;
    
    while (v_prev != v){
      v_prev = v;
      for(int i=0; i<andl_context->num_transitions; i++) {
        BDD r_i = transitions[i];
        //        BDD next_state = rel_prod(v, r_i, set, map);
        BDD next_state = sylvan_relnext(v, r_i, set);
        v = sylvan_or(v, next_state);
        export_bdd(v, i);
        printf("%i\t\%i\t%i\n", v, v_prev, v_prev!=v);

      }
    }

    int satcount = sylvan_satcount(v, set);
    printf("satcount: %i\n", satcount);

    
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
    const char *name = argv[1];
    
    warn("Successful parse of file '%s' :)", name);
    if (argc == 3) {
        const char *formulas = argv[2]; // sylvan_true's: initial state, relations
        int res = load_xml(formulas, andl_context, 0, transitions, andl_context->num_transitions, set, initial_marking);
        if (res) warn("Unable to load xml '%s'", formulas);
        }

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
* Recursive descent for formula parsing, thus validating the formula.
*/
BDD
parse_formula_BU(xmlNode *node, andl_context_t *andl_context, int isAll, BDD relations[], int n_relations, BDD x, BDDMAP map) 
{
    LACE_ME;
    warn("HERE");
    // first check if the node is not a NULL pointer.
    if (node == NULL) {       
        warn("Invalid XML");
    // only parse xml nodes, skip other parts of the XML file.
    } else if (node->type != XML_ELEMENT_NODE) return parse_formula_BU(xmlNextElementSibling(node), andl_context,0, relations, n_relations,  x, map );
    // parse forAll
    else if (xmlStrcmp(node->name, (const xmlChar*) "all-paths") == 0) {
        return parse_formula_BU(xmlFirstElementChild(node), andl_context, 1, relations, n_relations,  x, map );
    // parse Exists
    } else if (xmlStrcmp(node->name, (const xmlChar*) "exists-path") == 0) {
        return parse_formula_BU(xmlFirstElementChild(node), andl_context, 0, relations, n_relations,  x, map );
    // parse Globally
    } else if (xmlStrcmp(node->name, (const xmlChar*) "globally") == 0) {
        if (!isAll) // EG
        {
            return checkEG(parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map ), relations, n_relations,  x, map );
        } else { // AG
            return checkAG(parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map ), relations, n_relations,  x, map );

        }
    // parse Finally
    } else if (xmlStrcmp(node->name, (const xmlChar*) "finally") == 0) {
        if (!isAll) // EF
        {
            return checkEF(parse_formula_BU(xmlFirstElementChild(node), andl_context, 0, relations, n_relations,  x, map ), relations, n_relations,  x, map );
        } else { // AF
            return checkAF(parse_formula_BU(xmlFirstElementChild(node), andl_context, 0, relations, n_relations,  x, map ), relations, n_relations,  x, map );

        }
    // parse neXt
    } else if (xmlStrcmp(node->name, (const xmlChar*) "next") == 0) {
        return checkEX(parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map ), relations, n_relations,  x, map );
    // parse Until
    } else if (xmlStrcmp(node->name, (const xmlChar*) "until") == 0) {
        BDD res = parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map );
        BDD res2 = parse_formula_BU(xmlNextElementSibling(xmlFirstElementChild(node)), andl_context,0, relations, n_relations,  x, map );

        BDD r = checkEU(res, res2, relations, n_relations,  x, map );

        return r;
    // parse before
    } else if (xmlStrcmp(node->name, (const xmlChar*) "before") == 0) {
        return parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map );
    // parse reach
    } else if (xmlStrcmp(node->name, (const xmlChar*) "reach") == 0) {
        return  parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map );
    // parse negation
    } else if (xmlStrcmp(node->name, (const xmlChar*) "negation") == 0) {
        return sylvan_not (parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map ));
        fprintf(stderr, ")");
    // parse conjunction
    } else if (xmlStrcmp(node->name, (const xmlChar*) "conjunction") == 0) {
        BDD res = parse_formula_BU(xmlFirstElementChild(node),andl_context,0, relations, n_relations,  x, map );
        sylvan_protect(&res);
        BDD res2 = parse_formula_BU(xmlNextElementSibling(xmlFirstElementChild(node)), andl_context,0, relations, n_relations,  x, map );
        sylvan_protect(&res2);
        BDD fin =  sylvan_and(res, res2);
        sylvan_unprotect(&res);
        sylvan_unprotect(&res2);
        return fin;
    // parse disjunction
    } else if (xmlStrcmp(node->name, (const xmlChar*) "disjunction") == 0) {
        BDD res = parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map );
        sylvan_protect(&res);
        
        BDD res2 = parse_formula_BU(xmlNextElementSibling(xmlFirstElementChild(node)), andl_context,0, relations, n_relations,  x, map );
        sylvan_protect(&res2);

        BDD fin =  sylvan_or(res, res2);
        sylvan_unprotect(&res);
        sylvan_unprotect(&res2);
        return fin;
    // parse is-fireable: atomic predicate!
    } else if (xmlStrcmp(node->name, (const xmlChar*) "is-fireable") == 0) {
        return parse_formula_BU(xmlFirstElementChild(node), andl_context,0, relations, n_relations,  x, map );
    // parse transition (part of the atomic predicate)
    } else if (xmlStrcmp(node->name, (const xmlChar*) "transition") == 0) {
        BDD res = sylvan_false;
        sylvan_protect(&res );

        for (xmlNode *transition = node; transition != NULL;
                transition = xmlNextElementSibling(transition)) {
            warn(xmlNodeGetContent(transition));

            BDD intermediate = petri_fireable_transition (andl_context->transitions, xmlNodeGetContent(transition), andl_context->num_places);
                    sylvan_protect(&intermediate);
            res = sylvan_or(res, intermediate);
            sylvan_unprotect(&intermediate);
            fprintf(stderr, "%s,", xmlNodeGetContent(transition));
        }
        sylvan_unprotect(&res);
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
parse_xml(xmlNode *node, andl_context_t *transitions, int isAll, BDD relations[], int n_relations, BDD x, BDDMAP map, BDD initial_marking) 
{
    LACE_ME;
    int res = 0;
    // first check if the node is not a NULL pointer.
    if (node == NULL) {
        res = 1;
        warn("Invalid XML");
    // only parse xml nodes, skip other parts of the XML file.
    } else if (node->type != XML_ELEMENT_NODE) res = parse_xml(xmlNextElementSibling(node), transitions,  isAll, relations, n_relations,  x, map, initial_marking );
    // parse property-set
    else if (xmlStrcmp(node->name, (const xmlChar*) "property-set") == 0) {
        // loop over all children that are property nodes
        for (xmlNode *property = xmlFirstElementChild(node);
                property != NULL && !res;
                property = xmlNextElementSibling(property)) {
            res = parse_xml(property, transitions,  isAll, relations, n_relations,  x, map, initial_marking );
        }
    // parse property
    } else if (xmlStrcmp(node->name, (const xmlChar*) "property") == 0) {
        warn("parsing property");
        res = parse_xml(xmlFirstElementChild(node), transitions,  isAll, relations, n_relations,  x, map, initial_marking);
    // parse id of property
    } else if (xmlStrcmp(node->name, (const xmlChar*) "id") == 0) {
        warn("Property id is: %s", xmlNodeGetContent(node));
        res = parse_xml(xmlNextElementSibling(node), transitions,  isAll, relations, n_relations,  x, map, initial_marking );
    // parse description of property
    } else if (xmlStrcmp(node->name, (const xmlChar*) "description") == 0) {
        warn("Property description is: %s", xmlNodeGetContent(node));
        res = parse_xml(xmlNextElementSibling(node), transitions,  isAll, relations, n_relations,  x, map, initial_marking );
    // parse the formula
    } else if (xmlStrcmp(node->name, (const xmlChar*) "formula") == 0) {
        warn("Parsing formula...");
        parse_formula(xmlFirstElementChild(node));
        BDD ans = parse_formula_BU(xmlFirstElementChild(node), transitions,  isAll, relations, n_relations,  x, map );
        //export_bdd(ans, 9128);
        sylvan_protect(&ans);
        BDD ansz = sylvan_and(initial_marking, ans);
        sylvan_protect(&ansz);

        sylvan_unprotect(&ans);
        printf("\n\n NUMS: %i : %i\n", ansz, sylvan_false);
        sylvan_unprotect(&ansz);
    // sylvan_unprotect(&initial_marking);
       // int satcount = sylvan_satcount(ans, x);
        //printf("satcount: %i\n", satcount);
        res = 0;
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
int
load_xml(const char* name, andl_context_t *transitions, int isAll, BDD relations[], int n_relations, BDD x, BDDMAP map, BDD initial_marking) 
{
    LACE_ME;
    int res;

    LIBXML_TEST_VERSION
    warn("parsing formulas file: %s", name);
    xmlDoc *doc = xmlReadFile(name, NULL, 0);
    if (doc == NULL) res = 1;
    else {
        xmlNode *node = xmlDocGetRootElement(doc);
        res = parse_xml(node, transitions, isAll, relations, n_relations,  x, map, initial_marking );
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
            init_sylvan();

            // execute the main body of code
            do_ss_things(&andl_context, argc, argv);
            deinit_sylvan();
        }
    } else {
        warn("Usage: %s <petri-net>.andl [<CTL-formulas>.xml]", argv[0]);
        res = 1;
    }

    return res;
}

