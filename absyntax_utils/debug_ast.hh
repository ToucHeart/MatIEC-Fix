/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * Some classes to help with debuging.
 *
 * These classes will print out the current state of a symbol or a portion of the Abstract Syntax Tree.
 */






#include "../absyntax/absyntax.hh"


class debug_c { 
  public:
    static void print(const_value_c  cvalue);
    static void print(symbol_c      *symbol);
    static void print(const char    *str);

    /* print the AST from this point downwards */
    static void print_ast(symbol_c *root_symbol);
};


















