require 'test_helper'

class TreeSitter::DocumentTest < Minitest::Test
  def test_that_it_creates_documents
    assert TreeSitter::Document.new
  end
end
