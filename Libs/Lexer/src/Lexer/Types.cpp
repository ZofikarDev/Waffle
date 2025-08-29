#include <Lexer/Types.h>

#include <magic_enum/magic_enum.hpp>

namespace Lexer
{
    std::ostream &operator<<(std::ostream &os, TokenKind kind) { return os << magic_enum::enum_name(kind); }
} // namespace Lexer
