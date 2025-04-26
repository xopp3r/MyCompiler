#include <iostream>
#include <stdexcept>



#define INFO(...) do {std::cerr << "INFO: " << __VA_ARGS__ << std::endl;} while(0)
#define WARNING(...) do {std::cerr << "WARNING: " << __VA_ARGS__ << std::endl;} while(0)
#define ERROR(...) do {std::cerr << "ERROR: " << __VA_ARGS__ << std::endl; exit(EXIT_FAILURE);} while(0)



class TokenizerError : public std::runtime_error {
    public:
    const char* what() const noexcept override {
        return what_message.c_str();
    }

    private:
    std::string what_message;
};    
