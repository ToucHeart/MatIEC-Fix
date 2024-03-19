/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

#include <vector>

#include "../absyntax_utils/absyntax_utils.hh"


class declaration_check_c : public iterator_visitor_c {
    int error_count;
    int current_display_error_level;
    symbol_c *current_pou_decl;
    symbol_c *current_resource_decl;

public:
    declaration_check_c(symbol_c *ignore);
    virtual ~declaration_check_c(void);
    int get_error_count();

    /*****************************/
    /* B 1.5.2 - Function Blocks */
    /*****************************/
    void *visit(function_block_declaration_c *symbol);

    /******************************************/
    /* B 1.5.3 - Declaration & Initialisation */
    /******************************************/
    void *visit(program_declaration_c *symbol);

    /********************************/
    /* B 1.7 Configuration elements */
    /********************************/
    void *visit(configuration_declaration_c *symbol);
    void *visit(resource_declaration_c      *symbol);
    void *visit(program_configuration_c     *symbol);
};
