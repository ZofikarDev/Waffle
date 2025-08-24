#include <Lexer/Lexer.h>
#include <array>
#include <cctype>
#include <unordered_map>

using namespace std::literals;

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
                 {"<<"sv, TokenKind::LtLt},       {">>"sv, TokenKind::GtGt},     {"=="sv, TokenKind::EqEq},
                 {"!="sv, TokenKind::NotEq},      {"<="sv, TokenKind::LtEq},     {">="sv, TokenKind::GtEq},
                 {"&&"sv, TokenKind::AndAnd},     {"||"sv, TokenKind::OrOr},     {"+"sv, TokenKind::Plus},
                 {"-"sv, TokenKind::Minus},       {"*"sv, TokenKind::Star},      {"/"sv, TokenKind::Slash},
                 {"%"sv, TokenKind::Percent},     {"="sv, TokenKind::Eq},        {"<"sv, TokenKind::Lt},
                 {">"sv, TokenKind::Gt},          {"&="sv, TokenKind::AmpEq},    {"|="sv, TokenKind::PipeEq},
                 {"^="sv, TokenKind::CaretEq},    {"&"sv, TokenKind::Amp},       {"|"sv, TokenKind::Pipe},
                 {"^"sv, TokenKind::Caret},       {"~"sv, TokenKind::Tilde},     {"!"sv, TokenKind::Bang},
                 {"?"sv, TokenKind::Question},    {":"sv, TokenKind::Colon},     {"("sv, TokenKind::LParen},
                 {")"sv, TokenKind::RParen},      {"{"sv, TokenKind::LBrace},    {"}"sv, TokenKind::RBrace},
                 {","sv, TokenKind::Comma},       {";"sv, TokenKind::Semicolon}, {"."sv, TokenKind::Dot}});

    } // namespace Detail

    Lexer::Lexer(std::string src, std::string fname) : source(std::move(src)), filename(std::move(fname)), pos(0) {}

    Token Lexer::MakeToken_(TokenKind kind, std::size_t const start, std::size_t length) const
    {
        return Token{kind, source.substr(start, length), Span{filename, start, length}};
    }

    char Lexer::PeekChar_(std::size_t offset) const
    {
        if (pos + offset >= source.size())
            return '\0';
        return source[pos + offset];
    }

    char Lexer::Advance_()
    {
        if (pos >= source.size())
            return '\0';
        return source[pos++];
    }

    bool Lexer::Match_(std::string_view op)
    {
        for (size_t i = 0; i < op.size(); ++i) {
            if (PeekChar_(i) != op[i]) {
                return false;
            }
        }
        // If we reach here, it matches → consume
        for (size_t i = 0; i < op.size(); ++i) {
            Advance_();
        }
        return true;
    }

    std::string Lexer::SourceSlice_(std::size_t start, std::size_t end) const
    {
        return source.substr(start, end - start);
    }

    constexpr TokenKind Lexer::LookupKeyword_(std::string_view text)
    {
        if (auto it = std::ranges::find(Detail::kKeywords, text, [](auto const &pair) { return pair.first; });
            it != Detail::kKeywords.end())
            return it->second;
        return TokenKind::Ident;
    }

    bool Lexer::Eof_() const { return pos >= source.size(); }

    Token Lexer::Next()
    {
        // Skip whitespaces and comments
        while (true) {
            if (std::isspace(PeekChar_())) {
                Advance_();
                continue;
            }
            if (PeekChar_() == '/' && PeekChar_(1) == '/') {
                while (!Eof_() && PeekChar_() != '\n')
                    Advance_();
                continue;
            }
            if (PeekChar_() == '/' && PeekChar_(1) == '*') {
                Advance_();
                Advance_(); // consume "/*"
                while (!Eof_() && !(PeekChar_() == '*' && PeekChar_(1) == '/'))
                    Advance_();
                if (!Eof_()) {
                    Advance_();
                    Advance_();
                } // consume "*/"
                continue;
            }

            break;
        }

        if (Eof_())
            return MakeToken_(TokenKind::Eof, pos, 0);

        std::size_t start = pos;
        char c = PeekChar_();

        // Identifiers / keywords
        if (std::isalpha(c) || c == '_') {
            Advance_(); // only now consume
            while (std::isalnum(PeekChar_()) || PeekChar_() == '_')
                Advance_();
            auto text = SourceSlice_(start, pos);
            return MakeToken_(LookupKeyword_(text), start, pos - start);
        }

        // Numbers
        if (std::isdigit(c)) {
            Advance_();
            while (std::isdigit(PeekChar_()))
                Advance_();
            if (PeekChar_() == '.' && std::isdigit(PeekChar_(1))) {
                Advance_(); // dot
                while (std::isdigit(PeekChar_()))
                    Advance_();
                return MakeToken_(TokenKind::FloatLiteral, start, pos - start);
            }
            return MakeToken_(TokenKind::IntLiteral, start, pos - start);
        }

        // String literal
        if (c == '"') {
            Advance_(); // consume opening quote
            while (!Eof_() && PeekChar_() != '"') {
                if (PeekChar_() == '\\')
                    Advance_();
                Advance_();
            }
            if (Eof_())
                return MakeToken_(TokenKind::Error, start, pos - start); // unterminated string literal
            Advance_(); // closing quote
            return MakeToken_(TokenKind::StringLiteral, start, pos - start);
        }

        // Operators & punctuation
        for (auto const &[op, kind]: Detail::kOperators) {
            if (Match_(op)) {
                return MakeToken_(kind, start, op.size());
            }
        }

        // If nothing matches
        Advance_(); // consume one char as error recovery
        return MakeToken_(TokenKind::Error, start, 1);
    }

    Token Lexer::Peek(std::size_t lookahead)
    {
        std::size_t const saved = pos;
        Token t = Next();
        pos = saved;
        return t;
    }

    std::vector<Token> Lexer::Tokenize()
    {
        std::vector<Token> result;
        for (;;) {
            Token t = Next();
            result.push_back(t);
            if (t.kind == TokenKind::Eof)
                break;
        }
        return result;
    }

} // namespace Lexer
