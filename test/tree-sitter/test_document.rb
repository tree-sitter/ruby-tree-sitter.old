# frozen_string_literal: true

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

  # TODO
  def test_that_it_accepts_languages
    document = TreeSitter::Document.new
    document.language = 'tree_sitter_python'
  end

  def test_that_unknown_languages_error
    document = TreeSitter::Document.new

    assert_raises TreeSitter::DocumentError do
      document.language = 'tree_sitter_blah'
    end
  end

  def test_that_it_does_does_accept_non_string_input
    document = TreeSitter::Document.new
    document.language = 'tree_sitter_python'

    assert_raises TypeError do
      document.input_string = 123
    end
  end

  def test_that_it_parses
    document = TreeSitter::Document.new
    document.language = 'tree_sitter_python'
    document.input_string = 'a + b * 5'

    document.parse
  end

  def test_valid_input
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
    document = TreeSitter::Document.new
    document.language = 'tree_sitter_python'
    document.input_string = python
    document.parse

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

    assert_equal 'module', document.root_node.node_type
    assert_equal 3, document.root_node.named_child_count
    assert_equal 3, document.root_node.named_children.count
    assert_equal [
      'function_definition',
      'function_definition',
      'expression_statement',
    ],  document.root_node.children.map(&:node_type)


    first_function = document.root_node.children[0]

    assert_equal 5, first_function.child_count
    assert_equal 5, first_function.children.count

    assert_equal 3, first_function.named_child_count
    assert_equal 3, first_function.named_children.count

    assert_equal [
      'identifier',
      'parameters',
      'expression_statement',
    ], first_function.named_children.map(&:node_type)

    assert_equal 'first_function', first_function
      .named_children[0].text(python)

    assert_equal 'arg', first_function
      .named_children[1].named_children[0].text(python)

    assert_equal 'bubbles', first_function
      .named_children[2].named_children[0].text(python)

    second_function = document.root_node.children[1]

    assert_equal "a long\nstring", second_function
      .named_children[2].named_children[0].text(python).gsub('"', '').strip

    function_call = document.root_node.children[2].children[0]
    assert_equal 'first_function', function_call.children[0].text(python)
  end
end
