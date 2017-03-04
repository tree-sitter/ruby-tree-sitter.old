# TreeSitter

This gem wraps around [tree-sitter](https://github.com/tree-sitter/tree-sitter).

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'tree-sitter'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install tree-sitter

## Usage

Tree-sitter performs [two different operations](https://github.com/tree-sitter/tree-sitter#overview): compiling grammars and parsing text with those grammars.

### Compiling a grammar

``` ruby
require 'tree-sitter'

# compile a grammar
ruby_grammar = File.read("ruby/grammar.json")
results = TreeSitter.compile(ruby_grammar)
```

### Running a parser

Parsing is a bit different. As tree-sitter spits out C code, at compile/install time, you'll need to provide an absolute path to a directory containing C files that the library can incorporate:

``` bash
# if you've installed this gem
$ TREE_SITTER_PARSER_DIR=/somewhere/code bundle install
# if you're using this gem locally
$ TREE_SITTER_PARSER_DIR=/somewhere/code bundle exec rake compile
```

This directory is appended with the glob suffix `**/*.c`, so it can contain as many parser files as necessary.

Then, you can start making use of the parser:

``` ruby
document = TreeSitter::Document.new
document.language = "arithmetic"
```

## Development

After checking out the repo, run `script/bootstrap` to install dependencies. Then, run `bundle exec rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.
