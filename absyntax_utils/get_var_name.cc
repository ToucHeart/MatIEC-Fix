/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

/*
 *  A small helper visitor class, that will   
 *  return the name (tokn_c *) of a variable, as it will
 *  appear in the variable declaration.
 */



#include "absyntax_utils.hh"



   
    

get_var_name_c *get_var_name_c::singleton_instance_ = NULL;



/*  For ex.:
 *       VAR
 *          A : int;
 *          B : ARRAY [1..9] of int;
 *          C : some_struct_t;
 *       END_VAR
 *
 *          A    := 56;
 *          B[8] := 99;
 *          C.e  := 77;
 *
 *       Calling this method with symbolic_variable_c instance referencing 'A' in
 *       the line 'A := 56', will return the string "A".
 *
 *       Calling this method with array_variable_c instance referencing 'B[8]' in
 *       the line 'B[8] := 99', will return the string "B".
 *
 *       Calling this method with array_variable_c instance referencing 'C.e' in
 *       the line 'C.e := 77', will return the string "C".
 */

token_c *get_var_name_c::get_name(symbol_c *symbol) {
  if (NULL == singleton_instance_) singleton_instance_ = new get_var_name_c(); 
  if (NULL == singleton_instance_) ERROR; 
  
  return (token_c *)(symbol->accept(*singleton_instance_));
}


/*  Return the last field of a structured variable...
 * 
 *          A    := 56;   --> returns A
 *          B[8] := 99;   --> returns B
 *          C.e  := 77;   --> returns e   !!!
 */
symbol_c *get_var_name_c::get_last_field(symbol_c *symbol) {
  if (NULL == singleton_instance_) singleton_instance_ = new get_var_name_c(); 
  if (NULL == singleton_instance_) ERROR; 
  
  singleton_instance_->last_field = NULL;
  symbol_c *res = (symbol_c*)(symbol->accept(*singleton_instance_));
  return (NULL != singleton_instance_->last_field)? singleton_instance_->last_field : res;
}




/*************************/
/* B.1 - Common elements */
/*************************/
/*******************************************/
/* B 1.1 - Letters, digits and identifiers */
/*******************************************/
// SYM_TOKEN(identifier_c)
void *get_var_name_c::visit(identifier_c *symbol) {return (void *)symbol;}

/*********************/
/* B 1.4 - Variables */
/*********************/
// SYM_REF2(symbolic_variable_c, var_name, unused)
void *get_var_name_c::visit(symbolic_variable_c *symbol) {return symbol->var_name->accept(*this);}

/********************************************/
/* B.1.4.1   Directly Represented Variables */
/********************************************/
// SYM_TOKEN(direct_variable_c)
/*ADDNEW:when we meet the direct_variable_c symbol ,just return it for later string compare */
void*get_var_name_c::visit(direct_variable_c*symbol) {return (void*)symbol;}
/*************************************/
/* B.1.4.2   Multi-element Variables */
/*************************************/
/*  subscripted_variable '[' subscript_list ']' */
// SYM_REF2(array_variable_c, subscripted_variable, subscript_list)
void *get_var_name_c::visit(array_variable_c *symbol) {return symbol->subscripted_variable->accept(*this);}

/* subscript_list ',' subscript */
// SYM_LIST(subscript_list_c)

/*  record_variable '.' field_selector */
/*  WARNING: input and/or output variables of function blocks
 *           may be accessed as fields of a tructured variable!
 *           Code handling a structured_variable_c must take
 *           this into account!
 */
// SYM_REF2(structured_variable_c, record_variable, field_selector)
void *get_var_name_c::visit(structured_variable_c *symbol) {
  void *res = symbol->record_variable->accept(*this);
  last_field = symbol->field_selector;
  return res;
}



