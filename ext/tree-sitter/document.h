#ifndef RB_TREE_SITTER_DOCUMENT_H
#define RB_TREE_SITTER_DOCUMENT_H

#include "ruby.h"
#include <dlfcn.h>
#include "tree-sitter.h"

static VALUE rb_eDocumentError;

void init_document();
static VALUE rb_document_alloc(VALUE self);
VALUE rb_document_new(VALUE self, VALUE rb_str, VALUE rb_options);
VALUE rb_document_set_language(VALUE self, VALUE lang);
VALUE rb_document_set_input_string(VALUE self, VALUE str);
VALUE rb_document_parse(VALUE self);

#endif
