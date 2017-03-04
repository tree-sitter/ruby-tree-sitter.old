#include "tree-sitter.h"
#include "tree_sitter/compiler.h"
#include "runtime/document.h"

static VALUE rb_eGrammarError;
static VALUE rb_eDocumentError;

static VALUE rb_cDocument;

struct languages {
  char *name;
  TSCompileResult result;
};

void free_languages(char* name, TSCompileResult result)
{
	free(name);
}


/*
 * Public: Compiles a new grammar.
 *
 * rb_name    - A {String} identifying the grammar name.
 * rb_grammar - A {String} containing the language grammar in JSON.
 *
 */
VALUE rb_compile(VALUE self, VALUE rb_name, VALUE rb_grammar) {
  Check_Type(rb_name, T_STRING);
  Check_Type(rb_grammar, T_STRING);

  const char *name = StringValueCStr(rb_name);

  const char *grammar = StringValueCStr(rb_grammar);
  TSCompileResult result = ts_compile_grammar(grammar);

  if (result.error_type != TSCompileErrorTypeNone) {
    rb_raise(rb_eGrammarError, "Failed to compile %s grammar: %s\n", name, result.error_message);
  }

  VALUE code = rb_str_new2(result.code);
  free(result.code);

  return code;
}

/*
 * Public: Lists the known languages.
 *
 */
VALUE rb_languages(VALUE self) {
  struct languages *map;
  Data_Get_Struct(self, struct languages, map);

  return rb_iv_get(self, "@languages");
}

/*
 * Public: Sets the language grammar of the document.
 *
 * grammar - A {String} containing the language grammar.
 *
 * Raises DocumentError if the string content can't be set.
 */
static VALUE rb_document_set_language(VALUE self, VALUE s) {
  char *text;
  Check_Type(s, T_STRING);

  // Data_Get_Struct(self, cmark_node, node);
  // text = StringValueCStr(s);
  //
  // if (!cmark_node_set_literal(node, text)) {
  //   rb_raise(rb_DocumentError, "could not set string content");
  // }

  return Qnil;
}

/*
 * Public: Creates a new document
 *
 */
static VALUE rb_document_new(VALUE self, VALUE type) {
  TSDocument *document = ts_document_new();

  if (document == NULL) {
    rb_raise(rb_eDocumentError, "could not create document!");
  }

  return self;
}

__attribute__((visibility("default"))) void Init_treesitter() {
  VALUE module;
  module = rb_define_module("TreeSitter");

  rb_define_singleton_method(module, "compile", rb_compile, 2);
  rb_define_singleton_method(module, "languages", rb_languages, 0);

  rb_eGrammarError = rb_define_class_under(module, "GrammarError", rb_eStandardError);
  rb_eDocumentError = rb_define_class_under(module, "DocumentError", rb_eStandardError);

  rb_cDocument = rb_define_class_under(module, "Document", rb_cObject);
  rb_define_method(rb_cDocument, "set_language", rb_document_set_language, 1);

  rb_define_singleton_method(rb_cDocument, "new", rb_document_new, 0);
}
