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

``` ruby
require 'tree-sitter'

# compile a grammar
ruby_grammar = File.read("ruby/grammar.json")
TreeSitter.compile(ruby_grammar)

# make a document
doc = TreeSitter::Document.new

```
## Development

After checking out the repo, run `script/bootstrap` to install dependencies. Then, run `bundle exec rake test` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.
