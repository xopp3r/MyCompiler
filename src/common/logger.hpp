#include <iostream>
#include <stdexcept>



#define INFO std::cout << "INFO: "
#define WARNING std::cout << "WARNING: "
#define ERROR(...) std::cout << "ERROR: " << __VA_ARGS__ << std::endl; exit(EXIT_FAILURE);



class TokenizerError : public std::runtime_error {
    public:
    const char* what() const noexcept override {
        return what_message.c_str();
    }

    private:
    std::string what_message;
};    
