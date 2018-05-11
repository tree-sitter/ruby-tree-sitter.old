# frozen_string_literal: true

require 'test_helper'

class TreeSitter::NodeTest < Minitest::Test
  def setup
    @python = <<~PYTHON
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
    @document = TreeSitter::Document.new(@python, language: 'tree_sitter_python')
  end

  def test_gets_node_type
    assert_equal 'module', @document.root_node.node_type
  end

  def test_gets_children
    assert_equal 3, @document.root_node.named_child_count
    assert_equal 3, @document.root_node.named_children.count
    assert_equal [
      'function_definition',
      'function_definition',
      'expression_statement',
    ],  @document.root_node.children.map(&:node_type)
  end

  def test_gets_children_node_type
    first_function = @document.root_node.children[0]

    assert_equal 5, first_function.child_count
    assert_equal 5, first_function.children.count

    assert_equal 3, first_function.named_child_count
    assert_equal 3, first_function.named_children.count

    assert_equal [
      'identifier',
      'parameters',
      'expression_statement',
    ], first_function.named_children.map(&:node_type)
  end

  def test_gets_node_text
    first_function = @document.root_node.children[0]

    assert_equal 'first_function', first_function
      .named_children[0].text(@python)

    assert_equal 'arg', first_function
      .named_children[1].named_children[0].text(@python)

    assert_equal 'bubbles', first_function
      .named_children[2].named_children[0].text(@python)

    second_function = @document.root_node.children[1]

    assert_equal "a long\nstring", second_function
      .named_children[2].named_children[0].text(@python).gsub('"', '').strip

    function_call = @document.root_node.children[2].children[0]
    assert_equal 'first_function', function_call.children[0].text(@python)
  end
end
