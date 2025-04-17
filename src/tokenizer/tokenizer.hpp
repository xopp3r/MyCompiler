#pragma once

#include <vector>
#include <string_view>


enum TokenType {
    TOKEN_END = 0,
    TOKEN_INVALID,
    TOKEN_KEYWORD,
    TOKEN_SYMBOL
};



// Класс токена
class Token {
    public:
    Token() = delete;
    Token(size_t length, size_t startIndex, TokenType type, std::string_view lexeme)
        : length(length), position(startIndex), type(type), lexeme(lexeme) {}
        
    
    std::string_view lexeme;
    size_t length;
    size_t position;
    TokenType type;
};




// Фабрика для создания токенизаторов
// class TokenizerFactory {
//     public:
//         using Creator = std::unique_ptr<ITokenizer>(*)();
        
//         template<typename T>
//         static void registerLanguage(const std::string& name) {
//             auto& instance = getInstance();
//             instance.creators[name] = []() { return std::make_unique<T>(); };
//         }
        
//         static std::unique_ptr<ITokenizer> create(const std::string& language) {
//             auto& instance = getInstance();
//             auto it = instance.creators.find(language);
//             if (it != instance.creators.end()) {
//                 return it->second();
//             }
//             return nullptr;
//         }
        
//         static std::vector<std::string> availableLanguages() {
//             auto& instance = getInstance();
//             std::vector<std::string> languages;
//             for (const auto& pair : instance.creators) {
//                 languages.push_back(pair.first);
//             }
//             return languages;
//         }
    
//     private:
//         std::unordered_map<std::string, Creator> creators;
        
//         static TokenizerFactory& getInstance() {
//             static TokenizerFactory instance;
//             return instance;
//         }
// };
    




// Базовый интерфейс токенизатора
class ITokenizer {
    public:
        virtual ~ITokenizer() = default;
        virtual std::vector<Token> tokenize(std::string_view source) = 0;
};
        

// Токенизатор Си
class CTokenizer : public ITokenizer {
    public:
    CTokenizer();
    ~CTokenizer();
    
    std::vector<Token> tokenize(std::string_view sourceCode) override;
};