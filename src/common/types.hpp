#pragma once 

#include <string_view>




enum PrimitiveType {
    TYPE_INVALID,
    TYPE_INT,
    TYPE_UINT,
    TYPE_POINTER,
    TYPE_CHAR,
    TYPE_VOID
};




PrimitiveType getType(std::string_view str){
    if (str == "uint") return TYPE_UINT;    
    if (str == "int") return TYPE_INT;    
    if (str == "char") return TYPE_CHAR;    
    if (str == "pointer") return TYPE_POINTER;    
    if (str == "void") return TYPE_VOID;
    return TYPE_INVALID;    
}
