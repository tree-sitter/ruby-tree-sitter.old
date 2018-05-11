# frozen_string_literal: true

$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'tree-sitter'

require 'minitest/autorun'
require 'minitest/pride'
require 'minitest/focus'
require 'pry'

def fixtures_dir
  File.join(File.dirname(__FILE__), 'tree-sitter', 'fixtures')
end
