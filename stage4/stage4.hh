
/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * This file contains the code that stores the output generated
 * by each specific version of the 4th stage.
 */

#ifndef _STAGE4_HH
#define _STAGE4_HH

#include "../absyntax/absyntax.hh"
/*ADDNEW：Debug Control */
#define DEBUG_FLUSH 1

void stage4err(const char *stage4_generator_id, symbol_c *symbol1, symbol_c *symbol2, const char *errmsg, ...);


class stage4out_c {
  public:
    std::string indent_level;
    std::string indent_spaces;

  public:
    stage4out_c(std::string indent_level = "  ");
    stage4out_c(const char *dir, const char *radix, const char *extension, std::string indent_level = "  ");
    ~stage4out_c(void);
    
    void flush(void);
    
    void enable_output(void);
    void disable_output(void);

    void indent_right(void);
    void indent_left(void);

    void *print(          std::string  value);
    void *print(           const char *value);
    //void *print(               int64_t value); // not required, since we have long long int, or similar
    //void *print(              uint64_t value); // not required, since we have long long int, or similar
    void *print(              real64_t value);
    void *print(                   int value);
    void *print(              long int value);
    void *print(         long long int value);
    void *print(unsigned           int value);
    void *print(unsigned      long int value);
    void *print(unsigned long long int value);
    
    void *print_long_integer(unsigned long l_integer, bool suffix=true);
    void *print_long_long_integer(unsigned long long ll_integer, bool suffix=true);


    void *printupper(const char *str);
    void *printupper(std::string str);

    void *printlocation(const char *str);
    void *printlocation(std::string str);

    void *printlocation_comasep(const char *str);

  protected:
    std::ostream *out;
    std::fstream *m_file;
    
    /* A flag to tell whether to really print to the file, or to ignore any request to print to the file */
    /* This is used to implement the no_code_generation pragmas, that lets the user tell the compiler
     * when to switch on and off the code generation, without stoping the lexical, syntatical, and
     * semantic analysers from analysing the code.
     */
    bool allow_output;

};



int stage4(symbol_c *tree_root, const char *builddir);

/* Functions to be implemented by each generate_XX version of stage 4 */
int  stage4_parse_options(char *options);
void stage4_print_options(void);

#endif /* _STAGE4_HH */
