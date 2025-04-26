#pragma once

#include "token.hpp"




// Base interface for tokenizer
class ITokenizer {
    public:
    ITokenizer() = delete;
    ITokenizer(std::string_view code) { (void)code; };
    
    virtual ~ITokenizer() = default;

    virtual Token nextToken() = 0; // returns TOKEN_END after finishing
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
    






