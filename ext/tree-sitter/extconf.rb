# frozen_string_literal: true

require 'mkmf'

THIS_DIR = File.expand_path(File.dirname(__FILE__))
TREE_SITTER_DIR = File.join(THIS_DIR, 'tree-sitter')
TREE_SITTER_INCLUDE_DIR = File.join(TREE_SITTER_DIR, 'lib', 'include')
TREE_SITTER_SRC_DIR = File.join(TREE_SITTER_DIR, 'lib', 'src')
TREE_SITTER_PARSER_DIR = ENV['TREE_SITTER_PARSER_DIR']

if TREE_SITTER_PARSER_DIR.nil?
  abort 'You need to set the `TREE_SITTER_PARSER_DIR` environment variable! See the README for more information.'
end

unless Dir.exist?(TREE_SITTER_PARSER_DIR)
  abort "The `TREE_SITTER_PARSER_DIR` environment variable does not point to a real directory: `#{ENV['TREE_SITTER_PARSER_DIR']}`! See the README for more information."
end

parser_files = Dir.glob("#{TREE_SITTER_PARSER_DIR}/**/*.{c,cc}").map do |f|
  File.join(TREE_SITTER_PARSER_DIR, f)
end

$CFLAGS << "-std=c99"
$INCFLAGS = " -I#{TREE_SITTER_SRC_DIR} -I#{TREE_SITTER_INCLUDE_DIR}"
$srcs = [
  "#{TREE_SITTER_SRC_DIR}/lib.c",
  "#{THIS_DIR}/node.c",
  "#{THIS_DIR}/parser.c",
  "#{THIS_DIR}/tree-sitter.c",
  *parser_files
]

$objs = $srcs.each_with_index.map {|s, i| "#{File.basename(s)}.#{i}.o"}

p $srcs
p $objs

create_makefile('tree-sitter/treesitter')
