#include "node.h"

VALUE rb_cNode;
VALUE rb_cPoint;

VALUE rb_new_node(TSNode ts_node, TSDocument *ts_document)
{
  AstNode *node = xmalloc(sizeof(AstNode));
  node->ts_node = ts_node;
  node->ts_document = ts_document;
  return Data_Wrap_Struct(rb_cNode, NULL, xfree, node);
}

/*
 * Public: Render the node and its children as a string.
 *
 * Returns a {String}.
 */
static VALUE rb_node_to_s(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  return rb_str_new_cstr(ts_node_string(node->ts_node, node->ts_document));
}

/*
 * Public: The node type.
 *
 * Returns a {String}.
 */
static VALUE rb_node_type(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  return rb_str_new_cstr(ts_node_type(node->ts_node, node->ts_document));
}

/*
 * Public: Get the starting position for a node.
 *
 * Returns a {Point}.
 */
static VALUE rb_node_start_point(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  TSPoint start = ts_node_start_point(node->ts_node);
  Point *point = xmalloc(sizeof(Point));
  point->ts_point = start;

  return Data_Wrap_Struct(rb_cPoint, NULL, xfree, point);
}

/*
 * Public: Get the ending position for a node.
 *
 * Returns a {Point}.
 */
static VALUE rb_node_end_point(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  TSPoint start = ts_node_end_point(node->ts_node);

  Point *point = xmalloc(sizeof(Point));
  point->ts_point = start;

  return Data_Wrap_Struct(rb_cPoint, NULL, xfree, point);
}

/*
 * Public: Does the node have a name?
 *
 * Returns a {Boolean}.
 */
static VALUE rb_node_is_named(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  return ts_node_is_named(node->ts_node) ? Qtrue : Qfalse;
}

/*
 * Public: The number of named and unnamed children.
 *
 * Returns an {Integer}.
 */
static VALUE rb_node_child_count(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  if (node->ts_node.data) {
    return UINT2NUM(ts_node_child_count(node->ts_node));
  } else {
    return UINT2NUM(0);
  }
}

/*
 * Public: The number of named children.
 *
 * Returns an {Integer}.
 */
static VALUE rb_node_named_child_count(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  if (node->ts_node.data) {
    return UINT2NUM(ts_node_named_child_count(node->ts_node));
  } else {
    return UINT2NUM(0);
  }
}

/*
 * Public: Return the first child.
 *
 * Returns a {Node} or nil.
 */
static VALUE rb_node_first_child(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  TSNode child = ts_node_child(node->ts_node, 0);

  if (child.data) {
    return rb_new_node(child, node->ts_document);
  } else {
    return Qnil;
  }
}

/*
 * Public: Return the first named child.
 *
 * Returns a {Node} or nil.
 */
static VALUE rb_node_first_named_child(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  TSNode child = ts_node_named_child(node->ts_node, 0);

  if (child.data) {
    return rb_new_node(child, node->ts_document);
  } else {
    return Qnil;
  }
}

/*
 * Public: Return the last child.
 *
 * Returns a {Node} or nil.
 */
static VALUE rb_node_last_child(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  uint32_t child_count = ts_node_child_count(node->ts_node);
  if (child_count > 0) {
    TSNode child = ts_node_child(node->ts_node, child_count - 1);
    return rb_new_node(child, node->ts_document);
  } else {
    return Qnil;
  }
}

/*
 * Public: Return the last named child.
 *
 * Returns a {Node} or nil.
 */
static VALUE rb_node_last_named_child(VALUE self)
{
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  uint32_t child_count = ts_node_named_child_count(node->ts_node);
  if (child_count > 0) {
    TSNode child = ts_node_named_child(node->ts_node, child_count - 1);
    return rb_new_node(child, node->ts_document);
  } else {
    return Qnil;
  }
}

/*
 * Public: Return the child at the specified index.
 *
 * Returns a {Node} or nil.
 */
static VALUE rb_node_child(VALUE self, VALUE child_index)
{
  Check_Type(child_index, T_FIXNUM);
  uint32_t i = NUM2UINT(child_index);

  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  uint32_t child_count = ts_node_child_count(node->ts_node);

  if (i > child_count) {
    return Qnil;
  } else {
    TSNode child = ts_node_child(node->ts_node, i);
    return rb_new_node(child, node->ts_document);
  }
}

/*
 * Public: Return the named child at the specified index.
 *
 * Returns a {Node} or nil.
 */
static VALUE rb_node_named_child(VALUE self, VALUE child_index)
{
  Check_Type(child_index, T_FIXNUM);
  uint32_t i = NUM2UINT(child_index);

  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  uint32_t child_count = ts_node_named_child_count(node->ts_node);

  if (i > child_count) {
    return Qnil;
  } else {
    TSNode child = ts_node_named_child(node->ts_node, i);
    return rb_new_node(child, node->ts_document);
  }
}

/*
 * Public: Return the row for a point.
 *
 * Returns an {Integer}.
 */
static VALUE rb_point_row(VALUE self)
{
  Point *point;
  Data_Get_Struct(self, Point, point);
  return UINT2NUM(point->ts_point.row);
}

/*
 * Public: Return the column for a point.
 *
 * Returns an {Integer}.
 */
static VALUE rb_point_column(VALUE self)
{
  Point *point;
  Data_Get_Struct(self, Point, point);
  return UINT2NUM(point->ts_point.column);
}

void init_node(VALUE tree_sitter)
{
  rb_cNode = rb_define_class_under(tree_sitter, "Node", rb_cObject);
  rb_define_method(rb_cNode, "to_s", rb_node_to_s, 0);
  rb_define_method(rb_cNode, "node_type", rb_node_type, 0);
  rb_define_method(rb_cNode, "named?", rb_node_is_named, 0);
  rb_define_method(rb_cNode, "child_count", rb_node_child_count, 0);
  rb_define_method(rb_cNode, "named_child_count", rb_node_named_child_count, 0);
  rb_define_method(rb_cNode, "first_child", rb_node_first_child, 0);
  rb_define_method(rb_cNode, "first_named_child", rb_node_first_named_child, 0);
  rb_define_method(rb_cNode, "last_child", rb_node_last_named_child, 0);
  rb_define_method(rb_cNode, "last_named_child", rb_node_last_named_child, 0);
  rb_define_method(rb_cNode, "child", rb_node_child, 1);
  rb_define_method(rb_cNode, "named_child", rb_node_named_child, 1);
  rb_define_method(rb_cNode, "start_position", rb_node_start_point, 0);
  rb_define_method(rb_cNode, "end_position", rb_node_end_point, 0);

  rb_cPoint = rb_define_class_under(rb_cNode, "Point", rb_cObject);
  rb_define_method(rb_cPoint, "row", rb_point_row, 0);
  rb_define_method(rb_cPoint, "column", rb_point_column, 0);
}
