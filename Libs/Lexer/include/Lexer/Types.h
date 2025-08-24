#pragma once
#include <string>

namespace Lexer
{

    enum class TokenKind
    {
        Func,
        Extern,
        Return,
        While,
        For,
        If,
        Else,
        Use,
        Mut,
        Var,
        Public,
        Void,
        Bool,
        Int8,
        Int16,
        Int32,
        Int64,
        Uint8,
        Uint16,
        Uint32,
        Uint64,
        Fp32,
        Fp64,
        IntLiteral,
        FloatLiteral,
        StringLiteral,
        BoolLiteral,
        Ident,
        Plus,
        Minus,
        Star,
        Slash,
        Percent,
        PlusPlus,
        MinusMinus,
        Eq,
        PlusEq,
        MinusEq,
        StarEq,
        SlashEq,
        PercentEq,
        LtLt,
        GtGt,
        LtLtEq,
        GtGtEq,
        Amp,
        Pipe,
        Caret,
        Tilde,
        Bang,
        AmpEq,
        PipeEq,
        CaretEq,
        EqEq,
        NotEq,
        Lt,
        LtEq,
        Gt,
        GtEq,
        AndAnd,
        OrOr,
        Question,
        Colon,
        LParen,
        RParen,
        LBrace,
        RBrace,
        Comma,
        Semicolon,
        Dot,
        Eof,
        Error
    };

    struct Span
    {
        std::string file;
        size_t start;
        size_t length;
    };

    struct Token
    {
        TokenKind kind;
        std::string lexeme;
        Span span;
    };

} // namespace Lexer
