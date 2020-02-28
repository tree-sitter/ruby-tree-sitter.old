#include <dlfcn.h>
#include "ruby.h"
#include "ruby/encoding.h"
#include "tree_sitter/api.h"

VALUE rb_cParser;
VALUE rb_cTree;

/*
 * Internal: Allocate a new parser
 */
static VALUE rb_parser_alloc(VALUE self)
{
  TSParser *parser = ts_parser_new();
  return Data_Wrap_Struct(self, NULL, ts_parser_free, parser);
}

/*
 * Public: Creates a new parser
 */
static VALUE rb_parser_new(VALUE self)
{
  return self;
}

/*
 * Public: Set the language type of a parser.
 *
 * lang - A {String} identifying the language.
 *
 * Returns nothing.
 */
static VALUE rb_parser_set_language(VALUE self, VALUE lang)
{
  TSParser *parser;
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

  Data_Get_Struct(self, TSParser, parser);

  ts_parser_set_language(parser, (*language_func)());
  dlclose(handle);

  return Qnil;
}

/*
 * Public: Set the parser string.
 *
 * string - A {String} identifying the parser contents.
 *
 * Returns nothing.
 */
static VALUE rb_parser_set_input_string(VALUE self, VALUE rb_input_string)
{
  TSParser *parser;
  Check_Type(rb_input_string, T_STRING);
  char *c_string = StringValueCStr(rb_input_string);

  Data_Get_Struct(self, TSParser, parser);

  ts_parser_set_input_string(parser, c_string);

  return Qnil;
}

/*
 * Public: Parses the parser string.
 *
 * Returns true if successful.
 */
static VALUE rb_parser_parse(VALUE self)
{
  TSParser *parser;
  Data_Get_Struct(self, TSParser, parser);
  ts_parser_parse(parser);
  return Qtrue;
}

/*
 * Public: Returns the parser root node.
 *
 * Returns a {Node}.
 */
static VALUE rb_parser_root_node(VALUE self)
{
  TSParser *parser;
  Data_Get_Struct(self, TSParser, parser);
  TSNode ts_node = ts_parser_root_node(parser);
  return rb_new_node(ts_node, parser);
}

void ruby_tree_sitter_init_parser()
{
  VALUE tree_sitter = rb_define_module("TreeSitter");

  rb_eDocumentError = rb_define_class_under(tree_sitter, "DocumentError", rb_eStandardError);

  VALUE rb_cDocument = rb_define_class_under(tree_sitter, "Document", rb_cObject);
  rb_define_alloc_func(rb_cDocument, rb_parser_alloc);
  rb_define_method(rb_cDocument, "initialize", rb_parser_new, 2);
  rb_define_method(rb_cDocument, "language=", rb_parser_set_language, 1);
  rb_define_method(rb_cDocument, "input_string=", rb_parser_set_input_string, 1);
  rb_define_method(rb_cDocument, "parse", rb_parser_parse, 0);
  rb_define_method(rb_cDocument, "root_node", rb_parser_root_node, 0);
}
