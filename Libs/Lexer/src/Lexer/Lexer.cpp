#include <Lexer/Lexer.h>
#include <cctype>

namespace Lexer
{

    Lexer::Lexer(std::string src, std::string fname) : source(std::move(src)), filename(std::move(fname)), pos(0) {}

    Token Lexer::makeToken(TokenKind kind, std::size_t const start, std::size_t length) const
    {
        return Token{kind, source.substr(start, length), Span{filename, start, length}};
    }

    char Lexer::peekChar(std::size_t offset) const
    {
        if (pos + offset >= source.size())
            return '\0';
        return source[pos + offset];
    }

    char Lexer::advance()
    {
        if (pos >= source.size())
            return '\0';
        return source[pos++];
    }

    bool Lexer::eof() const { return pos >= source.size(); }

    Token Lexer::next()
    {
        // skip whitespace
        while (std::isspace(peekChar())) {
            advance();
        }

        if (eof())
            return makeToken(TokenKind::Eof, pos, 0);

        // TODO: real implementation (recognize keywords, literals, ops)

        std::size_t start = pos;
        char c = advance();

        if (std::isalpha(c) || c == '_') {
            while (std::isalnum(peekChar()) || peekChar() == '_')
                advance();
            return makeToken(TokenKind::Ident, start, pos - start);
        }

        return makeToken(TokenKind::Error, start, 1);
    }

    Token Lexer::peek(std::size_t lookahead)
    {
        std::size_t saved = pos;
        Token t = next();
        pos = saved;
        return t;
    }

    std::vector<Token> Lexer::tokenize()
    {
        std::vector<Token> result;
        for (;;) {
            Token t = next();
            result.push_back(t);
            if (t.kind == TokenKind::Eof)
                break;
        }
        return result;
    }

} // namespace Lexer
