#include "Lexer/Lexer.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <string_view>

using namespace std::string_view_literals;

namespace Lexer
{

    namespace Detail
    {
        constexpr auto kKeywords = std::to_array<std::pair<std::string_view, TokenKind>>(
                {{"func"sv, TokenKind::Func},        {"extern"sv, TokenKind::Extern},
                 {"return"sv, TokenKind::Return},    {"while"sv, TokenKind::While},
                 {"for"sv, TokenKind::For},          {"if"sv, TokenKind::If},
                 {"else"sv, TokenKind::Else},        {"use"sv, TokenKind::Use},
                 {"mut"sv, TokenKind::Mut},          {"var"sv, TokenKind::Var},
                 {"public"sv, TokenKind::Public},    {"void"sv, TokenKind::Void},
                 {"bool"sv, TokenKind::Bool},        {"int8"sv, TokenKind::Int8},
                 {"int16"sv, TokenKind::Int16},      {"int32"sv, TokenKind::Int32},
                 {"int64"sv, TokenKind::Int64},      {"uint8"sv, TokenKind::Uint8},
                 {"uint16"sv, TokenKind::Uint16},    {"uint32"sv, TokenKind::Uint32},
                 {"uint64"sv, TokenKind::Uint64},    {"fp32"sv, TokenKind::Fp32},
                 {"fp64"sv, TokenKind::Fp64},        {"true"sv, TokenKind::BoolLiteral},
                 {"false"sv, TokenKind::BoolLiteral}});

        constexpr auto kOperators = std::to_array<std::pair<std::string_view, TokenKind>>(
                {{"<<="sv, TokenKind::LtLtEq},    {">>="sv, TokenKind::GtGtEq},  {"++"sv, TokenKind::PlusPlus},
                 {"--"sv, TokenKind::MinusMinus}, {"+="sv, TokenKind::PlusEq},   {"-="sv, TokenKind::MinusEq},
                 {"*="sv, TokenKind::StarEq},     {"/="sv, TokenKind::SlashEq},  {"%="sv, TokenKind::PercentEq},
                 {"&="sv, TokenKind::AmpEq},      {"|="sv, TokenKind::PipeEq},   {"^="sv, TokenKind::CaretEq},
                 {"=="sv, TokenKind::EqEq},       {"!="sv, TokenKind::NotEq},    {"<="sv, TokenKind::LtEq},
                 {">="sv, TokenKind::GtEq},       {"<<"sv, TokenKind::LtLt},     {">>"sv, TokenKind::GtGt},
                 {"&&"sv, TokenKind::AndAnd},     {"||"sv, TokenKind::OrOr},     {"+"sv, TokenKind::Plus},
                 {"-"sv, TokenKind::Minus},       {"*"sv, TokenKind::Star},      {"/"sv, TokenKind::Slash},
                 {"%"sv, TokenKind::Percent},     {"="sv, TokenKind::Eq},        {"<"sv, TokenKind::Lt},
                 {">"sv, TokenKind::Gt},          {"&"sv, TokenKind::Amp},       {"|"sv, TokenKind::Pipe},
                 {"^"sv, TokenKind::Caret},       {"~"sv, TokenKind::Tilde},     {"!"sv, TokenKind::Bang},
                 {"?"sv, TokenKind::Question},    {":"sv, TokenKind::Colon},     {"("sv, TokenKind::LParen},
                 {")"sv, TokenKind::RParen},      {"{"sv, TokenKind::LBrace},    {"}"sv, TokenKind::RBrace},
                 {","sv, TokenKind::Comma},       {";"sv, TokenKind::Semicolon}, {"."sv, TokenKind::Dot}});
    } // namespace Detail

    Lexer::Lexer(std::istream &input, std::string filename) : input_(input), filename_(std::move(filename)) {}

    Token Lexer::Next()
    {
        SkipWhitespace_();

        if (Eof_()) {
            return MakeToken_(TokenKind::Eof, current_pos_, "");
        }

        return ScanToken_();
    }

    Token Lexer::Peek(std::size_t lookahead)
    {
        // Simple implementation: save state, advance lookahead times, restore
        auto saved_pos = current_pos_;
        auto saved_line = line_;
        auto saved_column = column_;
        auto saved_buffer = buffer_;

        Token result{TokenKind::Eof, "", {filename_, current_pos_, 0}};
        for (std::size_t i = 0; i <= lookahead; ++i) {
            result = Next();
        }

        // Restore state
        current_pos_ = saved_pos;
        line_ = saved_line;
        column_ = saved_column;
        buffer_ = saved_buffer;
        input_.clear();
        input_.seekg(current_pos_);

        return result;
    }

    std::vector<Token> Lexer::Tokenize()
    {
        std::vector<Token> tokens;
        Token token;
        do {
            token = Next();
            tokens.push_back(token);
        }
        while (token.kind != TokenKind::Eof);
        return tokens;
    }

    Token Lexer::MakeToken_(TokenKind kind, std::size_t start, std::string const &lexeme) const
    {
        return {kind, lexeme, {filename_, start, lexeme.length()}};
    }

    char Lexer::PeekChar_(std::size_t offset)
    {
        EnsureBuffered_(offset + 1);
        if (offset >= buffer_.size()) {
            return '\0';
        }
        return buffer_[offset];
    }

    char Lexer::Advance_()
    {
        if (Eof_()) {
            return '\0';
        }

        EnsureBuffered_(1);
        if (buffer_.empty()) {
            return '\0';
        }

        char c = buffer_.front();
        buffer_.pop_front();
        current_pos_++;

        if (c == '\n') {
            line_++;
            column_ = 1;
        }
        else {
            column_++;
        }

        return c;
    }

    bool Lexer::Match_(std::string_view op)
    {
        for (std::size_t i = 0; i < op.length(); ++i) {
            if (PeekChar_(i) != op[i]) {
                return false;
            }
        }

        // Consume the matched characters
        for (std::size_t i = 0; i < op.length(); ++i) {
            Advance_();
        }

        return true;
    }

    constexpr TokenKind Lexer::LookupKeyword_(std::string_view text)
    {
        auto it = std::find_if(Detail::kKeywords.begin(), Detail::kKeywords.end(),
                               [text](auto const &pair) { return pair.first == text; });
        return it != Detail::kKeywords.end() ? it->second : TokenKind::Ident;
    }

    bool Lexer::Eof_() const { return eof_reached_ && buffer_.empty(); }

    void Lexer::EnsureBuffered_(std::size_t count)
    {
        while (buffer_.size() < count && !eof_reached_) {
            char c;
            if (input_.get(c)) {
                buffer_.push_back(c);
            }
            else {
                eof_reached_ = true;
            }
        }
    }

    Token Lexer::ScanToken_()
    {
        std::size_t start = current_pos_;
        char c = PeekChar_();

        if (c == '/' && PeekChar_(1) == '/') {
            SkipLineComment_();
            return Next();
        }

        if (c == '/' && PeekChar_(1) == '*') {
            SkipBlockComment_();
            return Next();
        }

        // Try operators (longest first)
        for (auto const &[op, kind]: Detail::kOperators) {
            if (Match_(op)) {
                return MakeToken_(kind, start, std::string(op));
            }
        }

        c = PeekChar_();

        if (std::isalpha(c) || c == '_') {
            return ScanIdentifier_();
        }

        if (std::isdigit(c)) {
            return ScanNumber_();
        }

        if (c == '"') {
            return ScanString_();
        }

        if (c == '/' && PeekChar_(1) == '/') {
            SkipLineComment_();
            return Next();
        }

        if (c == '/' && PeekChar_(1) == '*') {
            SkipBlockComment_();
            return Next();
        }

        Advance_();
        return MakeToken_(TokenKind::Error, start, std::string(1, c));
    }

    Token Lexer::ScanIdentifier_()
    {
        std::size_t start = current_pos_;
        std::string lexeme;

        while (std::isalnum(PeekChar_()) || PeekChar_() == '_') {
            lexeme += Advance_();
        }

        TokenKind kind = LookupKeyword_(lexeme);
        return MakeToken_(kind, start, lexeme);
    }

    Token Lexer::ScanNumber_()
    {
        std::size_t start = current_pos_;
        std::string lexeme;
        bool is_float = false;

        while (std::isdigit(PeekChar_())) {
            lexeme += Advance_();
        }

        if (PeekChar_() == '.' && std::isdigit(PeekChar_(1))) {
            is_float = true;
            lexeme += Advance_(); // consume '.'
            while (std::isdigit(PeekChar_())) {
                lexeme += Advance_();
            }
        }

        TokenKind kind = is_float ? TokenKind::FloatLiteral : TokenKind::IntLiteral;
        return MakeToken_(kind, start, lexeme);
    }

    Token Lexer::ScanString_()
    {
        std::size_t start = current_pos_;
        std::string lexeme;

        lexeme += Advance_(); // consume opening quote

        while (!Eof_() && PeekChar_() != '"') {
            if (PeekChar_() == '\\') {
                lexeme += Advance_(); // consume backslash
                if (!Eof_()) {
                    lexeme += Advance_(); // consume escaped character
                }
            }
            else {
                lexeme += Advance_();
            }
        }

        if (Eof_()) {
            return MakeToken_(TokenKind::Error, start, lexeme);
        }

        lexeme += Advance_(); // consume closing quote
        return MakeToken_(TokenKind::StringLiteral, start, lexeme);
    }

    void Lexer::SkipLineComment_()
    {
        Advance_(); // consume first '/'
        Advance_(); // consume second '/'

        while (!Eof_() && PeekChar_() != '\n') {
            Advance_();
        }
    }

    void Lexer::SkipBlockComment_()
    {
        Advance_(); // consume '/'
        Advance_(); // consume '*'

        while (!Eof_()) {
            if (PeekChar_() == '*' && PeekChar_(1) == '/') {
                Advance_(); // consume '*'
                Advance_(); // consume '/'
                break;
            }
            Advance_();
        }
    }

    void Lexer::SkipWhitespace_()
    {
        while (!Eof_() && std::isspace(PeekChar_())) {
            Advance_();
        }
    }

} // namespace Lexer
