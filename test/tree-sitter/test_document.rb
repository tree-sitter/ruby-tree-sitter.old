require 'test_helper'

class TreeSitter::DocumentTest < Minitest::Test
  def test_that_it_creates_documents
    assert TreeSitter::Document.new
  end

  def test_that_it_does_not_accept_non_string_languages
    document = TreeSitter::Document.new
    assert_raises TypeError do
      document.language = 123
    end
  end

  def test_that_it_accepts_languages
    document = TreeSitter::Document.new
    document.language = 'tree_sitter_arithmetic'
  end

  def test_that_unknown_languages_error
    document = TreeSitter::Document.new

    assert_raises TreeSitter::DocumentError do
      document.language = 'tree_sitter_blah'
    end
  end

  def test_that_it_does_does_accept_non_string_input
    document = TreeSitter::Document.new
    document.language = 'tree_sitter_arithmetic'

    assert_raises TypeError do
      document.input_string = 123
    end
  end

  def test_that_it_parses
    document = TreeSitter::Document.new
    document.language = 'tree_sitter_arithmetic'
    document.input_string = 'a + b * 5'

    document.parse
  end
end
