/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * This is the main stage 3a file.
 *
 * In stage 3a some helpful symbol tables are instanciated and populated.
 * These symbol tables wll then be used by stage3b and atage4 code generators.
 */




#ifndef _SEARCH_UTILS_HH
#define _SEARCH_UTILS_HH

// #include <stdio.h>  /* required for NULL */
#include "../util/symtable.hh"
#include "../util/dsymtable.hh"
#include "../absyntax/absyntax.hh"
#include "../absyntax/visitor.hh"



/* returns 0 if the names are equal!! Case is ignored. */
int compare_identifiers(symbol_c *ident1, symbol_c *ident2);

/* A symbol table with all globally declared functions... */
typedef dsymtable_c<function_declaration_c *> function_symtable_t;
extern function_symtable_t function_symtable;

/* A symbol table with all globally declared functions block types... */
typedef symtable_c<function_block_declaration_c *> function_block_type_symtable_t;
extern  function_block_type_symtable_t function_block_type_symtable;

/* A symbol table with all globally declared program types... */
typedef symtable_c<program_declaration_c *> program_type_symtable_t;
extern  program_type_symtable_t program_type_symtable;

/* A symbol table with all user declared type definitions... */
/* Note that function block types and program types have their
 * own symbol tables, so do not get placed in this symbol table!
 *
 * The symbol_c * associated to the value will point to the data type declaration.
 */
typedef symtable_c<symbol_c *> type_symtable_t;
extern  type_symtable_t type_symtable;


/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

#include "spec_init_separator.hh"
#include "array_dimension_iterator.hh"
#include "case_element_iterator.hh"
#include "function_param_iterator.hh"
#include "function_call_iterator.hh"
#include "function_call_param_iterator.hh"
#include "type_initial_value.hh"
#include "search_fb_instance_decl.hh"
#include "search_fb_typedecl.hh"
#include "search_base_type.hh"
#include "search_var_instance_decl.hh"
#include "decompose_var_instance_name.hh"
#include "search_varfb_instance_type.hh"
#include "add_en_eno_param_decl.hh"
#include "get_sizeof_datatype.hh"
#include "search_il_label.hh"
#include "get_var_name.hh"
#include "get_datatype_info.hh"
#include "debug_ast.hh"

/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/




void absyntax_utils_init(symbol_c *tree_root);


#endif /* _SEARCH_UTILS_HH */
