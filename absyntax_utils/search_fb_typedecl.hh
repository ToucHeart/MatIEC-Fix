/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/* Returns the function block declaration symbol
 * of a specific function block type.
 */

class search_fb_typedecl_c: public search_visitor_c {

  private:
    symbol_c *search_scope;

    symbol_c *search_name;

  public:
    search_fb_typedecl_c(symbol_c *search_scope);
    symbol_c *get_decl(symbol_c *fb_type_name);

  private:
    /**************************************/
    /* B.1.5 - Program organization units */
    /**************************************/

    /*****************************/
    /* B 1.5.2 - Function Blocks */
    /*****************************/
    void *visit(function_block_declaration_c *symbol);

    /**********************/
    /* B 1.5.3 - Programs */
    /**********************/
    void *visit(program_declaration_c *symbol);
}; // search_fb_typedecl_c
