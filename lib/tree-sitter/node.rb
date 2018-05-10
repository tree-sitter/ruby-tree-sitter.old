# frozen_string_literal: true

module TreeSitter
  class Node
    def children
      child_count.times.map { |i| child(i) }
    end

    def named_children
      named_child_count.times.map { |i| named_child(i) }
    end

    def text(input)
      rows = input.lines[start_position.row..end_position.row]

      if rows.count > 1
        rows.map.with_index do |line, i|
          next line[start_position.column, -1] if i == 0
          next line[0, end_position.column] if i == rows.count - 1
          line
        end.join
      else
        rows[0][start_position.column..end_position.column - 1]
      end
    end
  end
end
