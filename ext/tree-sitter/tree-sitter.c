#include "tree-sitter.h"

#include <dlfcn.h>

static VALUE rb_eGrammarError;
static VALUE rb_eDocumentError;

static VALUE rb_cDocument;
static VALUE rb_cNode;
static VALUE rb_cPoint;

/*
 * Internal: Allocate a new document
 *
 */
VALUE rb_documment_alloc(VALUE self)
{
  TSDocument *document = ts_document_new();

  if (document == NULL) {
    rb_raise(rb_eDocumentError, "Could not create document!");
  }

  return Data_Wrap_Struct(self, NULL, ts_document_free, document);
}

/*
 * Public: Creates a new document
 *
 */
static VALUE rb_document_new(VALUE self)
{
  TSDocument *document;

  Data_Get_Struct(self, TSDocument, document);

  return self;
}

/*
 * Public: Set the language type of a document.
 *
 * lang - A {String} identifying the language.
 *
 * Returns nothing.
 */
static VALUE rb_document_set_language(VALUE self, VALUE lang)
{
  TSDocument *document;
  char *language_name;
  void *handle;
  char *error;
  Check_Type(lang, T_STRING);

  language_name = StringValueCStr(lang);

  const TSLanguage * (*language_func)();

  handle = dlopen(BUNDLE_PATH, RTLD_LAZY);
  if (!handle) {
    rb_raise(rb_eDocumentError, "%s", dlerror());
  }

  dlerror();    /* Clear any existing error */

  *(void **) (&language_func) = dlsym(handle, language_name);

  if ((error = dlerror()) != NULL)  {
    rb_raise(rb_eDocumentError, "%s", error);
  }

  Data_Get_Struct(self, TSDocument, document);

  ts_document_set_language(document, (*language_func)());
  dlclose(handle);

  return Qnil;
}

/*
 * Public: Set the document string.
 *
 * string - A {String} identifying the document contents.
 *
 * Returns nothing.
 */
static VALUE rb_document_set_input_string(VALUE self, VALUE str)
{
  TSDocument *document;
  Check_Type(str, T_STRING);
  char *string = StringValueCStr(str);

  Data_Get_Struct(self, TSDocument, document);

  ts_document_set_input_string(document, string);

  return Qnil;
}

/*
 * Public: Parses the document string.
 *
 * Returns nothing.
 */
static VALUE rb_document_parse(VALUE self)
{
  TSDocument *document;

  Data_Get_Struct(self, TSDocument, document);

  ts_document_parse(document);

  return Qnil;
}

typedef struct ast_node_type {
  TSNode ts_node;
  TSDocument *ts_document;
} AstNode;

typedef struct point_type {
  TSPoint ts_point;
} Point;

static void rb_node_free(void *n) {
  free(n);
}

static VALUE rb_new_node(TSNode ts_node, TSDocument *ts_document) {
  AstNode *node = malloc(sizeof(AstNode));
  node->ts_node = ts_node;
  node->ts_document = ts_document;
  return Data_Wrap_Struct(rb_cNode, NULL, rb_node_free, node);
}

/*
 * Public: Returns the document root node.
 *
 * Returns a Node.
 */
static VALUE rb_document_root_node(VALUE self) {
  TSDocument *document;

  Data_Get_Struct(self, TSDocument, document);
  TSNode ts_node = ts_document_root_node(document);

  return rb_new_node(ts_node, document);
}

/*
 * Public: Render the node and its children as a string.
 *
 * Returns a String.
 */
static VALUE rb_node_to_s(VALUE self) {
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  return rb_str_new_cstr(ts_node_string(node->ts_node, node->ts_document));
}

/*
 * Public: The node type.
 *
 * Returns a String.
 */
static VALUE rb_node_type(VALUE self) {
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  return rb_str_new_cstr(ts_node_type(node->ts_node, node->ts_document));
}

static void rb_point_free(void *p) {
  free(p);
}

/*
 * Public: Get the starting position for a node.
 *
 * Returns a Point.
 */
static VALUE rb_node_start_point(VALUE self) {
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  TSPoint start = ts_node_start_point(node->ts_node);

  Point *point = malloc(sizeof(Point));
  point->ts_point = start;

  return Data_Wrap_Struct(rb_cPoint, NULL, rb_point_free, point);
}

/*
 * Public: Get the ending position for a node.
 *
 * Returns a Point.
 */
static VALUE rb_node_end_point(VALUE self) {
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  TSPoint start = ts_node_end_point(node->ts_node);

  Point *point = malloc(sizeof(Point));
  point->ts_point = start;

  return Data_Wrap_Struct(rb_cPoint, NULL, rb_point_free, point);
}

/*
 * Public: Does the node have a name?
 *
 * Returns a Boolean.
 */
static VALUE rb_node_is_named(VALUE self) {
  AstNode *node;
  Data_Get_Struct(self, AstNode, node);

  return ts_node_is_named(node->ts_node) ? Qtrue : Qfalse;
}

/*
 * Public: The number of named and unnamed children.
 *
 * Returns an Integer.
 */
static VALUE rb_node_child_count(VALUE self) {
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
 * Returns an Integer.
 */
static VALUE rb_node_named_child_count(VALUE self) {
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
 * Returns a Node or Nil.
 */
static VALUE rb_node_first_child(VALUE self) {
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
 * Returns a Node or Nil.
 */
static VALUE rb_node_first_named_child(VALUE self) {
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
 * Returns a Node or Nil.
 */
static VALUE rb_node_last_child(VALUE self) {
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
 * Returns a Node or Nil.
 */
static VALUE rb_node_last_named_child(VALUE self) {
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
 * Returns a Node or Nil.
 */
static VALUE rb_node_child(VALUE self, VALUE child_index) {
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
 * Returns a Node or Nil.
 */
static VALUE rb_node_named_child(VALUE self, VALUE child_index) {
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
 * Returns an Integer.
 */
static VALUE rb_point_row(VALUE self) {
  Point *point;
  Data_Get_Struct(self, Point, point);
  return UINT2NUM(point->ts_point.row);
}

/*
 * Public: Return the column for a point.
 *
 * Returns an Integer.
 */
static VALUE rb_point_column(VALUE self) {
  Point *point;
  Data_Get_Struct(self, Point, point);
  return UINT2NUM(point->ts_point.column);
}

__attribute__((visibility("default"))) void Init_treesitter() {
  VALUE module;
  module = rb_define_module("TreeSitter");

  rb_eGrammarError = rb_define_class_under(module, "GrammarError", rb_eStandardError);
  rb_eDocumentError = rb_define_class_under(module, "DocumentError", rb_eStandardError);

  rb_cDocument = rb_define_class_under(module, "Document", rb_cObject);
  rb_define_alloc_func(rb_cDocument, rb_documment_alloc);
  rb_define_method(rb_cDocument, "initialize", rb_document_new, 0);
  rb_define_method(rb_cDocument, "language=", rb_document_set_language, 1);
  rb_define_method(rb_cDocument, "input_string=", rb_document_set_input_string, 1);
  rb_define_method(rb_cDocument, "parse", rb_document_parse, 0);
  rb_define_method(rb_cDocument, "root_node", rb_document_root_node, 0);

  rb_cNode = rb_define_class_under(module, "Node", rb_cObject);
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

  rb_cPoint = rb_define_class_under(module, "Point", rb_cObject);
  rb_define_method(rb_cPoint, "row", rb_point_row, 0);
  rb_define_method(rb_cPoint, "column", rb_point_column, 0);
}
