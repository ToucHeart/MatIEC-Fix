/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

#include <vector>
#include "../absyntax_utils/absyntax_utils.hh"
#include "datatype_functions.hh"


/* Expressions on the left hand side of assignment statements have aditional restrictions on their datatype.
 * For example, they cannot be literals, CONSTANT type variables, function invocations, etc...
 * This class wil do those checks.
 * 
 * Note that assignment may also be done when passing variables to OUTPUT or IN_OUT function parameters,so we check those too.
 */



class lvalue_check_c: public iterator_visitor_c {

  private:
    search_varfb_instance_type_c *search_varfb_instance_type;
    search_var_instance_decl_c *search_var_instance_decl;
    int error_count;
    int current_display_error_level;
    std::vector <token_c *> control_variables;
    symbol_c *current_il_operand;

    void verify_is_lvalue              (symbol_c *lvalue);
    void check_assignment_to_controlvar(symbol_c *lvalue);
    void check_assignment_to_output    (symbol_c *lvalue);
    void check_assignment_to_constant  (symbol_c *lvalue);
    void check_assignment_to_expression(symbol_c *lvalue);
    void check_assignment_to_il_list   (symbol_c *lvalue);
    
    void check_formal_call   (symbol_c *f_call, symbol_c *f_decl);
    void check_nonformal_call(symbol_c *f_call, symbol_c *f_decl);


  public:
    lvalue_check_c(symbol_c *ignore);
    virtual ~lvalue_check_c(void);
    int get_error_count();

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

    /****************************************/
    /* B.2 - Language IL (Instruction List) */
    /****************************************/
    /***********************************/
    /* B 2.1 Instructions and Operands */
    /***********************************/
    void *visit(il_instruction_c *symbol);
    void *visit(il_simple_operation_c *symbol);
    void *visit(il_function_call_c *symbol);
    void *visit(il_fb_call_c *symbol);
    void *visit(il_formal_funct_call_c *symbol);

    /*******************/
    /* B 2.2 Operators */
    /*******************/
    void *visit(ST_operator_c *symbol);
    void *visit(STN_operator_c *symbol);
    void *visit(S_operator_c *symbol);
    void *visit(R_operator_c *symbol);

    /***************************************/
    /* B.3 - Language ST (Structured Text) */
    /***************************************/
    /***********************/
    /* B 3.1 - Expressions */
    /***********************/
    void *visit(function_invocation_c *symbol);

    /*********************************/
    /* B 3.2.1 Assignment Statements */
    /*********************************/
    void *visit(assignment_statement_c *symbol);

    /*****************************************/
    /* B 3.2.2 Subprogram Control Statements */
    /*****************************************/
    void *visit(fb_invocation_c *symbol);

    /********************************/
    /* B 3.2.4 Iteration Statements */
    /********************************/
    void *visit(for_statement_c *symbol);

}; /* lvalue_check_c */







