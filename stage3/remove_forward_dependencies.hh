/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * Re-oder the POUs in te library so that no forward references occur.
 * 
 * Since stage1_2 now suppport POUs that contain references to POUS that are only declared later,
 * (e.g. a variable of FB1_t is declared, before the FB1_T function block is itself declared!)
 * we may need to re-order all the POUs in the library so that these forward references do not occur.
 * 
 * This utility class will do just that. However, it does not destroy the original abstract syntax 
 * tree (AST). It instead creates a new re-ordered AST, by instantiating a new library_c object.
 * This new library_c object will however point to the *same* objects of the original AST, just in 
 * a new order. 
 * This means that the new and original AST share all the object instances, and only use a distinct
 * library_c object!
 */

#include "../absyntax/absyntax.hh"
#include "../absyntax/visitor.hh"
#include "../util/symtable.hh"
#include <set>


class   find_forward_dependencies_c;
typedef symtable_c<symbol_c *> identifiers_symbtable_t;







class remove_forward_dependencies_c: public search_visitor_c {

  private:
    /* The level of detail that the user wants us to display error messages. */
    #define error_level_default (1)
    #define error_level_nagging (4)
    unsigned int    current_display_error_level;
    int             error_count;
    bool            warning_found;
    library_c      *new_tree;
    int             cycle_count; // main algorithm runs in a loop. The nuber of the current cycle...
    // NOTE: we need two lists in order to correctly handle overloaded functions
    identifiers_symbtable_t      declared_identifiers; // list of identifiers already declared by the symbols in the new tree
    std::set <symbol_c *>        inserted_symbols;     // list of symbols already inserted in the new tree 
    symbol_c       *current_code_generation_pragma;    // points to any currently 'active' enable_code_generation_pragma_c
    find_forward_dependencies_c    *find_forward_dependencies;  

  public:
     remove_forward_dependencies_c(void);
    ~remove_forward_dependencies_c(void);
    library_c *create_new_tree(symbol_c *old_tree);  // create a new tree with POUs ordered so it does not contain forward dependencies...
    int        get_error_count(void);

  private:
    void *handle_library_symbol(symbol_c *symbol, symbol_c *name, symbol_c *search1, symbol_c *search2 = NULL, symbol_c *search3 = NULL);
    void  print_circ_error(library_c *symbol);

    /***************************/
    /* B 0 - Programming Model */
    /***************************/
    void *visit(library_c *symbol);
    /**************************************/
    /* B.1.5 - Program organization units */
    /**************************************/
    void *visit(function_declaration_c *symbol);
    void *visit(function_block_declaration_c *symbol);
    void *visit(program_declaration_c *symbol);
    /********************************/
    /* B 1.7 Configuration elements */
    /********************************/
    void *visit(configuration_declaration_c *symbol);
    /********************/
    /* 2.1.6 - Pragmas  */
    /********************/
    void *visit(disable_code_generation_pragma_c *symbol);
    void *visit(enable_code_generation_pragma_c *symbol);
    void *visit(pragma_c *symbol);

};   /* class remove_forward_dependencies_c */



