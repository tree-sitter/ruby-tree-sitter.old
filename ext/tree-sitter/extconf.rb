require 'mkmf'
require 'fileutils'
require 'rbconfig'

if ENV['TREE_SITTER_PARSER_DIR'].nil?
  abort 'You need to set the `TREE_SITTER_PARSER_DIR` environment variable! See the README for more information.'
end

unless Dir.exist?(ENV['TREE_SITTER_PARSER_DIR'])
  abort "The `TREE_SITTER_PARSER_DIR` environment variable does not point to a real directory: `#{ENV['TREE_SITTER_PARSER_DIR']}`! See the README for more information."
end

SITEARCH = RbConfig::CONFIG['sitearch']
LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']
DLEXT = RbConfig::CONFIG["DLEXT"]

ROOT = File.expand_path(File.join(File.dirname(__FILE__), '..', '..'))
THIS_DIR = File.dirname(__FILE__)
OUT_DIR = File.join(THIS_DIR, 'out')

TREE_SITTER_DIR = File.expand_path(File.join(File.dirname(__FILE__), 'tree-sitter'))
TREE_SITTER_INCLUDE_DIR = File.join(TREE_SITTER_DIR, 'include')
TREE_SITTER_SRC_DIR = File.join(TREE_SITTER_DIR, 'src')

BUNDLE_PATH = File.join(ROOT, 'lib', 'tree-sitter', "treesitter.#{DLEXT}")

HEADER_DIRS = [INCLUDEDIR, TREE_SITTER_INCLUDE_DIR]
LIB_DIRS = [LIBDIR, OUT_DIR]

dir_config('treesitter', HEADER_DIRS, LIB_DIRS)

# don't even bother to do this check if using OS X's messed up system Ruby: http://git.io/vsxkn
unless SITEARCH =~ /^universal-darwin/
  abort 'libruntime is missing.' unless find_library('runtime', 'ts_document_new', TREE_SITTER_SRC_DIR)
end

FileUtils.mkdir_p(OUT_DIR)

Dir.chdir(THIS_DIR) do
  system 'make runtime'
end

require 'pry'

c_files = Dir.glob("#{ENV['TREE_SITTER_PARSER_DIR']}/**/*.{c,cc}")
c_files.each do |c_file|
  parent_dir = c_file.split(File::SEPARATOR)[-2]
  dir = File.join(OUT_DIR, parent_dir)
  FileUtils.mkdir_p(dir)
  FileUtils.cp(c_file, dir)
end

c_files = c_files.join(' ')
o_files = c_files.gsub(%r{(?:\S+)#{File::SEPARATOR}(\S+)#{File::SEPARATOR}(\w+)\.(?:c|cc)(\s|$)}, File.join(OUT_DIR, '\\1', '\\2.o\\3'))

flag = ENV['TRAVIS'] ? '-O0' : '-O2'

$LDFLAGS << " -I#{TREE_SITTER_INCLUDE_DIR} -lruntime -lstdc++"
$CFLAGS << " #{flag} -fPIC -std=c99 -I#{TREE_SITTER_SRC_DIR} -DBUNDLE_PATH='\"#{BUNDLE_PATH}\"'"

create_makefile('tree-sitter/treesitter')

makefile = File.read 'Makefile'

# tack on parser files as part of compilation
lines = makefile.lines.map do |line|
  if line.start_with?('ORIG_SRCS =')
    line.sub /$/, " #{c_files}"
  elsif line.start_with?('OBJS =')
    line.sub /$/, " #{o_files}"
  else
    line
  end
end

File.open 'Makefile', 'w' do |f|
  f.puts lines
end
