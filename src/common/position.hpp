#pragma once

#include <string>



class Position {
    public:
    Position() = default;
    Position(size_t Line, size_t Column, size_t Cursor)
        : line(Line), column(Column), cursor(Cursor) {}

    std::string string(){
        return std::string("( line: ") + std::to_string(line + 1) + " column: " + 
               std::to_string(column + 1) + " cursor: " + std::to_string(cursor) + " )";
    }

    size_t line = 0;
    size_t column = 0;
    size_t cursor = 0;
};
