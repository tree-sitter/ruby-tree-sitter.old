#include "tree-sitter.h"
#include "tree_sitter/compiler.h"
#include "runtime/document.h"

static VALUE rb_eGrammarError;
static VALUE rb_eDocumentError;

static VALUE rb_cDocument;

/*
 * Internal: Allocate a new document
 *
 */
VALUE rb_documment_alloc(VALUE self)
{
  TSDocument *document = ts_document_new();

  if (document == NULL) {
    rb_raise(rb_eDocumentError, "could not create document!");
  }

  return Data_Wrap_Struct(self, NULL, ts_document_free, document);
}

/*
 * Public: Creates a new document
 *
 */
static VALUE rb_document_new(VALUE self) {
  TSDocument *document;

  Data_Get_Struct(self, TSDocument, document);

  return self;
}

/*
 * Public: Fetches the language type of a document.
 *
 * Returns a string.
 */
static VALUE rb_document_set_language(VALUE self, VALUE language) {
  Check_Type(language, T_STRING);

  TSDocument *document;

  Data_Get_Struct(self, TSDocument, document);

  // ts_document_set_language(document, tree_sitter_arithmetic());

  return Qnil;
}

/*
 * Public: Compiles a new grammar.
 *
 * rb_grammar - A {String} containing the language grammar in JSON.
 *
 */
VALUE rb_compile(VALUE self, VALUE rb_grammar) {
  Check_Type(rb_grammar, T_STRING);

  const char *grammar = StringValueCStr(rb_grammar);
  TSCompileResult result = ts_compile_grammar(grammar);

  if (result.error_type != TSCompileErrorTypeNone) {
    free(result.code);
    rb_raise(rb_eGrammarError, "Failed to compile grammar: %s\n", result.error_message);
  }

  VALUE code = rb_str_new2(result.code);
  free(result.code);

  return code;
}

__attribute__((visibility("default"))) void Init_treesitter() {
  VALUE module;
  module = rb_define_module("TreeSitter");

  rb_define_singleton_method(module, "compile", rb_compile, 1);

  rb_eGrammarError = rb_define_class_under(module, "GrammarError", rb_eStandardError);
  rb_eDocumentError = rb_define_class_under(module, "DocumentError", rb_eStandardError);

  rb_cDocument = rb_define_class_under(module, "Document", rb_cObject);
  rb_define_alloc_func(rb_cDocument, rb_documment_alloc);
  rb_define_method(rb_cDocument, "initialize", rb_document_new, 0);
  rb_define_method(rb_cDocument, "language=", rb_document_set_language, 1);
}
