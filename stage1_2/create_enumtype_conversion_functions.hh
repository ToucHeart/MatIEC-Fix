/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

/*
 * create_enumtype_conversion_functions_c generates ST conversion functions source code for
 * enumerate user defined data types.
 *
 */

#ifndef _CREATE_ENUMTYPE_CONVERSION_FUNCTIONS_HH
#define _CREATE_ENUMTYPE_CONVERSION_FUNCTIONS_HH

#include <string>
#include <list>

#include "../absyntax_utils/absyntax_utils.hh"


class create_enumtype_conversion_functions_c: public iterator_visitor_c {
  private:
    static create_enumtype_conversion_functions_c *singleton;
    
  public:
    explicit create_enumtype_conversion_functions_c(symbol_c *ignore);
    virtual ~create_enumtype_conversion_functions_c(void);
    static std::string &get_declaration(symbol_c *symbol);

    void *visit(                 identifier_c *symbol);
    void *visit(         poutype_identifier_c *symbol);
    void *visit(derived_datatype_identifier_c *symbol);
    
    /**********************/
    /* B 1.3 - Data types */
    /**********************/
    /********************************/
    /* B 1.3.3 - Derived data types */
    /********************************/
    void *visit(enumerated_type_declaration_c *symbol);
    void *visit(enumerated_value_list_c *symbol);

  private:
    std::string text;
    std::string currentToken;
    std::list <std::string> currentTokenList;
    std::string getIntegerName(bool isSigned, size_t size);
    void printStringToEnum  (std::string &enumerateName, std::list <std::string> &enumerateValues);
    void printEnumToString  (std::string &enumerateName, std::list <std::string> &enumerateValues);
    void printIntegerToEnum (std::string &enumerateName, std::list <std::string> &enumerateValues, bool isSigned, size_t size);
    void printEnumToInteger (std::string &enumerateName, std::list <std::string> &enumerateValues, bool isSigned, size_t size);
};

#endif /* _CREATE_ENUMTYPE_CONVERSION_FUNCTIONS_HH */
