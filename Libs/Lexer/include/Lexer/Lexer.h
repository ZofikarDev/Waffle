#pragma once
#include <Lexer/Types.h>

#include <deque>
#include <istream>
#include <string>
#include <vector>

namespace Lexer
{

    class Lexer
    {
    public:
        explicit Lexer(std::istream &input, std::string filename = "<memory>");

        Token Next();
        Token Peek(std::size_t lookahead = 0);
        std::vector<Token> Tokenize();

    private:
        std::istream &input_;
        std::string filename_;
        std::deque<char> buffer_;
        std::size_t current_pos_ = 0;
        std::size_t line_ = 1;
        std::size_t column_ = 1;
        bool eof_reached_ = false;

        [[nodiscard]] Token MakeToken_(TokenKind kind, std::size_t start, std::string const &lexeme) const;
        [[nodiscard]] char PeekChar_(std::size_t offset = 0);
        char Advance_();
        bool Match_(std::string_view op);
        [[nodiscard]] static constexpr TokenKind LookupKeyword_(std::string_view text);
        [[nodiscard]] bool Eof_() const;
        void EnsureBuffered_(std::size_t count);
        Token ScanToken_();
        Token ScanIdentifier_();
        Token ScanNumber_();
        Token ScanString_();
        void SkipLineComment_();
        void SkipBlockComment_();
        void SkipWhitespace_();
    };

} // namespace Lexer
