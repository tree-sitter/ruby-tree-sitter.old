#include "document.h"

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
 * Public: Creates a new document
 *
 */
VALUE rb_document_new(VALUE self, VALUE rb_input_string, VALUE rb_options)
{
  TSDocument *document;
  VALUE rb_language;

  Check_Type(rb_input_string, T_STRING);
  Check_Type (rb_options, T_HASH);

  rb_language = rb_hash_aref(rb_options, CSTR2SYM("language"));
  Check_Type(rb_language, T_STRING);

  Data_Get_Struct(self, TSDocument, document);

  rb_document_set_language(self, rb_language);
  rb_document_set_input_string(self, rb_input_string);
  rb_document_parse(self);

  return self;
}


/*
 * Public: Set the language type of a document.
 *
 * lang - A {String} identifying the language.
 *
 * Returns nothing.
 */
VALUE rb_document_set_language(VALUE self, VALUE lang)
{
  TSDocument *document;
  char *language_name;
  void *handle;
  char *error;
  Check_Type(lang, T_STRING);

  language_name = StringValueCStr(lang);

  const TSLanguage * (*language_func)();

  handle = dlopen(NULL, RTLD_LAZY);
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
  rb_define_method(rb_cDocument, "initialize", rb_document_new, 2);
  rb_define_method(rb_cDocument, "language=", rb_document_set_language, 1);
  rb_define_method(rb_cDocument, "input_string=", rb_document_set_input_string, 1);
  rb_define_method(rb_cDocument, "parse", rb_document_parse, 0);
  rb_define_method(rb_cDocument, "root_node", rb_document_root_node, 0);
}
