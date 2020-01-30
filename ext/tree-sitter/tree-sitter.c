#include "tree-sitter.h"

__attribute__((visibility("default"))) void Init_treesitter()
{
  VALUE tree_sitter = rb_define_module("TreeSitter");

  init_document(tree_sitter);
  init_node(tree_sitter);
}
