#pragma once
#include <Lexer/Types.h>

#include <vector>

namespace Lexer
{

    class Lexer
    {
    public:
        explicit Lexer(std::string source, std::string filename = "<memory>");

        Token Next();
        Token Peek(std::size_t lookahead = 0);
        std::vector<Token> Tokenize();

    private:
        std::string source;
        std::string filename;
        std::size_t pos = 0;

        [[nodiscard]] Token MakeToken_(TokenKind kind, std::size_t start, std::size_t length) const;
        [[nodiscard]] char PeekChar_(std::size_t offset = 0) const;
        char Advance_();
        bool Match_(std::string_view op);
        [[nodiscard]] std::string SourceSlice_(std::size_t start, std::size_t end) const;
        [[nodiscard]] static constexpr TokenKind LookupKeyword_(std::string_view text);
        [[nodiscard]] bool Eof_() const;
    };

} // namespace Lexer
