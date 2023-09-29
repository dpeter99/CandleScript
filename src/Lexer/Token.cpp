#include "Token.h"

#undef TOKEN
#define TOKEN(type,value) TokenKind TokenKinds::type = TokenKinds::tokenMap.insert({value, TokenKind(#type, value)}).first->second;

std::map<std::string, TokenKind> TokenKinds::tokenMap;

#include "Tokens.h"

//TokenKind TokenKinds::SPACE = TokenKinds::tokenMap.insert({" ", TokenKind("SPACE", " ")}).first->second;
