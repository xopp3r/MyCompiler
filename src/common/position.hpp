#pragma once

#include <string>



class Position {
    public:
    Position() = default;
    Position(size_t Line, size_t Column, size_t Cursor)
        : line(Line), column(Column), cursor(Cursor) {}

    std::string string(){
        return std::string("( line: ") + std::to_string(line) + " column: " + 
               std::to_string(column) + " cursor: " + std::to_string(cursor) + " )";
    }

    size_t line;
    size_t column;
    size_t cursor;
};
