/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * Function call parameter iterator.
 *
 * This is part of the 4th stage that generates
 * a c++ source program equivalent to the IL and ST
 * code.
 */

/* given a function_body_c, iterate through each
 * function/FB call in that code.
 */


#include "function_call_iterator.hh"
#include "../main.hh" // required for ERROR() and ERROR_MSG() macros.


//#define DEBUG
#ifdef DEBUG
#define TRACE(classname) printf("\n____%s____\n",classname);
#else
#define TRACE(classname)
#endif





/* initialise the iterator object.
 * We must be given a reference to the function declaration
 * that will be analysed...
 */
function_call_iterator_c::function_call_iterator_c(symbol_c *symbol) {
  this->start_symbol = symbol;
  next_fcall = fcall_count = 0;
  current_finvocation = NULL;
  current_fcall_name = NULL;
}

/* Skip to the next function call. After object creation,
 * the object references _before_ the first, so
 * this function must be called once to get the object to
 * reference the first function call...
 *
 * Returns the function_invocation_c!
 */
//function_invocation_c *next(void) {TRACE("function_call_iterator_c::next(): called ");
symbol_c *function_call_iterator_c::next(void) {TRACE("function_call_iterator_c::next(): called ");
  fcall_count = 0;
  next_fcall++;
  current_finvocation = NULL;
  current_fcall_name = NULL;

  start_symbol->accept(*this);
  return current_finvocation;
}

/* Returns the name of the currently referenced function invocation */
token_c *function_call_iterator_c::fname(void) {
  token_c *fname_sym = dynamic_cast<token_c *>(current_fcall_name);
  if (fname_sym == NULL) ERROR;
  return fname_sym;
}


/***************************************/
/* B.3 - Language ST (Structured Text) */
/***************************************/
/***********************/
/* B 3.1 - Expressions */
/***********************/
  void *function_call_iterator_c::visit(function_invocation_c *symbol) {
    fcall_count++;
    if (next_fcall == fcall_count) {
      current_finvocation = symbol;
      current_fcall_name = symbol->function_name;
    }
    return NULL;
  }



/****************************************/
/* B.2 - Language IL (Instruction List) */
/****************************************/
/***********************************/
/* B 2.1 Instructions and Operands */
/***********************************/

/* | function_name [il_operand_list] */
// SYM_REF2(il_function_call_c, function_name, il_operand_list)
  void *function_call_iterator_c::visit(il_function_call_c *symbol) {
    fcall_count++;
    if (next_fcall == fcall_count) {
      current_finvocation = symbol;
      current_fcall_name = symbol->function_name;
    }
    return NULL;
  }



/* | function_name '(' eol_list [il_param_list] ')' */
// SYM_REF2(il_formal_funct_call_c, function_name, il_param_list)
  void *function_call_iterator_c::visit(il_formal_funct_call_c *symbol) {
    fcall_count++;
    if (next_fcall == fcall_count) {
      current_finvocation = symbol;
      current_fcall_name = symbol->function_name;
    }
    return NULL;
  }





