/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * The public interface to stage1_2.cc 
 */



#ifndef _STAGE1_2_HH
#define _STAGE1_2_HH



/* This file includes the interface through which the main function accesses the stage1_2 services */


int stage1_2(const char *filename, symbol_c **tree_root);





#endif   /* _STAGE1_2_HH */
