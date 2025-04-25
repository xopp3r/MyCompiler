#pragma once

#include <string>



class Position {
    public:
    Position();
    Position(size_t line, size_t column, size_t cursor)
        : line(line), column(column), cursor(cursor) {}

    std::string string(){
        return std::string("( line: ") + std::to_string(line) + " column: " + 
               std::to_string(column) + " cursor: " + std::to_string(cursor) + " )";
    }

    size_t line;
    size_t column;
    size_t cursor;
};
