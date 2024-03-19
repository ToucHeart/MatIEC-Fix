/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

// #include <vector>
#include "../absyntax_utils/absyntax_utils.hh"
// #include "datatype_functions.hh"



class array_range_check_c: public iterator_visitor_c {

  private:
    search_varfb_instance_type_c *search_varfb_instance_type;
    int error_count;
    int current_display_error_level;

    void check_dimension_count(array_variable_c *symbol);
    void check_bounds(array_variable_c *symbol);

  public:
    array_range_check_c(symbol_c *ignore);
    virtual ~array_range_check_c(void);
    int get_error_count();

    /*************************/
    /* B.1 - Common elements */
    /*************************/
    /**********************/
    /* B.1.3 - Data types */
    /**********************/
    /********************************/
    /* B 1.3.3 - Derived data types */
    /********************************/
    /* NOTE: we may later want to move the following 2 methods to a visitor that will focus on analysing the data type declarations! */
    void *visit(subrange_c *symbol);
    void *visit(array_initial_elements_c *symbol);
  
    /*********************/
    /* B 1.4 - Variables */
    /*********************/
    /*************************************/
    /* B 1.4.2 - Multi-element variables */
    /*************************************/
    void *visit(array_variable_c *symbol);

    /**************************************/
    /* B 1.5 - Program organisation units */
    /**************************************/
    /***********************/
    /* B 1.5.1 - Functions */
    /***********************/
    void *visit(function_declaration_c *symbol);

    /*****************************/
    /* B 1.5.2 - Function blocks */
    /*****************************/
    void *visit(function_block_declaration_c *symbol);

    /**********************/
    /* B 1.5.3 - Programs */
    /**********************/
    void *visit(program_declaration_c *symbol);

}; /* array_range_check_c */







