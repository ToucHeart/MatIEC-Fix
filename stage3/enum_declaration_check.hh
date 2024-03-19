/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


#include "../absyntax_utils/absyntax_utils.hh"

class populate_enumvalue_symtable_c;

class enum_declaration_check_c : public iterator_visitor_c {
  private:
    int error_count;
    int current_display_error_level;
    populate_enumvalue_symtable_c *populate_enumvalue_symtable;
    symbol_c::enumvalue_symtable_t *global_enumvalue_symtable;
    
  public:
     enum_declaration_check_c(symbol_c *ignore);
    ~enum_declaration_check_c(void);
    int get_error_count();

    
    /***************************/
    /* B 0 - Programming Model */
    /***************************/
    void *visit(library_c *symbol);

    /**********************/
    /* B.1.3 - Data types */
    /**********************/
    void *visit(data_type_declaration_c *symbol);
  
    /***********************/
    /* B 1.5.1 - Functions */
    /***********************/
    void *visit(function_declaration_c *symbol);
   
    /*****************************/
    /* B 1.5.2 - Function Blocks */
    /*****************************/
    void *visit(function_block_declaration_c *symbol);

    /******************************************/
    /* B 1.5.3 - Declaration & Initialisation */
    /******************************************/
    void *visit(program_declaration_c *symbol);

    /********************************/
    /* B 1.7 Configuration elements */
    /********************************/
    void *visit(configuration_declaration_c *symbol);
    void *visit(resource_declaration_c *symbol);
    void *visit(single_resource_declaration_c *symbol);    
};
