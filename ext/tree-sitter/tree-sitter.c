#include "ruby.h"

void ruby_tree_sitter_init_parser();
void ruby_tree_sitter_init_node();

__attribute__((visibility("default")))
void Init_treesitter()
{
  rb_define_module("TreeSitter");
  ruby_tree_sitter_init_parser();
  ruby_tree_sitter_init_node();
}
