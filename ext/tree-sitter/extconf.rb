require 'mkmf'
require 'fileutils'
require 'rbconfig'

HOST_OS = RbConfig::CONFIG['host_os']
SITEARCH = RbConfig::CONFIG['sitearch']
LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

ROOT_TMP = File.expand_path(File.join(File.dirname(__FILE__), '..', '..', 'tmp'))
TREE_SITTER_DIR = File.expand_path(File.join(File.dirname(__FILE__), 'tree-sitter'))
TREE_SITTER_OUTPUT = File.join(TREE_SITTER_DIR, 'out', 'Release')

Dir.chdir(TREE_SITTER_DIR) do
  system 'script/configure'
  system 'make' or abort 'make failed'
end

HEADER_DIRS = [INCLUDEDIR]
LIB_DIRS = [LIBDIR, TREE_SITTER_OUTPUT]

dir_config('treesitter', HEADER_DIRS, LIB_DIRS)

# don't even bother to do this check if using OS X's messed up system Ruby: http://git.io/vsxkn
unless SITEARCH =~ /^universal-darwin/
  # abort 'libcompiler is missing.' unless find_library('compiler', 'compile')
  abort 'libruntime is missing.' unless find_library('runtime', 'ts_document_new')
end

puts TREE_SITTER_OUTPUT
$LDFLAGS << " -L#{TREE_SITTER_OUTPUT} -lcompiler -lruntime"

create_makefile('tree-sitter/treesitter')
