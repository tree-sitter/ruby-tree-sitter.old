#include "point.h"

/*
 * Public: Return the row for a point.
 *
 * Returns an Integer.
 */
VALUE rb_point_row(VALUE self)
{
  Point *point;
  Data_Get_Struct(self, Point, point);
  return UINT2NUM(point->ts_point.row);
}

/*
 * Public: Return the column for a point.
 *
 * Returns an Integer.
 */
VALUE rb_point_column(VALUE self)
{
  Point *point;
  Data_Get_Struct(self, Point, point);
  return UINT2NUM(point->ts_point.column);
}

void init_point()
{
  VALUE tree_sitter = rb_define_module("TreeSitter");

  rb_cPoint = rb_define_class_under(tree_sitter, "Point", rb_cObject);
  rb_define_method(rb_cPoint, "row", rb_point_row, 0);
  rb_define_method(rb_cPoint, "column", rb_point_column, 0);
}
