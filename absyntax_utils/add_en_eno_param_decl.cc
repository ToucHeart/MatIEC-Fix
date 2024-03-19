/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * Iterate through all declared functions and Function Blocks, 
 * and, for each function/FB, add a declaration of the EN and ENO 
 * parameters, if they have not already been explicitly declared.
 *
 * EN and ENO parameters declared explicitly (by the user in the source code)
 * and implicitly (by the comnpiler, i.e. by this visitor class) may be
 * distinguished later on by the 'method' flag in the en_param_declaration_c
 * and eno_param_declaration_c objects.
 */

#include "add_en_eno_param_decl.hh"
#include <strings.h>
#include "../main.hh" // required for ERROR() and ERROR_MSG() macros.


// #define DEBUG
#ifdef DEBUG
#define TRACE(classname) printf("\n____%s____\n",classname);
#else
#define TRACE(classname)
#endif





/* This class is a singleton.
 * So we need a pointer to the singe instance...
 */
add_en_eno_param_decl_c *add_en_eno_param_decl_c::singleton = NULL;

/* Constructor for the singleton class */
symbol_c *add_en_eno_param_decl_c::add_to(symbol_c *tree_root) {
      if (NULL == singleton) {
        singleton = new add_en_eno_param_decl_c;
        if (NULL == singleton)
          return NULL;
      }
      tree_root->accept(*singleton);
      return tree_root;
    }

/* Destructor for the singleton class */
add_en_eno_param_decl_c::~add_en_eno_param_decl_c(void) {
      if (NULL != singleton) delete singleton;
      singleton = NULL;
    }



void* add_en_eno_param_decl_c::iterate_list(list_c *list) {
  for (int i = 0; i < list->n; i++) {
    list->get_element(i)->accept(*this);
  }
  return NULL;
}


input_declarations_c *add_en_eno_param_decl_c::build_en_param(void) {
  boolean_literal_c  *boolean_literal = new boolean_literal_c(new bool_type_name_c(), new boolean_true_c());
  identifier_c       *identifier      = new identifier_c("EN");
  simple_spec_init_c *type_spec_init  = new simple_spec_init_c(new bool_type_name_c(), boolean_literal);
  en_param_declaration_c *en_param_declaration = 
    new en_param_declaration_c(identifier, type_spec_init, new implicit_definition_c());
    /* the last paramater is to flag that this
     * declaration was inserted automatically, i.e. an implicit declaration 
     */
  input_declaration_list_c *input_declaration_list = new input_declaration_list_c();
  input_declaration_list->add_element(en_param_declaration);

  input_declarations_c *input_declarations = new input_declarations_c(NULL, input_declaration_list, new implicit_definition_c());
  return input_declarations;
}


output_declarations_c *add_en_eno_param_decl_c::build_eno_param(void) {
  identifier_c *identifier = new identifier_c("ENO");
  eno_param_declaration_c *eno_param_declaration =
    new eno_param_declaration_c(identifier, new bool_type_name_c(), new implicit_definition_c()); 
    /* the last paramater is to flag that this
     * declaration was inserted automatically, i.e. an implicit declaration 
     */
  var_init_decl_list_c *var_init_decl_list = new var_init_decl_list_c();
  var_init_decl_list->add_element(eno_param_declaration);

  output_declarations_c *output_declarations = new output_declarations_c(NULL, var_init_decl_list, new implicit_definition_c());
  return output_declarations;
}




/***************************/
/* B 0 - Programming Model */
/***************************/
void *add_en_eno_param_decl_c::visit(library_c *symbol) {
  TRACE("library_c"); 
  return iterate_list(symbol);
}


/***********************/
/* B 1.5.1 - Functions */
/***********************/
void *add_en_eno_param_decl_c::visit(function_declaration_c *symbol) {
  TRACE("function_declaration_c"); 
  return symbol->var_declarations_list->accept(*this);
}

void *add_en_eno_param_decl_c::visit(var_declarations_list_c *symbol) {
    TRACE("var_declarations_list_c");
    en_declared  = false; 
    eno_declared = false; 
    iterate_list(symbol);
    /* insert elements to begining of list! */
    /* We want EN first, and then ENO.
     * But, since we are insertin them into the head of the list, we must insert EN last so it will stay in the first position!
     */
    if(eno_declared == false) symbol->insert_element(build_eno_param());
    if(en_declared  == false) symbol->insert_element(build_en_param());
    /* append elements to end of list! */
    /*
    if(en_declared  == false) symbol->add_element(build_en_param());
    if(eno_declared == false) symbol->add_element(build_eno_param());
    */
    return NULL;
}



/****************************************/
/* 1.4.3 - Declaration & Initialisation */
/****************************************/
void *add_en_eno_param_decl_c::visit(input_declarations_c *symbol) {
  TRACE("input_declarations_c");
//   current_param_direction = direction_in;
  return symbol->input_declaration_list->accept(*this);
}

void *add_en_eno_param_decl_c::visit(input_declaration_list_c *symbol) {TRACE("input_declaration_list_c"); return iterate_list(symbol);}

void *add_en_eno_param_decl_c::visit(en_param_declaration_c *symbol) {
  TRACE("en_param_declaration_c");
  en_declared = true;
  return NULL;
}

void *add_en_eno_param_decl_c::visit(eno_param_declaration_c *symbol) {
  TRACE("eno_param_declaration_c");
  eno_declared = true;
  return NULL;
}

void *add_en_eno_param_decl_c::visit(output_declarations_c *symbol) {
  TRACE("output_declarations_c");
//   current_param_direction = direction_out;
  return symbol->var_init_decl_list->accept(*this);
}

void *add_en_eno_param_decl_c::visit(var_init_decl_list_c *symbol) 
{TRACE("var_init_decl_list_c"); return iterate_list(symbol);}


/*****************************/
/* B 1.5.2 - Function Blocks */
/*****************************/
/*  FUNCTION_BLOCK derived_function_block_name io_OR_other_var_declarations function_block_body END_FUNCTION_BLOCK */
void *add_en_eno_param_decl_c::visit(function_block_declaration_c *symbol) {
  TRACE("function_block_declaration_c"); 
  return symbol->var_declarations->accept(*this);
}




