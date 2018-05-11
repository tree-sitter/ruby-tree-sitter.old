#ifndef RB_TREE_SITTER_POINT_H
#define RB_TREE_SITTER_POINT_H

#include "ruby.h"
#include "tree-sitter.h"

typedef struct point_type {
  TSPoint ts_point;
} Point;

static VALUE rb_cPoint;

void init_point();
VALUE rb_point_row(VALUE self);
VALUE rb_point_column(VALUE self);

#endif
