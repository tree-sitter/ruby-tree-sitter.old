# frozen_string_literal: true

require 'test_helper'

class TreeSitter::DocumentTest < Minitest::Test
  def test_that_it_creates_documents
    document = TreeSitter::Document.new('a + b * 5', language: 'tree_sitter_python')

    assert document
  end

  def test_that_it_requires_language_key
    assert_raises TypeError do
      TreeSitter::Document.new('a + b * 5', {})
    end
  end

  def test_that_it_does_not_accept_non_string_inputs
    assert_raises TypeError do
      TreeSitter::Document.new(123, language: 'tree_sitter_python')
    end
  end

  def test_that_it_does_not_accept_non_string_languages
    assert_raises TypeError do
      TreeSitter::Document.new('a + b * 5', language: 123)
    end
  end

  def test_that_it_accepts_string_languages
    document = TreeSitter::Document.new('a + b * 5', language: 'tree_sitter_python')
    document.language = 'tree_sitter_python'

    assert_raises TypeError do
      document.language = 123
    end
  end

  def test_that_unknown_languages_error
    assert_raises TreeSitter::DocumentError do
      TreeSitter::Document.new('a + b * 5', language: 'tree_sitter_blah')
    end
  end

  def test_that_it_accepts_string_inputs
    document = TreeSitter::Document.new('a + b * 5', language: 'tree_sitter_python')
    document.input_string = 'b * 10'

    assert_raises TypeError do
      document.input_string = 123
    end
  end

  def test_that_it_parses_valid_input
    python = <<~PYTHON
      def first_function(arg):
          bubbles
      def second_function(arg):
          """
      a long
      string
          """

      first_function(
          arg=(1, 2, 3)
      )
    PYTHON
    document = TreeSitter::Document.new(python, language: 'tree_sitter_python')

    tree = <<~TREE.gsub(/\s+/m, ' ').strip
      (module
        (function_definition
          (identifier)
          (parameters (identifier))
          (expression_statement (identifier)))
        (function_definition
          (identifier)
          (parameters (identifier))
          (expression_statement (string)))
        (expression_statement
          (call
            (identifier)
            (argument_list
              (keyword_argument
                (identifier)
                (tuple
                  (integer)
                  (integer)
                  (integer)))))))
    TREE
    assert_equal tree, document.root_node.to_s

    document.input_string = 'a + b * 5'
    document.parse

    tree = <<~TREE.gsub(/\s+/m, ' ').strip
      (module
        (expression_statement
          (binary_operator
            (identifier)
          (binary_operator
            (identifier)
          (integer)))))
    TREE
    assert_equal tree, document.root_node.to_s
  end
end
