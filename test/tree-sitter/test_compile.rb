require 'test_helper'

class TreeSitter::CompileTest < Minitest::Test
  def setup
    @arithmetic = File.read(File.join(fixtures_dir, 'arithmetic', 'grammar.json'))
  end

  def test_that_it_parses_grammars
    results = TreeSitter.compile(@arithmetic)
    assert_match %r{#include <tree_sitter/parser.h>}, results
  end

  def test_that_it_refuses_non_strings
    assert_raises TypeError do
      TreeSitter.compile(123)
    end
  end

  def test_that_it_raises_errors
    # mess up the grammar
    contents = @arithmetic[-123]
    assert_raises TreeSitter::GrammarError do
      TreeSitter.compile(contents)
    end
  end
end
