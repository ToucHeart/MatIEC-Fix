/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/* Determine the data type on which another data type is based on. */

/*
 * What is a Base Type?
 *    A base type is the fundamental data type from which the type is derived.
 *    The main idea is that if two datatyes (A and B) share a common base type, 
 *    then these two datatypes may be used interchangeably in an expression.
 * 
 * What is an Equivalent Type? 
 *    An equivalent type is the data type from which the type is derived.
 *    The Base type and the Equivalent type will always be the same, with the
 *    exception of subranges!
 *
 * E.g. TYPE new_int_t  : INT; END_TYPE;
 *      TYPE new_int2_t : INT := 2; END_TYPE;
 *      TYPE new_int3_t : new_int2_t := 3; END_TYPE;
 *      TYPE new_sub_t  : INT (4..10); END_TYPE;
 *      TYPE new_sub2_t : new_sub_t  := 5 ; END_TYPE;
 *      TYPE new_sub3_t : new_sub2_t := 6 ; END_TYPE;
 *      TYPE new_sub4_t : new_int3_t (4..10); END_TYPE;    <-----  This is NOT legal syntax!
 *
 *    new_int_t   : base type->INT          equivalent type->INT
 *    new_int2_t  : base type->INT          equivalent type->INT
 *    new_int3_t  : base type->INT          equivalent type->INT
 *    new_sub_t   : base type->INT          equivalent type->new_sub_t
 *    new_sub2_t  : base type->INT          equivalent type->new_sub_t
 *    new_sub3_t  : base type->INT          equivalent type->new_sub_t
 *
 * Note too that a FB declaration is also considered a base type, as
 * we may have FB instances declared of a specific FB type.
 */


class search_base_type_c: public null_visitor_c {

  private:
    symbol_c *current_basetype_name;
    symbol_c *current_basetype;
    symbol_c *current_equivtype;
    static search_base_type_c *search_base_type_singleton; // Make this a singleton class!
    
  private:  
    static void create_singleton(void);
    void *handle_datatype_identifier(token_c *type_name);

  public:
    search_base_type_c(void);
    static symbol_c *get_equivtype_decl(symbol_c *symbol);  /* get the Equivalent Type declaration */
    static symbol_c *get_basetype_decl (symbol_c *symbol);  /* get the Base       Type declaration */
    static symbol_c *get_basetype_id   (symbol_c *symbol);  /* get the Base       Type identifier  */

  public:
  /*************************/
  /* B.1 - Common elements */
  /*************************/
  /*******************************************/
  /* B 1.1 - Letters, digits and identifiers */
  /*******************************************/
    void *visit(                 identifier_c *type_name);
    void *visit(derived_datatype_identifier_c *type_name);
    void *visit(         poutype_identifier_c *type_name);

  /*********************/
  /* B 1.2 - Constants */
  /*********************/
  /******************************/
  /* B 1.2.1 - Numeric Literals */
  /******************************/
     /* Numeric literals without any explicit type cast have unknown data type, 
      * so we continue considering them as their own basic data types until
      * they can be resolved (for example, when using '30+x' where 'x' is a LINT variable, the
      * numeric literal '30' must then be considered a LINT so the ADD function may be called
      * with all inputs of the same data type.
      * If 'x' were a SINT, then the '30' would have to be a SINT too!
      */
    void *visit(real_c *symbol);
    void *visit(neg_real_c *symbol);
    void *visit(integer_c *symbol);
    void *visit(neg_integer_c *symbol);
    void *visit(binary_integer_c *symbol);
    void *visit(octal_integer_c *symbol);
    void *visit(hex_integer_c *symbol);
    void *visit(boolean_true_c *symbol);
    void *visit(boolean_false_c *symbol);


  /***********************************/
  /* B 1.3.1 - Elementary Data Types */
  /***********************************/
    void *visit(time_type_name_c *symbol);
    void *visit(bool_type_name_c *symbol);
    void *visit(sint_type_name_c *symbol);
    void *visit(int_type_name_c *symbol);
    void *visit(dint_type_name_c *symbol);
    void *visit(lint_type_name_c *symbol);
    void *visit(usint_type_name_c *symbol);
    void *visit(uint_type_name_c *symbol);
    void *visit(udint_type_name_c *symbol);
    void *visit(ulint_type_name_c *symbol);
    void *visit(real_type_name_c *symbol);
    void *visit(lreal_type_name_c *symbol);
    void *visit(date_type_name_c *symbol);
    void *visit(tod_type_name_c *symbol);
    void *visit(dt_type_name_c *symbol);
    void *visit(byte_type_name_c *symbol);
    void *visit(word_type_name_c *symbol);
    void *visit(dword_type_name_c *symbol);
    void *visit(lword_type_name_c *symbol);
    void *visit(string_type_name_c *symbol);
    void *visit(wstring_type_name_c *symbol);
    void *visit(void_type_name_c *symbol); /* A non standard datatype! */

    /******************************************************/
    /* Extensions to the base standard as defined in      */
    /* "Safety Software Technical Specification,          */
    /*  Part 1: Concepts and Function Blocks,             */
    /*  Version 1.0 – Official Release"                   */
    /* by PLCopen - Technical Committee 5 - 2006-01-31    */
    /******************************************************/
    void *visit(safetime_type_name_c *symbol);
    void *visit(safebool_type_name_c *symbol);
    void *visit(safesint_type_name_c *symbol);
    void *visit(safeint_type_name_c *symbol);
    void *visit(safedint_type_name_c *symbol);
    void *visit(safelint_type_name_c *symbol);
    void *visit(safeusint_type_name_c *symbol);
    void *visit(safeuint_type_name_c *symbol);
    void *visit(safeudint_type_name_c *symbol);
    void *visit(safeulint_type_name_c *symbol);
    void *visit(safereal_type_name_c *symbol);
    void *visit(safelreal_type_name_c *symbol);
    void *visit(safedate_type_name_c *symbol);
    void *visit(safetod_type_name_c *symbol);
    void *visit(safedt_type_name_c *symbol);
    void *visit(safebyte_type_name_c *symbol);
    void *visit(safeword_type_name_c *symbol);
    void *visit(safedword_type_name_c *symbol);
    void *visit(safelword_type_name_c *symbol);
    void *visit(safestring_type_name_c *symbol);
    void *visit(safewstring_type_name_c *symbol);

  /********************************/
  /* B.1.3.2 - Generic data types */
  /********************************/
  void *visit(generic_type_any_c *symbol);

  /********************************/
  /* B 1.3.3 - Derived data types */
  /********************************/
  /*ADDNEW:*/
  void *visit(single_byte_limited_len_string_spec_c*symbol);
  void *visit(single_byte_string_spec_c*symbol);
  void *visit(double_byte_limited_len_string_spec_c*symbol);
  void *visit(double_byte_string_spec_c*symbol);
  /*  simple_type_name ':' simple_spec_init */
    void *visit(simple_type_declaration_c *symbol);
  /* simple_specification ASSIGN constant */
    void *visit(simple_spec_init_c *symbol);
  /*  subrange_type_name ':' subrange_spec_init */
    void *visit(subrange_type_declaration_c *symbol);
  /* subrange_specification ASSIGN signed_integer */
    void *visit(subrange_spec_init_c *symbol);
  /*  integer_type_name '(' subrange')' */
    void *visit(subrange_specification_c *symbol);
  /*  signed_integer DOTDOT signed_integer */
    void *visit(subrange_c *symbol);

  /*  enumerated_type_name ':' enumerated_spec_init */
    void *visit(enumerated_type_declaration_c *symbol);
  /* enumerated_specification ASSIGN enumerated_value */
    void *visit(enumerated_spec_init_c *symbol);
  /* helper symbol for enumerated_specification->enumerated_spec_init */
  /* enumerated_value_list ',' enumerated_value */
    void *visit(enumerated_value_list_c *symbol);
  /* enumerated_type_name '#' identifier */
  // SYM_REF2(enumerated_value_c, type, value)
    void *visit(enumerated_value_c *symbol);
  /*  identifier ':' array_spec_init */
    void *visit(array_type_declaration_c *symbol);
  /* array_specification [ASSIGN array_initialization} */
  /* array_initialization may be NULL ! */
    void *visit(array_spec_init_c *symbol);
  /* ARRAY '[' array_subrange_list ']' OF non_generic_type_name */
    void *visit(array_specification_c *symbol);
  /* helper symbol for array_specification */
  /* array_subrange_list ',' subrange */
    void *visit(array_subrange_list_c *symbol);
  /* array_initialization:  '[' array_initial_elements_list ']' */
  /* helper symbol for array_initialization */
  /* array_initial_elements_list ',' array_initial_elements */
    void *visit(array_initial_elements_list_c *symbol);
  /* integer '(' [array_initial_element] ')' */
  /* array_initial_element may be NULL ! */
    void *visit(array_initial_elements_c *symbol);
  /*  structure_type_name ':' structure_specification */
      /* NOTE: structure_specification will point to either a
       *       initialized_structure_c
       *       OR A
       *       structure_element_declaration_list_c
       */
    void *visit(structure_type_declaration_c *symbol);
  /*  var1_list ':' structure_type_name */
    void *visit(structured_var_declaration_c *symbol);
  /* structure_type_name ASSIGN structure_initialization */
  /* structure_initialization may be NULL ! */
    void *visit(initialized_structure_c *symbol);
  /* helper symbol for structure_declaration */
  /* structure_declaration:  STRUCT structure_element_declaration_list END_STRUCT */
  /* structure_element_declaration_list structure_element_declaration ';' */
    void *visit(structure_element_declaration_list_c *symbol);
  /*  structure_element_name ':' *_spec_init */
    void *visit(structure_element_declaration_c *symbol);
  /* helper symbol for structure_initialization */
  /* structure_initialization: '(' structure_element_initialization_list ')' */
  /* structure_element_initialization_list ',' structure_element_initialization */
    void *visit(structure_element_initialization_list_c *symbol);
  /*  structure_element_name ASSIGN value */
    void *visit(structure_element_initialization_c *symbol);
  /*  string_type_name ':' elementary_string_type_name string_type_declaration_size string_type_declaration_init */
  /*
  SYM_REF4(string_type_declaration_c, string_type_name,
  					elementary_string_type_name,
  					string_type_declaration_size,
  					string_type_declaration_init) // may be == NULL!
  */
    void *visit(string_type_declaration_c *symbol);

  /* function_block_type_name ASSIGN structure_initialization */
  /* structure_initialization -> may be NULL ! */
    void *visit(fb_spec_init_c *symbol);
    
  /* REF_TO (non_generic_type_name | function_block_type_name) */
    void *visit(ref_spec_c *symbol);       // Defined in IEC 61131-3 v3
  /* ref_spec [ ASSIGN ref_initialization ]; *//* NOTE: ref_initialization may be NULL!! */
    void *visit(ref_spec_init_c *symbol);  // Defined in IEC 61131-3 v3
  /* identifier ':' ref_spec_init */
    void *visit(ref_type_decl_c *symbol);  // Defined in IEC 61131-3 v3

  /*****************************/
  /* B 1.5.2 - Function Blocks */
  /*****************************/
  /*  FUNCTION_BLOCK derived_function_block_name io_OR_other_var_declarations function_block_body END_FUNCTION_BLOCK */
  // SYM_REF3(function_block_declaration_c, fblock_name, var_declarations, fblock_body)
    void *visit(function_block_declaration_c *symbol);

  /*********************************************/
  /* B.1.6  Sequential function chart elements */
  /*********************************************/
  /* INITIAL_STEP step_name ':' action_association_list END_STEP */
  // SYM_REF2(initial_step_c, step_name, action_association_list)
    void *visit(initial_step_c *symbol);

  /* STEP step_name ':' action_association_list END_STEP */
  // SYM_REF2(step_c, step_name, action_association_list)
    void *visit(step_c *symbol);


}; // search_base_type_c




