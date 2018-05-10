require 'mkmf'
require 'fileutils'
require 'rbconfig'

if ENV['TREE_SITTER_PARSER_DIR'].nil?
  abort 'You need to set the `TREE_SITTER_PARSER_DIR` environment variable! See the README for more information.'
end

HOST_OS = RbConfig::CONFIG['host_os']
SITEARCH = RbConfig::CONFIG['sitearch']
LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

OS         = case RbConfig::CONFIG['host_os']
             when /mswin|msys|mingw|cygwin|bccwin|wince|emc/
               :windows
             when /darwin|mac os/
               :macos
             when /linux/
               :linux
             when /solaris|bsd/
               :unix
             else
               raise Error::WebDriverError, "unknown os: #{host_os.inspect}"
             end

SHARED_EXT = OS == :macos ? 'bundle' : 'so'

ROOT = File.expand_path(File.join(File.dirname(__FILE__), '..', '..'))
THIS_DIR = File.dirname(__FILE__)
OUT_DIR = File.join(THIS_DIR, 'out')

TREE_SITTER_DIR = File.expand_path(File.join(File.dirname(__FILE__), 'tree-sitter'))
TREE_SITTER_INCLUDE_DIR = File.join(TREE_SITTER_DIR, 'include')
TREE_SITTER_SRC_DIR = File.join(TREE_SITTER_DIR, 'src')

BUNDLE_PATH = File.join(ROOT, 'lib', 'tree-sitter', "treesitter.#{SHARED_EXT}")

HEADER_DIRS = [INCLUDEDIR, TREE_SITTER_INCLUDE_DIR]
LIB_DIRS = [LIBDIR, OUT_DIR]

dir_config('treesitter', HEADER_DIRS, LIB_DIRS)

Dir.chdir(THIS_DIR) do
  system 'make runtime'
end

files = Dir.glob("#{ENV['TREE_SITTER_PARSER_DIR']}/**/*.c")

flag = ENV['TRAVIS'] ? '-O0' : '-O2'
$LDFLAGS << " -I#{TREE_SITTER_INCLUDE_DIR} -lruntime #{files.join(' ')}"
$CFLAGS << " #{flag} -fPIC -std=c99 -I#{TREE_SITTER_SRC_DIR} -DBUNDLE_PATH='\"#{BUNDLE_PATH}\"'"

create_makefile('tree-sitter/treesitter')
