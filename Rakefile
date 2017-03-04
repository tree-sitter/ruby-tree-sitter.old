require 'rake/extensiontask'
require 'bundler/gem_tasks'

# Gem Spec
gem_spec = Gem::Specification.load('tree-sitter.gemspec')

# Ruby Extension
Rake::ExtensionTask.new('tree-sitter', gem_spec) do |ext|
  ext.name    = 'treesitter'
  ext.lib_dir = File.join('lib', 'tree-sitter')
end

Rake::Task['clean'].enhance do
  ext_dir = File.join(File.dirname(__FILE__), 'ext', 'tree-sitter', 'tree-sitter')
  Dir.chdir(ext_dir) do
    `make clean`
  end
end

# Testing
require 'rake/testtask'

Rake::TestTask.new('test:unit') do |t|
  t.libs << 'lib'
  t.libs << 'test'
  t.pattern = 'test/**/*test_*.rb'
  t.verbose = true
  t.warning = false
end

task 'test:unit' => :compile

desc 'Run unit and conformance tests'
task test: %w(test:unit)

task :console do
  require 'pry'
  require 'tree-sitter'

  def reload!
    files = $LOADED_FEATURES.select { |feat| feat =~ /\/tree-sitter\// }
    files.each { |file| load file }
  end

  ARGV.clear
  Pry.start
end

task default: [:compile, :test]
