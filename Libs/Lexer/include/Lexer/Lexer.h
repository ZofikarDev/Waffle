#pragma once
#include <Lexer/Types.h>

#include <vector>

namespace Lexer
{

    class Lexer
    {
    public:
        explicit Lexer(std::string source, std::string filename = "<memory>");

        Token next();
        Token peek(std::size_t lookahead = 0);
        std::vector<Token> tokenize();

    private:
        std::string source;
        std::string filename;
        std::size_t pos = 0;

        Token makeToken(TokenKind kind, std::size_t start, std::size_t length) const;
        [[nodiscard]] char peekChar(std::size_t offset = 0) const;
        char advance();
        [[nodiscard]] bool eof() const;
    };

} // namespace Lexer
