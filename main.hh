/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */



#ifndef _MAIN_HH
#define _MAIN_HH



/* Compiler options, specified at runtime on the command line */

typedef struct {
   /* options specific to stage1_2 */
	bool allow_void_datatype;      /* Allow declaration of functions returning VOID  */
	bool allow_missing_var_in;     /* Allow definition and invocation of POUs with no input, output and in_out parameters! */
	bool disable_implicit_en_eno;  /* Disable the generation of implicit EN and ENO parameters on functions and Function Blocks */
	bool pre_parsing;              /* Support forward references (Run a pre-parsing phase before the defintive parsing phase that builds the AST) */
	bool safe_extensions;          /* support SAFE_* datatypes defined in PLCOpen TC5 "Safety Software Technical Specification - Part 1" v1.0 */
	bool full_token_loc;           /* error messages specify full token location */
	bool conversion_functions;     /* Create a conversion function for derived datatype */
	bool nested_comments;          /* Allow the use of nested comments. */
	bool ref_standard_extensions;  /* Allow the use of REFerences (keywords REF_TO, REF, DREF, ^, NULL). */
	bool ref_nonstand_extensions;  /* Allow the use of non-standard extensions to REF_TO datatypes: REF_TO ANY, and REF_TO in struct elements! */
	bool nonliteral_in_array_size; /* Allow the use of constant non-literals when specifying size of arrays (ARRAY [1..max] OF INT) */
	const char *includedir;        /* Include directory, where included files will be searched for... */
	
   /* options specific to stage3 */
	bool relaxed_datatype_model;   /* Use the relaxed datatype equivalence model, instead of the default strict equivalence model */
} runtime_options_t;

extern runtime_options_t runtime_options;






 /* Function used throughout the code --> used to report failed assertions (i.e. internal compiler errors)! */
#include <stddef.h>  /* required for NULL */
 
#define ERROR               error_exit(__FILE__,__LINE__)
#define ERROR_MSG(msg, ...) error_exit(__FILE__,__LINE__, msg, ## __VA_ARGS__)

extern void error_exit(const char *file_name, int line_no, const char *errmsg = NULL, ...);




 /* Get the definition of INT16_MAX, INT16_MIN, UINT64_MAX, INT64_MAX, INT64_MIN, ... */
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS /* required to have UINTxx_MAX defined when including stdint.h from C++ source code. */
#endif
#include <stdint.h>         
#include <limits>

#ifndef   UINT64_MAX 
  #define UINT64_MAX (std::numeric_limits< uint64_t >::max())
#endif
#ifndef    INT64_MAX 
  #define  INT64_MAX (std::numeric_limits<  int64_t >::max())
#endif
#ifndef    INT64_MIN
  #define  INT64_MIN (std::numeric_limits<  int64_t >::min()) 
#endif



/* Determine, for the current platform, which datas types (float, double or long double) use 64 and 32 bits. */
/* NOTE: We cant use sizeof() in pre-processor directives, so we have to do it another way... */
/* CURIOSITY: We can use sizeof() and offsetof() inside static_assert() but:
 *          - this only allows us to make assertions, and not #define new macros
 *          - is only available in the C standard [ISO/IEC 9899:2011] and the C++ 0X draft standard [Becker 2008]. It is not available in C99.
 *          https://www.securecoding.cert.org/confluence/display/seccode/DCL03-C.+Use+a+static+assertion+to+test+the+value+of+a+constant+expression
 *         struct {int a, b, c, d} header_t;
 *  e.g.:  static_assert(offsetof(struct header_t, c) == 8, "Compile time error message.");
 */

#include <float.h>
/*CHANGE:use typedef to replace #define */
#if    (LDBL_MANT_DIG == 53) /* NOTE: 64 bit IEC559 real has 53 bits for mantissa! */
  //#define real64_tX long_double /* so we can later use #if (real64_t == long_double) directives in the code! */
  //#define real64_t  long double /* NOTE: no underscore '_' between 'long' and 'double' */
  typedef long double real64_tX;
  typedef long double real64_t;
  #define REAL64_MAX  LDBL_MAX
#elif  ( DBL_MANT_DIG == 53) /* NOTE: 64 bit IEC559 real has 53 bits for mantissa! */
  // #define real64_tX double
  // #define real64_t  double
  typedef double real64_tX;
  typedef double real64_t;
  #define REAL64_MAX  DBL_MAX
#elif  ( FLT_MANT_DIG == 53) /* NOTE: 64 bit IEC559 real has 53 bits for mantissa! */
  // #define real64_tX float
  // #define real64_t  float
  typedef float real64_tX;
  typedef float real64_t;
  #define REAL64_MAX  FLT_MAX
#else 
  #error Could not find a 64 bit floating point data type on this platform. Aborting...
#endif


#if    (LDBL_MANT_DIG == 24) /* NOTE: 32 bit IEC559 real has 24 bits for mantissa! */
  // #define real32_tX long_double /* so we can later use #if (real32_t == long_double) directives in the code! */
  // #define real32_t  long double /* NOTE: no underscore '_' between 'long' and 'double' */
  typedef long double real32_tX;
  typedef long double real32_t;
  #define REAL32_MAX  LDBL_MAX
#elif  ( DBL_MANT_DIG == 24) /* NOTE: 32 bit IEC559 real has 24 bits for mantissa! */
  // #define real32_tX double
  // #define real32_t  double
  typedef double real32_tX;
  typedef double real32_t;
  #define REAL32_MAX  DBL_MAX
#elif  ( FLT_MANT_DIG == 24) /* NOTE: 32 bit IEC559 real has 24 bits for mantissa! */
  // #define real32_tX float
  // #define real32_t  float
  typedef float real32_tX;
  typedef float real32_t;
  #define REAL32_MAX  FLT_MAX
#else 
  #error Could not find a 32 bit floating point data type on this platform. Aborting...
#endif



#include <math.h>
#ifndef INFINITY
  #error Could not find the macro that defines the value for INFINITY in the current platform.
#endif
#ifndef NAN
  #error Could not find the macro that defines the value for NAN in the current platform.
#endif



/* get the printf format macros for printing variables of fixed data size
 * e.g.  int64_t v; printf("value=%"PRId64" !!\n", v);
 * e.g. uint64_t v; printf("value=%"PRIu64" !!\n", v);
 * e.g. uint64_t v; printf("value=%"PRIx64" !!\n", v);  // hexadecimal format
 */
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>


#endif // #ifndef _MAIN_HH
