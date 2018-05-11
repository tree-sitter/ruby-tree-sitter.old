#ifndef RB_TREE_SITTER_H
#define RB_TREE_SITTER_H

#ifndef __MSXML_LIBRARY_DEFINED__
#define __MSXML_LIBRARY_DEFINED__
#endif

#include "ruby.h"
#include "runtime/document.h"
#include "ruby/encoding.h"

#include "document.h"
#include "node.h"
#include "point.h"

#define CSTR2SYM(s) (ID2SYM(rb_intern((s))))

void Init_treesitter();

#endif
