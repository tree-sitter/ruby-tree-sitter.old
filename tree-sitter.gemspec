# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'tree-sitter/version'

Gem::Specification.new do |spec|
  spec.name          = 'tree-sitter'
  spec.version       = TreeSitter::VERSION
  spec.authors       = ['Garen Torikian']
  spec.email         = ['gjtorikian@gmail.com']

  spec.summary       = 'Ruby bindings to Tree-Sitter'
  spec.homepage      = 'https://www.github.com/tree-sitter/ruby-tree-sitter'

  spec.files         = %w(LICENSE.txt README.md Rakefile tree-sitter.gemspec)
  spec.files        += Dir.glob('lib/**/*.rb')
  spec.files        += Dir.glob('ext/**/*')
  spec.test_files    = Dir.glob('test/**/*')
  spec.extensions    = ['ext/tree-sitter/extconf.rb']

  spec.test_files = spec.files.grep(%r{^test/})
  # spec.executables = ['tree-sitter']
  spec.require_paths = %w(lib ext)

  spec.add_development_dependency 'awesome_print'
  spec.add_development_dependency 'bundler', '~> 1.14'
  spec.add_development_dependency 'minitest', '~> 5.0'
  spec.add_development_dependency 'minitest-focus', '~> 1.1'
  spec.add_development_dependency 'pry', '~> 0.10.0'
  spec.add_development_dependency 'rake', '~> 12.0'
  spec.add_development_dependency 'rake-compiler', '~> 0.9'
  spec.add_development_dependency 'rubocop-github'
end
