/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 *  Search for a specific label in an IL list. 
 *
 *  when instantiated, must be given a pointer to one of the following
 *     - function_declaration_c
 *     - function_block_declaration_c
 *     - program_declaration_c
 *     - instruction_list_c
 *
 * which is where all calls to search for a specific label will look for said label.
 */



#include "../absyntax_utils/absyntax_utils.hh"


class search_il_label_c: public search_visitor_c {

  private:
    search_varfb_instance_type_c *search_varfb_instance_type;
    symbol_c *search_scope;
    symbol_c *search_label;

  public:
    search_il_label_c(symbol_c *search_scope);
    virtual ~search_il_label_c(void);

    il_instruction_c *find_label(const char *label);
    il_instruction_c *find_label(symbol_c   *label);

    
    /****************************************/
    /* B.2 - Language IL (Instruction List) */
    /****************************************/
    /***********************************/
    /* B 2.1 Instructions and Operands */
    /***********************************/
//     void *visit(instruction_list_c *symbol);
    void *visit(il_instruction_c *symbol);
//     void *visit(il_simple_operation_c *symbol);
//     void *visit(il_function_call_c *symbol);
//     void *visit(il_expression_c *symbol);
//     void *visit(il_fb_call_c *symbol);
//     void *visit(il_formal_funct_call_c *symbol);
//     void *visit(il_operand_list_c *symbol);
//     void *visit(simple_instr_list_c *symbol);
//     void *visit(il_simple_instruction_c*symbol);
//     void *visit(il_param_list_c *symbol);
//     void *visit(il_param_assignment_c *symbol);
//     void *visit(il_param_out_assignment_c *symbol);


}; // search_il_label_c








