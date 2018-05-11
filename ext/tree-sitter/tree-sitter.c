#include "tree-sitter.h"

__attribute__((visibility("default"))) void Init_treesitter()
{
  VALUE tree_sitter = rb_define_module("TreeSitter");

  init_document();
  init_node();
  init_point();
}
