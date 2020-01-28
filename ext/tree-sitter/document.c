#include "document.h"

#if SIZEOF_VOIDP == SIZEOF_LONG
# define NUM2PTR(x)   ((void*)(NUM2ULONG(x)))
#else
# define NUM2PTR(x)   ((void*)(NUM2ULL(x)))
#endif

VALUE rb_cDocument;

/*
 * Internal: Allocate a new document
 *
 */
static VALUE rb_document_alloc(VALUE self)
{
  TSDocument *document = ts_document_new();

  if (document == NULL) {
    rb_raise(rb_eDocumentError, "Could not create document!");
  }

  return Data_Wrap_Struct(self, NULL, ts_document_free, document);
}

/*
 * Private: Set the language type of a document.
 *
 * ptr - A {Numeric} address of the function that returns the data type for treesitter.
 *
 * Returns nothing.
 */
static VALUE rb_document_set_language(VALUE self, VALUE ptr)
{
  TSDocument *document;
  const TSLanguage * (*language_func)();

  Data_Get_Struct(self, TSDocument, document);

  language_func = NUM2PTR(ptr);

  ts_document_set_language(document, (*language_func)());

  return Qnil;
}

/*
 * Public: Set the document string.
 *
 * string - A {String} identifying the document contents.
 *
 * Returns nothing.
 */
VALUE rb_document_set_input_string(VALUE self, VALUE rb_input_string)
{
  TSDocument *document;
  Check_Type(rb_input_string, T_STRING);
  char *c_string = StringValueCStr(rb_input_string);

  Data_Get_Struct(self, TSDocument, document);

  ts_document_set_input_string(document, c_string);

  return Qnil;
}

/*
 * Public: Parses the document string.
 *
 * Returns true if successful.
 */
VALUE rb_document_parse(VALUE self)
{
  TSDocument *document;

  Data_Get_Struct(self, TSDocument, document);

  ts_document_parse(document);

  return Qtrue;
}

/*
 * Public: Returns the document root node.
 *
 * Returns a {Node}.
 */
VALUE rb_document_root_node(VALUE self)
{
  TSDocument *document;

  Data_Get_Struct(self, TSDocument, document);
  TSNode ts_node = ts_document_root_node(document);

  return rb_new_node(ts_node, document);
}

void init_document()
{
  VALUE tree_sitter = rb_define_module("TreeSitter");

  rb_eDocumentError = rb_define_class_under(tree_sitter, "DocumentError", rb_eStandardError);

  VALUE rb_cDocument = rb_define_class_under(tree_sitter, "Document", rb_cObject);
  rb_define_alloc_func(rb_cDocument, rb_document_alloc);
  rb_define_const(rb_cDocument, "BUNDLE_PATH", rb_str_new2(BUNDLE_PATH));
  rb_define_private_method(rb_cDocument, "set_language", rb_document_set_language, 1);
  rb_define_method(rb_cDocument, "input_string=", rb_document_set_input_string, 1);
  rb_define_method(rb_cDocument, "parse", rb_document_parse, 0);
  rb_define_method(rb_cDocument, "root_node", rb_document_root_node, 0);
}
