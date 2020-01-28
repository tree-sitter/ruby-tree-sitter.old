#!/usr/bin/env ruby
# frozen_string_literal: true

require 'tree-sitter/treesitter'
require 'tree-sitter/version'
require 'tree-sitter/node'

module TreeSitter
  class Document
    def initialize(input_string, options)
      language = options[:language]
      self.language = language
      self.input_string = input_string
      self.parse
    end
  end
end
