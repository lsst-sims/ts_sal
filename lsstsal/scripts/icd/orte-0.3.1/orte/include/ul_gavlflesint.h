#ifndef _UL_GAVLFLESINT_H
#define _UL_GAVLFLESINT_H

#include "ul_gavl.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Declaration of tree with first/last enhanced speed functions with internal node */
#define GAVL_FLES_INT_DEC(cust_prefix, cust_root_t, cust_item_t, cust_key_t,\
		cust_root_field, cust_item_node, cust_item_key, cust_cmp_fnc) \
\
static inline cust_item_t * \
cust_prefix##_node2item(const cust_root_t *root, const gavl_node_t *node) \
  {return (cust_item_t*)((char*)node-(long)&((cust_item_t*)0)->cust_item_node);}\
\
static inline cust_key_t *\
cust_prefix##_node2key(const cust_root_t *root, gavl_node_t *node)\
  { return &(cust_prefix##_node2item(root, node)->cust_item_key);}\
\
void cust_prefix##_init_root_field(cust_root_t *root);\
int cust_prefix##_search_node(const cust_root_t *root, cust_key_t *key, gavl_node_t **nodep);\
cust_item_t *cust_prefix##_find(const cust_root_t *root, cust_key_t *key);\
cust_item_t *cust_prefix##_find_first(const cust_root_t *root, cust_key_t *key);\
cust_item_t *cust_prefix##_find_after(const cust_root_t *root, cust_key_t *key);\
int cust_prefix##_insert(cust_root_t *root, cust_item_t *item);\
cust_item_t *cust_prefix##_cut_first(cust_root_t *root);\
int cust_prefix##_delete_node(cust_root_t *root, gavl_node_t *node);\
int cust_prefix##_delete(cust_root_t *root, cust_item_t *item);\
static inline gavl_node_t *\
cust_prefix##_first_node(const cust_root_t *root)\
{\
  return root->cust_root_field.first;\
}\
static inline gavl_node_t *\
cust_prefix##_last_node(const cust_root_t *root)\
{\
  return root->cust_root_field.last;\
}\
static inline cust_item_t *\
cust_prefix##_first(const cust_root_t *root)\
{\
  gavl_node_t *n=cust_prefix##_first_node(root);\
  return n?cust_prefix##_node2item(root,n):NULL;\
}\
static inline cust_item_t *\
cust_prefix##_last(const cust_root_t *root)\
{\
  gavl_node_t *n=cust_prefix##_last_node(root);\
  return n?cust_prefix##_node2item(root,n):NULL;\
}\
static inline cust_item_t *\
cust_prefix##_next(const cust_root_t *root, cust_item_t *item)\
{\
  gavl_node_t *n=gavl_next_node(&item->cust_item_node);\
  return n?cust_prefix##_node2item(root,n):NULL;\
}\
static inline cust_item_t *\
cust_prefix##_prev(const cust_root_t *root, cust_item_t *item)\
{\
  gavl_node_t *n=gavl_prev_node(&item->cust_item_node);\
  return n?cust_prefix##_node2item(root,n):NULL;\
}\
static inline int \
cust_prefix##_is_empty(const cust_root_t *root)\
{\
  return !root->cust_root_field.root;\
}\
/*** Iterators ***/\
UL_ITBASE_SORT_DEC(cust_prefix, cust_root_t, cust_item_t, cust_key_t)


/**
 * GAVL_FLES_INT_IMP - Implementation of new custom tree with fast first/last functions
 * @cust_prefix:	defines prefix for builded function names 
 * @cust_root_t:	user defined structure type of root of the tree
 * @cust_item_t:	user defined structure type of items stored in the tree
 * @cust_key_t:		type of the key used for sorting of the items
 * @cust_root_field:	the field of the root structure pointing to the tree root node 
 * @cust_item_node:	the field of item structure used for chaining of items
 * @cust_item_key:	the key field of item structure defining order of items
 * @cust_cmp_fnc:	the keys compare function 
 *
 * This version of custom tree implementation allows multiple items with same
 * key value to be stored in tree.
 * There are two macros designed for building custom AVL trees. The macro
 * %GAVL_CUST_NODE_INT_DEC declares functions for custom tree manipulations
 * and is intended for use in header files.
 * The macro %GAVL_CUST_NODE_INT_REP_IMP builds implementations for non-inlined
 * functions declared by %GAVL_CUST_NODE_INT_DEC. The @cust_cmp_fnc is used
 * for comparison of item keys in the search and insert functions. The types
 * of two input arguments of @cust_cmp_fnc functions must correspond 
 * with @cust_key_t type. Return value should be positive for case when
 * the first pointed key value is greater then second, negative for reverse
 * case and zero for equal pointed values.
 */
#define GAVL_FLES_INT_IMP(cust_prefix, cust_root_t, cust_item_t, cust_key_t,\
		cust_root_field, cust_item_node, cust_item_key, cust_cmp_fnc,\
		cust_ins_fl, cust_first_change, cust_last_change, cust_empty_state) \
\
void cust_prefix##_init_root_field(cust_root_t *root)\
{\
  root->cust_root_field.root=NULL;\
  root->cust_root_field.first=NULL;\
  root->cust_root_field.last=NULL;\
  root->cust_root_field.count=0;\
}\
\
int cust_prefix##_search_node4(const cust_root_t *root, cust_key_t *key, gavl_node_t **nodep, int mode)\
{\
  int cmp=1;\
  gavl_node_t *n, *p;\
  n=p=root->cust_root_field.root;\
  while(n){\
    cmp=cust_cmp_fnc(cust_prefix##_node2key(root,n),key);\
    p=n;\
    if(cmp>0){\
      n=n->left;\
    }else if((cmp<0)||(mode&GAVL_FAFTER)){\
      n=n->right;\
    }else{\
      break;\
    }\
  }\
  if(!cmp && (mode&GAVL_FFIRST)){\
    while((n=p->left)){\
      cmp=cust_cmp_fnc(cust_prefix##_node2key(root,n),key);\
      if(!cmp){\
        p=n;\
      }else{\
        while((n=n->right)){\
          cmp=cust_cmp_fnc(cust_prefix##_node2key(root,n),key);\
          if(!cmp){\
            p=n;\
            break;\
          }\
        }\
        if(cmp) break;\
      }\
    }\
    cmp=0;\
  }\
  *nodep=p;\
  return cmp;\
}\
\
int cust_prefix##_search_node(const cust_root_t *root, cust_key_t *key, gavl_node_t **nodep)\
{\
  return cust_prefix##_search_node4(root, key, nodep, 0);\
}\
\
cust_item_t *cust_prefix##_find(const cust_root_t *root, cust_key_t *key)\
{\
  gavl_node_t *node;\
  if(cust_prefix##_search_node4(root, key, &node, 0))\
    return NULL;\
  return cust_prefix##_node2item(root,node);\
}\
\
cust_item_t *cust_prefix##_find_first(const cust_root_t *root, cust_key_t *key)\
{\
  gavl_node_t *n;\
  if(cust_prefix##_search_node4(root, key, &n, GAVL_FFIRST))\
    return NULL;\
  return cust_prefix##_node2item(root,n);\
}\
\
cust_item_t *cust_prefix##_find_after(const cust_root_t *root, cust_key_t *key)\
{\
  gavl_node_t *node;\
  if(cust_prefix##_search_node4(root, key, &node, GAVL_FAFTER)<=0){\
     if(node) node=gavl_next_node(node);\
  }\
  return node?cust_prefix##_node2item(root,node):NULL;\
}\
\
int cust_prefix##_insert(cust_root_t *root, cust_item_t *item)\
{\
  int cmp;\
  gavl_node_t *where, *n2add;\
  \
  cmp=cust_prefix##_search_node4(root, &item->cust_item_key, &where, cust_ins_fl);\
  if(!cmp && !(cust_ins_fl&GAVL_FAFTER)) return -1;\
  n2add=&item->cust_item_node;\
  if(!root->cust_root_field.root){\
    root->cust_root_field.first=root->cust_root_field.last=n2add;\
    cust_first_change; cust_last_change;\
  }else if((cmp>0)&&(where==root->cust_root_field.first)){\
    root->cust_root_field.first=n2add;\
    cust_first_change;\
  }else if((cmp<=0)&&(where==root->cust_root_field.last)){\
    root->cust_root_field.last=n2add;\
    cust_last_change;\
  }\
  root->cust_root_field.count++;\
  return gavl_insert_primitive_at(&root->cust_root_field.root, n2add, where, cmp);\
}\
\
int cust_prefix##_delete_node(cust_root_t *root, gavl_node_t *node)\
{\
  gavl_node_t *p;\
  /*check if node is inserted into tree*/\
  for(p=node; p->parent; p=p->parent);\
  if(p!=root->cust_root_field.root)\
    return -1;\
  if(root->cust_root_field.first==node){\
    if(root->cust_root_field.last==node){\
      root->cust_root_field.first=root->cust_root_field.last=NULL;\
      cust_empty_state;\
    }else{\
      root->cust_root_field.first=gavl_next_node(node);\
      cust_first_change;\
    }\
  }else if(root->cust_root_field.last==node){\
    root->cust_root_field.last=gavl_prev_node(node);\
    cust_last_change;\
  }\
  root->cust_root_field.count--;\
  return gavl_delete_primitive(&root->cust_root_field.root, node);\
}\
\
int cust_prefix##_delete(cust_root_t *root, cust_item_t *item)\
{\
  gavl_node_t *n;\
  if(!item) return -1;\
  n=&item->cust_item_node;\
  return cust_prefix##_delete_node(root, n);\
}\
\
cust_item_t *cust_prefix##_cut_first(cust_root_t *root)\
{\
  gavl_node_t *n, *p;\
  gavl_node_t **np=&root->cust_root_field.root;\
  if(!(n=root->cust_root_field.first))\
    return NULL;\
  if(n->parent) np=&n->parent->left;\
  if((*np=n->right)){\
    p=n->right;\
    p->parent=n->parent;\
    while(p->left) p=p->left;\
    root->cust_root_field.first=p;\
    cust_first_change;\
  }else{\
    if(!(root->cust_root_field.first=n->parent)){\
      root->cust_root_field.last=n->parent;\
      cust_empty_state;\
    }else{\
      cust_first_change;\
    }\
  }\
  for(p=n->parent;p;p=p->parent)\
    if(p->hdiff++<0) break;\
  n->parent=n->left=n->right=NULL;\
  root->cust_root_field.count--;\
  return cust_prefix##_node2item(root,n);\
}


#ifdef __cplusplus
} /* extern "C"*/
#endif

#endif /* _UL_GAVLFLESINT_H */
