/*
 * (c) 2003 Mario de Sousa
 *
 * Offered to the public under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * This code is made available on the understanding that it will not be
 * used in safety-critical situations without a full and competent review.
 */

/*
 * An IEC 61131-3 IL and ST compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */

/* Determine the size, in bits, of the data type.
 * 
 * NOTE: Currently, only elementary data types with well defined sizes (in the standard) are supported.
 *       - derived data types are not supported, and these will return 0
 *       - TIME, DATE, TIME_OF_DAY, and DATE_AND_TIME are not supported, and will return 0
 *       - STRING and WSTRING are not supported, and the standard merely defines bit per character,
 *              and not the maximum number of characters, so these will return 0
 *
 *       We also support the 'Numeric Literals' Data types.
 *       i.e., numeric literals are considerd basic data types
 *       as their data type is undefined (e.g. the datat type of '30'
 *       could be 'INT' or 'SINT' or 'LINT' or 'USINT' or ...
 *       NOTE: for base 10 numeric literals, any number taking up more than 64 bits
 *             will only return a bitsize of 1024!
 *
 *       For numeric literals, we return the minimum number of bits
 *       required to store the value.
 *
 * E.g. TYPE new_int_t : INT; END_TYPE;
 *      TYPE new_int2_t : INT = 2; END_TYPE;
 *      TYPE new_subr_t : INT (4..5); END_TYPE;
 *
 *    sizeof(SINT) ->  8
 *    sizeof(INT)  -> 16
 *    sizeof(DINT) -> 32
 *    sizeof(LINT) -> 64
 *
 *    sizeof('1')       ->  1
 *    sizeof('015')     ->  4    # Leading zeros are ignored!
 *    sizeof('0')       ->  1    # This is a special case! Even the value 0 needs at least 1 bit to store!
 *    sizeof('16')      ->  5
 *    sizeof('2#00101') ->  3
 *    sizeof('8#334')   ->  9
 *    sizeof('16#2A')   ->  8
 *
 *    sizeof('7.4')     ->  32   # all real literals return 32 bits, the size of a 'REAL'
 *                               # TODO: study IEC 60559 for the range of values that may be
 *                               #       stored in a REAL (basic single width floating point format)
 *                               #       and in a LREAL (basic double width floating point format)
 *                               #       and see if some real literals need to return 64 instead!
 */

#include "get_sizeof_datatype.hh"

#include <stdlib.h>
#include <string.h>
#include <limits.h>  // get definition of ULLONG_MAX
/* tell stdint.h we want the definition of UINT64_MAX */
#define __STDC_LIMIT_MACROS
#include <stdint.h>  // get definition of uint64_t and UINT64_MAX


#define ERROR error_exit(__FILE__,__LINE__)
/* function defined in main.cc */
extern void error_exit(const char *file_name, int line_no);


/* This class is a singleton.
 * So we need a pointer to the singe instance...
 */
get_sizeof_datatype_c *get_sizeof_datatype_c::singleton = NULL;


#define _encode_int(value)   ((void *)(((char *)NULL) + value))
#define _decode_int(ptr)     (((char *)ptr) - ((char *)NULL))




/* divide a base 10 literal in a string by 2 */
/* returns remainder of division (0 or 1)    */
static int strdivby2(char **strptr) {
  char *str = *strptr;
  int carry = 0;

  while (*str != '\0') {
    /* Assumes ASCII */
    int newcarry;
//     newcarry = ((*str-'0') mod 2);
    newcarry = ((*str-'0') - ((*str-'0')/2)*2);
    *str = (((*str-'0') + 10*carry)/2) + '0';
    carry = newcarry;
    str++;
  }

  /* ignore leading zeros in result... */
  while (**strptr == '0') 
    (*strptr)++;

  return carry;
}


/* Constructor for the singleton class */
int get_sizeof_datatype_c::getsize(symbol_c *data_type_symbol) {
      if (NULL == singleton) {
        singleton = new get_sizeof_datatype_c;
        if (NULL == singleton)
          ERROR;
      }
      return _decode_int(data_type_symbol->accept(*singleton));
    }

/* Destructor for the singleton class */
get_sizeof_datatype_c::~get_sizeof_datatype_c(void) {
      if (NULL != singleton) delete singleton;
      singleton = NULL;
    }


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

/* NOTE: all integer and real literal tokens will always be positive (i.e. no leading '-')
 * due to the way the source code is parsed by iec.flex.
 */
void *get_sizeof_datatype_c::visit(real_c *symbol) {
  return _encode_int(32);
}

/* NOTE: all integer and real literal tokens will always be positive (i.e. no leading '-')
 * due to the way the source code is parsed by iec.flex.
 */
void *get_sizeof_datatype_c::visit(integer_c *symbol) {
  int bitsize = 0;

  if (NULL ==   symbol->value ) ERROR;
  if ('\0' == *(symbol->value)) ERROR;

#if 0
  char *endptr;
  /* Convert the string to an unsigned 64 bit integer */
  /* We can use strtoull(), but we are not guaranteed that an unsigned long long int
   * is 64 bits wide. First make sure that it is...
   *
   * We could also use the strtouq() instead, which converts
   * to a quad word (64 bits). However, this requires either GCC or BSD extensions.
   */
  #ifdef strtoull // this ifdef does not work!!
    /* we have long long int, use it... */
    #define ival_MAX ULLONG_MAX
    unsigned long long int ival = 0;
    ival = strtoull (symbol->value, &endptr, 10 /* base */);
  #else
    /* use long int ... */
    #define ival_MAX ULONG_MAX
    unsigned long int ival = 0;
    ival = strtoul (symbol->value, &endptr, 10 /* base */);
  #endif

  #if (ival_MAX < UINT64_MAX)
  #error Largest strtoint() conversion function converts to an int less than 64 bits wide!
  #endif

  if (NULL ==  endptr) ERROR;
  if ('\0' != *endptr) ERROR;
  // TODO: return _encode_int(1024) if value takes up more than 64 bits! 

  /* determine the number of bits used... */
  for (bitsize = 0; ival > 0; ival /= 2, bitsize++);

  /* special case... if (value == 0) <=> (bitsize == 0), return bit size of 1 ! */
  if (0 == bitsize) bitsize = 1;

  return _encode_int(bitsize);
#endif
  /* We could first convert from string to some kind of integer, and then
   * determine the the bitsize using integer aritmetic.
   * However, we are then limited to the bit size of the widest available integer
   * (usually 64 bits), which is not good at all!
   */
  /* Let's try to determine bitsize by converting directly from the string!! */
  char *sval = strdup(symbol->value);
  char *oval = sval;
  if (NULL ==  sval) ERROR;
  if ('\0' == *sval) ERROR;

  for (bitsize = 0; *sval != '\0'; strdivby2(&sval), bitsize ++);

  /* Even for (value == 0), the above loop will return bitsize == 1!, 
   * so we don't need to handle the special case...
   */
  /* special case... if (value == 0) <=> (bitsize == 0), return bit size of 1 ! */
  // if (0 == bitsize) bitsize = 1;

  free(oval);
  return _encode_int(bitsize);
}


/* NOTE: all integer and real literal tokens will always be positive (i.e. no leading '-')
 * due to the way the source code is parsed by iec.flex.
 */
void *get_sizeof_datatype_c::visit(binary_integer_c *symbol) {
  const char *sval = symbol->value;
  int bitsize = 0;

  /* first 2 characters had better be "2#" ! */
  if (NULL ==  sval) ERROR;
  if ('\0' == *sval) ERROR;
  if ( '2' != *sval) ERROR;
  sval++;
  if ('\0' == *sval) ERROR;
  if ( '#' != *sval) ERROR;
  sval++;
  if ('\0' == *sval) ERROR;

  /* determine the number of bits used... */
  for (bitsize = 0; '\0' != *sval; sval++, bitsize++) {
    /* consistency check: make sure we only have binary digits! */
    if (('0' != *sval) && ('1' != *sval))
      ERROR;
  }

  /* special case... if (value == 0) <=> (bitsize == 0), return bit size of 1 ! */
  if (0 == bitsize) bitsize = 1;

  return _encode_int(bitsize);
}


/* NOTE: all integer and real literal tokens will always be positive (i.e. no leading '-')
 * due to the way the source code is parsed by iec.flex.
 */
void *get_sizeof_datatype_c::visit(octal_integer_c *symbol) {
  const char *sval = symbol->value;
  int bitsize = 0;

  /* first 2 characters had better be "8#" ! */
  if (NULL ==  sval) ERROR;
  if ('\0' == *sval) ERROR;
  if ( '8' != *sval) ERROR;
  sval++;
  if ('\0' == *sval) ERROR;
  if ( '#' != *sval) ERROR;
  sval++;
  if ('\0' == *sval) ERROR;

  /* determine the number of bits used... */
  for (bitsize = 0; '\0' != *sval; sval++, bitsize += 3 /* 3 bits per octal digit */) {
    /* consistency check: make sure we only have octal digits! */
    /* Assumes ASCII */
    if (('0' > *sval) || ('7' < *sval))
      ERROR;
  }

  /* special case... if (value == 0) <=> (bitsize == 0), return bit size of 1 ! */
  if (0 == bitsize) bitsize = 1;

  return _encode_int(bitsize);
}


/* NOTE: all integer and real literal tokens will always be positive (i.e. no leading '-')
 * due to the way the source code is parsed by iec.flex.
 */
void *get_sizeof_datatype_c::visit(hex_integer_c *symbol) {
  const char *sval = symbol->value;
  int bitsize = 0;

  /* first 3 characters had better be "16#" ! */
  if (NULL ==  sval) ERROR;
  if ('\0' == *sval) ERROR;
  if ( '1' != *sval) ERROR;
  sval++;
  if ('\0' == *sval) ERROR;
  if ( '6' != *sval) ERROR;
  sval++;
  if ('\0' == *sval) ERROR;
  if ( '#' != *sval) ERROR;
  sval++;
  if ('\0' == *sval) ERROR;

  /* determine the number of bits used... */
  for (bitsize = 0; '\0' != *sval; sval++, bitsize += 4 /* 4 bits per hex digit */) {
    /* consistency check: make sure we only have hex digits! */
    /* Assumes ASCII */
    if (!(('0' <= *sval) && ('9' >= *sval)) && 
        !(('A' <= *sval) && ('F' >= *sval)) &&
        !(('a' <= *sval) && ('b' >= *sval)))
      ERROR;
  }

  /* special case... if (value == 0) <=> (bitsize == 0), return bit size of 1 ! */
  if (0 == bitsize) bitsize = 1;

  return _encode_int(bitsize);
}


/***********************************/
/* B 1.3.1 - Elementary Data Types */
/***********************************/
// void *get_sizeof_datatype_c::visit(time_type_name_c *symbol)	{return _encode_int(0); }
void *get_sizeof_datatype_c::visit(bool_type_name_c *symbol)	{return _encode_int(1); }
void *get_sizeof_datatype_c::visit(sint_type_name_c *symbol)	{return _encode_int(8); }
void *get_sizeof_datatype_c::visit(int_type_name_c *symbol)	{return _encode_int(16);}
void *get_sizeof_datatype_c::visit(dint_type_name_c *symbol)	{return _encode_int(32);}
void *get_sizeof_datatype_c::visit(lint_type_name_c *symbol)	{return _encode_int(64);}
void *get_sizeof_datatype_c::visit(usint_type_name_c *symbol)	{return _encode_int(8); }
void *get_sizeof_datatype_c::visit(uint_type_name_c *symbol)	{return _encode_int(16);}
void *get_sizeof_datatype_c::visit(udint_type_name_c *symbol)	{return _encode_int(32);}
void *get_sizeof_datatype_c::visit(ulint_type_name_c *symbol)	{return _encode_int(64);}
void *get_sizeof_datatype_c::visit(real_type_name_c *symbol)	{return _encode_int(32);}
void *get_sizeof_datatype_c::visit(lreal_type_name_c *symbol)	{return _encode_int(64);}
// void *get_sizeof_datatype_c::visit(date_type_name_c *symbol)	{return _encode_int(0); }
// void *get_sizeof_datatype_c::visit(tod_type_name_c *symbol)	{return _encode_int(0); }
// void *get_sizeof_datatype_c::visit(dt_type_name_c *symbol)	{return _encode_int(0); }
void *get_sizeof_datatype_c::visit(byte_type_name_c *symbol)	{return _encode_int(8); }
void *get_sizeof_datatype_c::visit(word_type_name_c *symbol)	{return _encode_int(16);}
void *get_sizeof_datatype_c::visit(dword_type_name_c *symbol)	{return _encode_int(32);}
void *get_sizeof_datatype_c::visit(lword_type_name_c *symbol)	{return _encode_int(64);}
// void *get_sizeof_datatype_c::visit(string_type_name_c *symbol)	{return _encode_int(0); }
// void *get_sizeof_datatype_c::visit(wstring_type_name_c *symbol)	{return _encode_int(0); }
/******************************************************/
/* Extensions to the base standard as defined in      */
/* "Safety Software Technical Specification,          */
/*  Part 1: Concepts and Function Blocks,             */
/*  Version 1.0 – Official Release"                   */
/* by PLCopen - Technical Committee 5 - 2006-01-31    */
/******************************************************/
void *get_sizeof_datatype_c::visit(safebool_type_name_c *symbol)	{return _encode_int(1);}

/********************************/
/* B 1.3.3 - Derived data types */
/********************************/
// Not yet supported...