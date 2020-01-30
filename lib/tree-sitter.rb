#!/usr/bin/env ruby
# frozen_string_literal: true

require 'tree-sitter/treesitter'
require 'tree-sitter/version'
require 'tree-sitter/node'
require 'fiddle'

module TreeSitter
  class Document
    def initialize(input_string, options)
      language = options[:language]
      self.language = language
      self.input_string = input_string
      self.parse
    end

    # Public: Set the language type of a document.
    #
    # lang - A {String} identifying the language.
    #
    # Returns nothing.
    def language=(lang)
      handle = Fiddle.dlopen BUNDLE_PATH
      function_address = handle[lang]
      set_language function_address
    rescue Fiddle::DLError
      raise TreeSitter::DocumentError
    end
  end
end
