#ifndef RB_TREE_SITTER_NODE_H
#define RB_TREE_SITTER_NODE_H

#include "ruby.h"
#include "runtime/document.h"
#include "tree-sitter.h"

typedef struct ast_node_type {
  TSNode ts_node;
  TSDocument *ts_document;
} AstNode;

static VALUE rb_cNode;

void init_node();
static void rb_node_free(void *n);
VALUE rb_new_node(TSNode ts_node, TSDocument *ts_document);
VALUE rb_node_to_s(VALUE self);
VALUE rb_node_type(VALUE self);
VALUE rb_node_start_point(VALUE self);
VALUE rb_node_end_point(VALUE self);
VALUE rb_node_is_named(VALUE self);
VALUE rb_node_child_count(VALUE self);
VALUE rb_node_named_child_count(VALUE self);
VALUE rb_node_first_child(VALUE self);
VALUE rb_node_first_named_child(VALUE self);
VALUE rb_node_last_child(VALUE self);
VALUE rb_node_last_named_child(VALUE self);
VALUE rb_node_child(VALUE self, VALUE child_index);
VALUE rb_node_named_child(VALUE self, VALUE child_index);

#endif
