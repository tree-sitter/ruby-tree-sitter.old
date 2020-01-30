#ifndef RB_TREE_SITTER_NODE_H
#define RB_TREE_SITTER_NODE_H

#include "ruby.h"
#include "runtime/document.h"
#include "tree-sitter.h"

typedef struct ast_node_type {
  TSNode ts_node;
  TSDocument *ts_document;
} AstNode;

typedef struct point_type {
  TSPoint ts_point;
} Point;

void init_node(VALUE);
static void rb_node_free(void *n);
VALUE rb_new_node(TSNode ts_node, TSDocument *ts_document);

#endif
