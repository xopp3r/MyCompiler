#pragma once

#include <vector>
#include <string_view>


enum TokenType {
    TOKEN_END = 0,
    TOKEN_INVALID,
    TOKEN_SYMBOL,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_CHAR,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATOR,
    TOKEN_PREPROCESSOR,
    TOKEN_COMMENT,
    TOKEN_WHITESPACE,

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






class TokenizerError : public std::runtime_error {
    public:
    const char* what() const noexcept override {
        return what_message.c_str();
    }

    private:
    std::string what_message;
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
    ITokenizer() = delete;
    ITokenizer(std::string_view code);
    
    virtual ~ITokenizer() = default;

    virtual std::vector<Token> tokenize() = 0;
};
        

// Токенизатор Си
class MyTokenizer : public ITokenizer {
    public:
    MyTokenizer() = delete;
    MyTokenizer(std::string_view code);
    ~MyTokenizer();
    
    std::vector<Token> tokenize() override;

    private:
    std::vector<Token> tokens;
    std::string_view code;

    size_t lineBegining; // Индекс начала строки
    size_t lineNumber;
    size_t cursor; // Текущий индекс
    
    void eat(void);
    bool matchPrefix(std::string_view prefix);

    void handleCharLiteral(void);
    void handleStringLiteral(void);
    void handleIdentifier(void);
    void handleDigitLiteral(void);

    
    template<typename Matcher>
    std::string_view getSequence(Matcher&& matchingFunction) {
        size_t cursorStart = cursor;
        while (cursor < code.length() and matchingFunction(code[cursor])){
            eat(1);
        }
        return code.substr(cursorStart, cursor - cursorStart);
    }
};





