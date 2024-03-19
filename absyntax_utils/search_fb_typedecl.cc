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

#include "absyntax_utils.hh"


search_fb_typedecl_c::search_fb_typedecl_c(symbol_c *search_scope) {
  this->search_scope = search_scope;
}

symbol_c *search_fb_typedecl_c::get_decl(symbol_c *fb_type_name) {
  this->search_name = fb_type_name;
  return (symbol_c *)search_scope->accept(*this);
}
/**************************************/
/* B.1.5 - Program organization units */
/**************************************/

/*****************************/
/* B 1.5.2 - Function Blocks */
/*****************************/
void *search_fb_typedecl_c::visit(function_block_declaration_c *symbol) {
  if (compare_identifiers(symbol->fblock_name, search_name) == 0)
    return symbol;
  return NULL;
}

/**********************/
/* B 1.5.3 - Programs */
/**********************/
void *search_fb_typedecl_c::visit(program_declaration_c *symbol) {
  if (compare_identifiers(symbol->program_type_name, search_name) == 0)
    return symbol;
  return NULL;
}

