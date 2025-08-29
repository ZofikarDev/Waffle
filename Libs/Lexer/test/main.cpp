#include <Lexer/Lexer.h>
#include <gtest/gtest.h>

//
// Identifiers
//
TEST(LexerIdentifiers, SimpleIdents)
{
    auto text = std::istringstream("foo bar123");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    ASSERT_EQ(toks[0].kind, Lexer::TokenKind::Ident);
    EXPECT_EQ(toks[0].lexeme, "foo");

    ASSERT_EQ(toks[1].kind, Lexer::TokenKind::Ident);
    EXPECT_EQ(toks[1].lexeme, "bar123");
}

TEST(LexerIdentifiers, UnderscoreIdents)
{
    auto text = std::istringstream("_baz");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    ASSERT_EQ(toks[0].kind, Lexer::TokenKind::Ident);
    EXPECT_EQ(toks[0].lexeme, "_baz");
}

//
// Keywords
//
TEST(LexerKeywords, BasicKeywords)
{
    auto text = std::istringstream(
            "func return mut var if else while for extern use public void bool int32 fp64 true false");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::Func);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::Return);
    EXPECT_EQ(toks[2].kind, Lexer::TokenKind::Mut);
    EXPECT_EQ(toks[3].kind, Lexer::TokenKind::Var);
    EXPECT_EQ(toks[4].kind, Lexer::TokenKind::If);
    EXPECT_EQ(toks[5].kind, Lexer::TokenKind::Else);
    EXPECT_EQ(toks[6].kind, Lexer::TokenKind::While);
    EXPECT_EQ(toks[7].kind, Lexer::TokenKind::For);
    EXPECT_EQ(toks[8].kind, Lexer::TokenKind::Extern);
    EXPECT_EQ(toks[9].kind, Lexer::TokenKind::Use);
    EXPECT_EQ(toks[10].kind, Lexer::TokenKind::Public);
    EXPECT_EQ(toks[11].kind, Lexer::TokenKind::Void);
    EXPECT_EQ(toks[12].kind, Lexer::TokenKind::Bool);
    EXPECT_EQ(toks[13].kind, Lexer::TokenKind::Int32);
    EXPECT_EQ(toks[14].kind, Lexer::TokenKind::Fp64);
    EXPECT_EQ(toks[15].kind, Lexer::TokenKind::BoolLiteral); // true
    EXPECT_EQ(toks[16].kind, Lexer::TokenKind::BoolLiteral); // false
}

//
// Literals
//
TEST(LexerLiterals, IntAndFloat)
{
    auto text = std::istringstream("42 123.456");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::IntLiteral);
    EXPECT_EQ(toks[0].lexeme, "42");

    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::FloatLiteral);
    EXPECT_EQ(toks[1].lexeme, "123.456");
}

TEST(LexerLiterals, StringLiteral)
{
    auto text = std::istringstream("\"hello world\"");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    ASSERT_EQ(toks[0].kind, Lexer::TokenKind::StringLiteral);
    EXPECT_EQ(toks[0].lexeme, "\"hello world\"");
}

//
// Operators & Punctuation
//
TEST(LexerOperators, SimpleOperators)
{
    auto text = std::istringstream("+-*/% = == != < <= > >= && || ++ -- &=");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::Plus);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::Minus);
    EXPECT_EQ(toks[2].kind, Lexer::TokenKind::Star);
    EXPECT_EQ(toks[3].kind, Lexer::TokenKind::Slash);
    EXPECT_EQ(toks[4].kind, Lexer::TokenKind::Percent);
    EXPECT_EQ(toks[5].kind, Lexer::TokenKind::Eq);
    EXPECT_EQ(toks[6].kind, Lexer::TokenKind::EqEq);
    EXPECT_EQ(toks[7].kind, Lexer::TokenKind::NotEq);
    EXPECT_EQ(toks[8].kind, Lexer::TokenKind::Lt);
    EXPECT_EQ(toks[9].kind, Lexer::TokenKind::LtEq);
    EXPECT_EQ(toks[10].kind, Lexer::TokenKind::Gt);
    EXPECT_EQ(toks[11].kind, Lexer::TokenKind::GtEq);
    EXPECT_EQ(toks[12].kind, Lexer::TokenKind::AndAnd);
    EXPECT_EQ(toks[13].kind, Lexer::TokenKind::OrOr);
    EXPECT_EQ(toks[14].kind, Lexer::TokenKind::PlusPlus);
    EXPECT_EQ(toks[15].kind, Lexer::TokenKind::MinusMinus);
    EXPECT_EQ(toks[16].kind, Lexer::TokenKind::AmpEq);
}

TEST(LexerPunctuation, BracesAndParens)
{
    auto text = std::istringstream("(){},;");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::LParen);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::RParen);
    EXPECT_EQ(toks[2].kind, Lexer::TokenKind::LBrace);
    EXPECT_EQ(toks[3].kind, Lexer::TokenKind::RBrace);
    EXPECT_EQ(toks[4].kind, Lexer::TokenKind::Comma);
    EXPECT_EQ(toks[5].kind, Lexer::TokenKind::Semicolon);
}

//
// Comments
//
TEST(LexerComments, LineAndBlockComments)
{
    auto text = std::istringstream("// line comment\n42 /* block comment */ 7");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::IntLiteral);
    EXPECT_EQ(toks[0].lexeme, "42");
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::IntLiteral);
    EXPECT_EQ(toks[1].lexeme, "7");
}

//
// EOF
//
TEST(LexerMisc, EofToken)
{
    auto text = std::istringstream("");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    ASSERT_EQ(toks[0].kind, Lexer::TokenKind::Eof);
}

//
// Edge cases: Identifiers
//
TEST(LexerIdentifiers, KeywordVsIdent)
{
    auto text = std::istringstream("funcs returnValue int32bit");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    // Should not match keywords because extra characters are present
    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::Ident); // "funcs"
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::Ident); // "returnValue"
    EXPECT_EQ(toks[2].kind, Lexer::TokenKind::Ident); // "int32bit"
}

TEST(LexerIdentifiers, MixedCase)
{
    auto text = std::istringstream("Func Return INT32");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    // Case sensitivity check
    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::Ident);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::Ident);
    EXPECT_EQ(toks[2].kind, Lexer::TokenKind::Ident);
}

//
// Edge cases: Literals
//
TEST(LexerLiterals, FloatWithoutFractional)
{
    auto text = std::istringstream("123.");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    // Currently lexed as int and dot
    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::IntLiteral);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::Dot);
}

TEST(LexerLiterals, UnterminatedString)
{
    auto text = std::istringstream("\"hello");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    ASSERT_EQ(toks[0].kind, Lexer::TokenKind::Error);
}

TEST(LexerLiterals, EscapedQuote)
{
    auto text = std::istringstream(R"("he\"llo")");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    ASSERT_EQ(toks[0].kind, Lexer::TokenKind::StringLiteral);
    EXPECT_EQ(toks[0].lexeme, "\"he\\\"llo\"");
}

//
// Edge cases: Operators
//
TEST(LexerOperators, CompoundAssignments)
{
    auto text = std::istringstream("+= -= *= /= %= <<= >>= &= |= ^=");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::PlusEq);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::MinusEq);
    EXPECT_EQ(toks[2].kind, Lexer::TokenKind::StarEq);
    EXPECT_EQ(toks[3].kind, Lexer::TokenKind::SlashEq);
    EXPECT_EQ(toks[4].kind, Lexer::TokenKind::PercentEq);
    EXPECT_EQ(toks[5].kind, Lexer::TokenKind::LtLtEq);
    EXPECT_EQ(toks[6].kind, Lexer::TokenKind::GtGtEq);
    EXPECT_EQ(toks[7].kind, Lexer::TokenKind::AmpEq);
    EXPECT_EQ(toks[8].kind, Lexer::TokenKind::PipeEq);
    EXPECT_EQ(toks[9].kind, Lexer::TokenKind::CaretEq);
}

TEST(LexerOperators, ShiftOps)
{
    auto text = std::istringstream("<< >>");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::LtLt);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::GtGt);
}

//
// Edge cases: Punctuation
//
TEST(LexerPunctuation, QuestionColon)
{
    auto text = std::istringstream("?:");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::Question);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::Colon);
}

//
// Edge cases: Comments
//
TEST(LexerComments, UnterminatedBlockComment)
{
    auto text = std::istringstream("42 /* unterminated");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    // Should still return int literal, then hit EOF
    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::IntLiteral);
    EXPECT_EQ(toks[1].kind, Lexer::TokenKind::Eof);
}

TEST(LexerComments, NestedBlockCommentNotSupported)
{
    auto text = std::istringstream("1 /* outer /* inner */ still outer */ 2");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    // Depending on your spec, this might lex as:
    // "1", Error/Eof, "2"
    EXPECT_EQ(toks[0].kind, Lexer::TokenKind::IntLiteral);
    // The rest depends on whether you support nesting
}

//
// Edge cases: Misc
//
TEST(LexerMisc, UnknownCharacter)
{
    auto text = std::istringstream("@");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    ASSERT_EQ(toks[0].kind, Lexer::TokenKind::Error);
}

TEST(LexerMisc, MultipleEofCalls)
{
    auto text = std::istringstream("");
    Lexer::Lexer lx(text);
    auto toks = lx.Tokenize();

    ASSERT_EQ(toks[0].kind, Lexer::TokenKind::Eof);

    // Call Next() again explicitly
    auto again = lx.Next();
    ASSERT_EQ(again.kind, Lexer::TokenKind::Eof);
}
