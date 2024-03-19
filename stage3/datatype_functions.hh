#ifndef _HELPER_FUNCTIONS_HH_
#define _HELPER_FUNCTIONS_HH_

#include "../absyntax/visitor.hh"
#include <typeinfo>





typedef struct {
  symbol_c *function_name;
  symbol_c *nonformal_operand_list;
  symbol_c *   formal_operand_list;

  enum {POU_FB, POU_function} POU_type;
//symbol_c &*datatype;
//std::vector <symbol_c *> &candidate_datatypes;
  std::vector <symbol_c *> &candidate_functions;
  symbol_c *&called_function_declaration;
  int      &extensible_param_count;
} generic_function_call_t;








/* Widening Primitive Conversion */
struct widen_entry {
	symbol_c *left;
	symbol_c *right;
	symbol_c *result;
	enum {ok, deprecated} status;
};
/*
 * 2.5.1.5.6 Functions of time data types
 * Table 30 - page 64
 */
extern const struct widen_entry widen_ADD_table[];
extern const struct widen_entry widen_SUB_table[];
extern const struct widen_entry widen_MUL_table[];
extern const struct widen_entry widen_DIV_table[];
extern const struct widen_entry widen_MOD_table[];
extern const struct widen_entry widen_EXPT_table[];
extern const struct widen_entry widen_AND_table[];
extern const struct widen_entry widen_OR_table[];
extern const struct widen_entry widen_XOR_table[];
extern const struct widen_entry widen_CMP_table[];

/* Search for a datatype inside a candidate_datatypes list.
 * Returns: position of datatype in the list, or -1 if not found.
 */
int search_in_candidate_datatype_list(symbol_c *datatype, const std::vector <symbol_c *> &candidate_datatypes);

/* Remove a datatype inside a candidate_datatypes list.
 * Returns: If successful it returns true, false otherwise.
 */
bool remove_from_candidate_datatype_list(symbol_c *datatype, std::vector <symbol_c *> &candidate_datatypes);

/* Intersect two candidate_datatype_lists.
 * Remove from list1 (origin, dest.) all elements that are not found in list2 (with).
 * In essence, list1 will contain the result of the intersection of list1 with list2.
 * In other words, modify list1 so it only contains the elelements that are simultaneously in list1 and list2!
 */
void intersect_candidate_datatype_list(symbol_c *list1 /*origin, dest.*/, symbol_c *list2 /*with*/);

/* intersect the candidate_datatype lists of all prev_il_intructions, and set the local candidate_datatype list to the result! */
void intersect_prev_candidate_datatype_lists(il_instruction_c *symbol);





#endif /* _HELPER_FUNCTIONS_HH_ */
