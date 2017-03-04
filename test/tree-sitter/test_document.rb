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

  def test_that_it_accpets_languages
    document = TreeSitter::Document.new
    document.language = "arithmetic"
  end
end
