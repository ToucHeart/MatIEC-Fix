/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * Case Options Checking:
 *   - Check whether the options in a case statement are repeated, either directly, or in a range.
 *       For example:
 *         case var of
 *           1: ...   <- OK
 *           2: ...   <- OK
 *           1: ...   <- OK (not an error), but produce a warning!
 *           0..8: ...<- OK (not an error), but produce a warning!
 */

#include "../absyntax_utils/absyntax_utils.hh"



class case_elements_check_c: public iterator_visitor_c {

  private:
    bool warning_found;
    int error_count;
    int current_display_error_level;

    std::vector<symbol_c *> case_elements_list;
    void check_subr_subr(symbol_c *s1, symbol_c *s2);
    void check_subr_symb(symbol_c *s1, symbol_c *s2);
    void check_symb_symb(symbol_c *s1, symbol_c *s2);
  

  public:
    case_elements_check_c(symbol_c *ignore);
    virtual ~case_elements_check_c(void);
    int get_error_count();

    /***************************************/
    /* B.3 - Language ST (Structured Text) */
    /***************************************/
    /********************/
    /* B 3.2 Statements */
    /********************/
    /********************************/
    /* B 3.2.3 Selection Statements */
    /********************************/
    void *visit(case_statement_c *symbol);
    void *visit(case_list_c      *symbol);
}; /* case_elements_check_c */







