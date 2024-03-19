/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

/*
 *  A small helper visitor class, that will   
 *  return the name (tokn_c *) of a variable, as it will
 *  appear in the variable declaration.
 */

/*  For ex.:
 *       VAR
 *          A : int;
 *          B : ARRAY [1..9] of int;
 *          C : some_struct_t;
 *       END_VAR
 *
 *          A    := 56;
 *          B[8] := 99;
 *          C.e  := 77;
 *
 *       Calling this visitor class with symbolic_variable_c instance referencing 'A' in
 *       the line 'A := 56', will return the string "A".
 *
 *       Calling this visitor class with array_variable_c instance referencing 'B[8]' in
 *       the line 'B[8] := 99', will return the string "B".
 *
 *       Calling this visitor class with array_variable_c instance referencing 'C.e' in
 *       the line 'C.e := 77', will return the string "C".
 */




class get_var_name_c : public search_visitor_c {    
  public:
    get_var_name_c(void)  {};
    ~get_var_name_c(void) {};  
    static token_c  *get_name(symbol_c *symbol);
    static symbol_c *get_last_field(symbol_c *symbol);
    
  private:
    static get_var_name_c *singleton_instance_;
    symbol_c *last_field;
    
  private:  
    /*************************/
    /* B.1 - Common elements */
    /*************************/
    /*******************************************/
    /* B 1.1 - Letters, digits and identifiers */
    /*******************************************/
    // SYM_TOKEN(identifier_c)
    void *visit(identifier_c *symbol);

    /*********************/
    /* B 1.4 - Variables */
    /*********************/
    // SYM_REF2(symbolic_variable_c, var_name, unused)
    void *visit(symbolic_variable_c *symbol);

    /********************************************/
    /* B.1.4.1   Directly Represented Variables */
    /********************************************/
    // SYM_TOKEN(direct_variable_c)
    /*ADDNEW:when we meet the direct_variable_c symbol ,just return it for later string compare */
    void *visit(direct_variable_c *symbol);

    /*************************************/
    /* B.1.4.2   Multi-element Variables */
    /*************************************/
    /*  subscripted_variable '[' subscript_list ']' */
    // SYM_REF2(array_variable_c, subscripted_variable, subscript_list)
    void *visit(array_variable_c *symbol);

    /* subscript_list ',' subscript */
    // SYM_LIST(subscript_list_c)
    // void *visit(subscript_list_c *symbol);

    /*  record_variable '.' field_selector */
    /*  WARNING: input and/or output variables of function blocks
     *           may be accessed as fields of a tructured variable!
     *           Code handling a structured_variable_c must take
     *           this into account!
     */
    // SYM_REF2(structured_variable_c, record_variable, field_selector)
    void *visit(structured_variable_c *symbol);
};



