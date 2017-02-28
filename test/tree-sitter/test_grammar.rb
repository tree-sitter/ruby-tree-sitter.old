require 'test_helper'

class TreeSitter::GrammarTest < Minitest::Test
  def setup
    @arithmetic = File.read(File.join(fixtures_dir, 'arithmetic', 'grammar.json'))
  end

  def test_that_it_parses_grammars
    assert TreeSitter.compile('Arithmetic', @arithmetic)
  end

  def test_that_it_refuses_non_strings
    assert_raises TypeError do
      TreeSitter.compile(12345, @arithmetic)
    end

    assert_raises TypeError do
      TreeSitter.compile('Arithmetic', 1234)
    end
  end

  def test_that_it_raises_errors
    # mess up the grammar
    contents = @arithmetic[-123]
    assert_raises TreeSitter::GrammarError do
      TreeSitter.compile('Arithmetic', contents)
    end
  end

  # def test_that_it_gets_languages
  #   TreeSitter.compile('Arithmetic', @arithmetic)
  #   assert_equal ['Arithmetic'], TreeSitter.languages
  # end
end
