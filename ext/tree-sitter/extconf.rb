require 'mkmf'
require 'fileutils'
require 'rbconfig'

if ENV['TREE_SITTER_PARSER_DIR'].nil?
  abort 'You need to set the TREE_SITTER_PARSER_DIR environment variable! See the README for more information!'
end

HOST_OS = RbConfig::CONFIG['host_os']
SITEARCH = RbConfig::CONFIG['sitearch']
LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

ROOT_TMP = File.expand_path(File.join(File.dirname(__FILE__), '..', '..', 'tmp'))
TREE_SITTER_DIR = File.expand_path(File.join(File.dirname(__FILE__), 'tree-sitter'))
TREE_SITTER_SRC_DIR = File.join(TREE_SITTER_DIR, 'src')
TREE_SITTER_INCLUDE_DIR = File.join(TREE_SITTER_DIR, 'include')
TREE_SITTER_OUTPUT_DIR = File.join(TREE_SITTER_DIR, 'out', 'Release')

Dir.chdir(TREE_SITTER_DIR) do
  system 'script/configure'
  system 'make' or abort 'make failed'
end

HEADER_DIRS = [INCLUDEDIR, TREE_SITTER_SRC_DIR, TREE_SITTER_INCLUDE_DIR]
LIB_DIRS = [LIBDIR, TREE_SITTER_OUTPUT_DIR]

dir_config('treesitter', HEADER_DIRS, LIB_DIRS)

# don't even bother to do this check if using OS X's messed up system Ruby: http://git.io/vsxkn
unless SITEARCH =~ /^universal-darwin/
  abort 'libruntime is missing.' unless find_library('runtime', 'ts_document_new')
end

files = Dir.glob("#{ENV['TREE_SITTER_PARSER_DIR']}/**/*.c")

$LDFLAGS << " -L#{TREE_SITTER_OUTPUT_DIR} -I#{TREE_SITTER_INCLUDE_DIR} -lcompiler -lruntime #{files.join(' ')}"
$CFLAGS << " -O2 -I#{TREE_SITTER_INCLUDE_DIR} -I#{TREE_SITTER_SRC_DIR}"

create_makefile('tree-sitter/treesitter')
