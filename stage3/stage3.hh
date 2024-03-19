/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

#ifndef _STAGE3_HH
#define _STAGE3_HH

#include "../util/symtable.hh"


int stage3(symbol_c *tree_root, symbol_c **ordered_tree_root);

#endif /* _STAGE3_HH */
