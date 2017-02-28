#include "tree-sitter.h"

static VALUE rb_Document;

__attribute__((visibility("default"))) void Init_treesitter() {
  VALUE module;
  module = rb_define_module("TreeSitter");
  rb_Document = rb_define_class_under(module, "Document", rb_cObject);
}
