#include <gtest/gtest.h>
#include <Lexer/Lexer.h>

TEST(LexerIdentifiers, SimpleIdents) {
  Lexer::Lexer lx("foo bar123");
  auto toks = lx.tokenize();

  ASSERT_EQ(toks[0].kind, Lexer::TokenKind::Ident);
  EXPECT_EQ(toks[0].lexeme, "foo");

  ASSERT_EQ(toks[1].kind, Lexer::TokenKind::Ident);
  EXPECT_EQ(toks[1].lexeme, "bar123");
}

TEST(LexerIdentifiers, UnderscoreIdents) {
  Lexer::Lexer lx("_baz");
  auto toks = lx.tokenize();

  ASSERT_EQ(toks[0].kind, Lexer::TokenKind::Ident);
  EXPECT_EQ(toks[0].lexeme, "_baz");
}
