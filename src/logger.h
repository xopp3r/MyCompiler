#include <iostream>

#define INFO std::cout << "INFO: "
#define WARNING std::cout << "WARNING: "
#define ERROR(...) std::cout << "ERROR: " << __VA_ARGS__ << std::endl; exit(EXIT_FAILURE);
