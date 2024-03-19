/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * VISITOR.CC
 *
 * Three base implementations of the visitor interface,
 * that may be later extended to execute a particular algorithm.
 *
 * The null (class null_visitor_c) does nothing.
 *
 * The iterator (class iterator_visitor_c) iterates through
 * every object in the syntax tree.
 *
 * The search class (class search_visitor_c) iterates through
 * every object, until one returns a value != NULL.
 */





#include <unistd.h>

#include <stdio.h>  /* required for NULL */
#include "visitor.hh"

#include <iostream>


/******************/
/* visitor_c      */
/******************/

visitor_c::~visitor_c(void) {return;}


/******************/
/* null_visitor_c */
/******************/

null_visitor_c::~null_visitor_c(void) {return;}

#define SYM_LIST(class_name_c, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_TOKEN(class_name_c, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF0(class_name_c, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF1(class_name_c, ref1, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF2(class_name_c, ref1, ref2, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF3(class_name_c, ref1, ref2, ref3, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF4(class_name_c, ref1, ref2, ref3, ref4, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF5(class_name_c, ref1, ref2, ref3, ref4, ref5, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF6(class_name_c, ref1, ref2, ref3, ref4, ref5, ref6, ...)	\
  void *null_visitor_c::visit(class_name_c *symbol) {return NULL;}


#include "absyntax.def"




#undef SYM_LIST
#undef SYM_TOKEN
#undef SYM_REF0
#undef SYM_REF1
#undef SYM_REF2
#undef SYM_REF3
#undef SYM_REF4
#undef SYM_REF5
#undef SYM_REF6







/*******************/
/* fcall_visitor_c */
/*******************/

//fcall_visitor_c::~fcall_visitor_c(void) {return;};


// void fcall_visitor_c::fcall(symbol_c *symbol) {return;};

#define VISIT_METHOD {fcall(symbol); return NULL;}

#define SYM_LIST(class_name_c, ...)                                             void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_TOKEN(class_name_c, ...)                                            void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF0(class_name_c, ...)                                             void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF1(class_name_c, ref1, ...)                                       void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF2(class_name_c, ref1, ref2, ...)                                 void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF3(class_name_c, ref1, ref2, ref3, ...)                           void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF4(class_name_c, ref1, ref2, ref3, ref4, ...)                     void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF5(class_name_c, ref1, ref2, ref3, ref4, ref5, ...)               void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF6(class_name_c, ref1, ref2, ref3, ref4, ref5, ref6, ...)         void *fcall_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;

#include "../absyntax/absyntax.def"

#undef VISIT_METHOD

#undef SYM_LIST
#undef SYM_TOKEN
#undef SYM_REF0
#undef SYM_REF1
#undef SYM_REF2
#undef SYM_REF3
#undef SYM_REF4
#undef SYM_REF5
#undef SYM_REF6






/**********************/
/* iterator_visitor_c */
/**********************/

iterator_visitor_c::~iterator_visitor_c(void) {return;}


void *iterator_visitor_c::visit_list(list_c *list) {
  for(int i = 0; i < list->n; i++) {
    list->get_element(i)->accept(*this);
  }
  return NULL;
}


#define SYM_LIST(class_name_c, ...)	\
  void *iterator_visitor_c::visit(class_name_c *symbol) {return visit_list(symbol);}

#define SYM_TOKEN(class_name_c, ...)	\
  void *iterator_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF0(class_name_c, ...)	\
  void *iterator_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF1(class_name_c, ref1, ...)			\
void *iterator_visitor_c::visit(class_name_c *symbol) {	\
  if (symbol->ref1!=NULL) symbol->ref1->accept(*this);	\
  return NULL;						\
}

#define SYM_REF2(class_name_c, ref1, ref2, ...)		\
void *iterator_visitor_c::visit(class_name_c *symbol) {	\
  if (symbol->ref1!=NULL) symbol->ref1->accept(*this);	\
  if (symbol->ref2!=NULL) symbol->ref2->accept(*this);	\
  return NULL;						\
}

#define SYM_REF3(class_name_c, ref1, ref2, ref3, ...)			\
void *iterator_visitor_c::visit(class_name_c *symbol) {			\
  if (symbol->ref1) symbol->ref1->accept(*this);			\
  if (symbol->ref2) symbol->ref2->accept(*this);			\
  if (symbol->ref3) symbol->ref3->accept(*this);			\
  return NULL;								\
}

#define SYM_REF4(class_name_c, ref1, ref2, ref3, ref4, ...)		\
void *iterator_visitor_c::visit(class_name_c *symbol) {			\
  if (symbol->ref1) symbol->ref1->accept(*this);			\
  if (symbol->ref2) symbol->ref2->accept(*this);			\
  if (symbol->ref3) symbol->ref3->accept(*this);			\
  if (symbol->ref4) symbol->ref4->accept(*this);			\
  return NULL;								\
}

#define SYM_REF5(class_name_c, ref1, ref2, ref3, ref4, ref5, ...)	\
void *iterator_visitor_c::visit(class_name_c *symbol) {			\
  if (symbol->ref1) symbol->ref1->accept(*this);			\
  if (symbol->ref2) symbol->ref2->accept(*this);			\
  if (symbol->ref3) symbol->ref3->accept(*this);			\
  if (symbol->ref4) symbol->ref4->accept(*this);			\
  if (symbol->ref5) symbol->ref5->accept(*this);			\
  return NULL;								\
}

#define SYM_REF6(class_name_c, ref1, ref2, ref3, ref4, ref5, ref6, ...)	\
void *iterator_visitor_c::visit(class_name_c *symbol) {			\
  if (symbol->ref1) symbol->ref1->accept(*this);			\
  if (symbol->ref2) symbol->ref2->accept(*this);			\
  if (symbol->ref3) symbol->ref3->accept(*this);			\
  if (symbol->ref4) symbol->ref4->accept(*this);			\
  if (symbol->ref5) symbol->ref5->accept(*this);			\
  if (symbol->ref6) symbol->ref6->accept(*this);			\
  return NULL;								\
}



#include "absyntax.def"



#undef SYM_LIST
#undef SYM_TOKEN
#undef SYM_REF0
#undef SYM_REF1
#undef SYM_REF2
#undef SYM_REF3
#undef SYM_REF4
#undef SYM_REF5
#undef SYM_REF6





/****************************/
/* fcall_iterator_visitor_c */
/****************************/

fcall_iterator_visitor_c::~fcall_iterator_visitor_c(void) {return;};

void fcall_iterator_visitor_c::prefix_fcall(symbol_c *symbol) {return;};
void fcall_iterator_visitor_c::suffix_fcall(symbol_c *symbol) {return;};

#define VISIT_METHOD {\
  prefix_fcall(symbol);                \
  iterator_visitor_c::visit(symbol);   \
  suffix_fcall(symbol);                \
  return NULL;                         \
}

#define SYM_LIST(class_name_c, ...)                                             void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_TOKEN(class_name_c, ...)                                            void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF0(class_name_c, ...)                                             void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF1(class_name_c, ref1, ...)                                       void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF2(class_name_c, ref1, ref2, ...)                                 void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF3(class_name_c, ref1, ref2, ref3, ...)                           void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF4(class_name_c, ref1, ref2, ref3, ref4, ...)                     void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF5(class_name_c, ref1, ref2, ref3, ref4, ref5, ...)               void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;
#define SYM_REF6(class_name_c, ref1, ref2, ref3, ref4, ref5, ref6, ...)         void *fcall_iterator_visitor_c::visit(class_name_c *symbol) VISIT_METHOD;

#include "../absyntax/absyntax.def"

#undef VISIT_METHOD

#undef SYM_LIST
#undef SYM_TOKEN
#undef SYM_REF0
#undef SYM_REF1
#undef SYM_REF2
#undef SYM_REF3
#undef SYM_REF4
#undef SYM_REF5
#undef SYM_REF6








/********************/
/* search_visitor_c */
/********************/

search_visitor_c::~search_visitor_c(void) {return;}


void *search_visitor_c::visit_list(list_c *list) {
  for(int i = 0; i < list->n; i++) {
    void *res = list->get_element(i)->accept(*this);
    if (res != NULL)
      return res;
  }
  return NULL;
}


#define SYM_LIST(class_name_c, ...)	\
  void *search_visitor_c::visit(class_name_c *symbol) {return visit_list(symbol);}

#define SYM_TOKEN(class_name_c, ...)	\
  void *search_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF0(class_name_c, ...)	\
  void *search_visitor_c::visit(class_name_c *symbol) {return NULL;}

#define SYM_REF1(class_name_c, ref1, ...)				\
void *search_visitor_c::visit(class_name_c *symbol) {		\
  if (symbol->ref1) return symbol->ref1->accept(*this);		\
  return NULL;							\
}

#define SYM_REF2(class_name_c, ref1, ref2, ...)			\
void *search_visitor_c::visit(class_name_c *symbol) {		\
  void *res = NULL;						\
  if (symbol->ref1) res =  symbol->ref1->accept(*this);		\
  if (res != NULL)  return res;					\
  if (symbol->ref2) return symbol->ref2->accept(*this);		\
  return NULL;							\
}

#define SYM_REF3(class_name_c, ref1, ref2, ref3, ...)		\
void *search_visitor_c::visit(class_name_c *symbol) {		\
  void *res = NULL;						\
  if (symbol->ref1) res =  symbol->ref1->accept(*this);		\
  if (res != NULL)  return res;					\
  if (symbol->ref2) res =  symbol->ref2->accept(*this);		\
  if (res != NULL)  return res;					\
  if (symbol->ref3) return symbol->ref3->accept(*this);		\
  return NULL;							\
}

#define SYM_REF4(class_name_c, ref1, ref2, ref3, ref4, ...)		\
void *search_visitor_c::visit(class_name_c *symbol) {		\
  void *res = NULL;						\
  if (symbol->ref1) res =  symbol->ref1->accept(*this);		\
  if (res != NULL)  return res;					\
  if (symbol->ref2) res =  symbol->ref2->accept(*this);		\
  if (res != NULL)  return res;					\
  if (symbol->ref3) res =  symbol->ref3->accept(*this);		\
  if (res != NULL)  return res;					\
  if (symbol->ref4) return symbol->ref4->accept(*this);		\
  return NULL;							\
}

#define SYM_REF5(class_name_c, ref1, ref2, ref3, ref4, ref5, ...)		\
void *search_visitor_c::visit(class_name_c *symbol) {			\
  void *res = NULL;							\
  if (symbol->ref1) res =  symbol->ref1->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref2) res =  symbol->ref2->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref3) res =  symbol->ref3->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref4) res =  symbol->ref4->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref5) return symbol->ref5->accept(*this);			\
  return NULL;								\
}

#define SYM_REF6(class_name_c, ref1, ref2, ref3, ref4, ref5, ref6, ...)	\
void *search_visitor_c::visit(class_name_c *symbol) {			\
  void *res = NULL;							\
  if (symbol->ref1) res =  symbol->ref1->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref2) res =  symbol->ref2->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref3) res =  symbol->ref3->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref4) res =  symbol->ref4->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref5) res =  symbol->ref5->accept(*this);			\
  if (res != NULL)  return res;						\
  if (symbol->ref6) return symbol->ref6->accept(*this);			\
  return NULL;								\
}



#include "absyntax.def"



#undef SYM_LIST
#undef SYM_TOKEN
#undef SYM_REF0
#undef SYM_REF1
#undef SYM_REF2
#undef SYM_REF3
#undef SYM_REF4
#undef SYM_REF5
#undef SYM_REF6












