#include <object.h>
#include <parser.h>
#include <tokenizer.h>

#include <stdexcept>
#include <memory>
#include <variant>

#include "error.h"

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Пустой токен");
    }

    auto token = tokenizer->GetToken();
    tokenizer->Next();
    if (token == Token(BracketToken::OPEN)) {
        return ReadList(tokenizer);
    }
    if (const ConstantToken* constant_token = std::get_if<ConstantToken>(&token)) {
        if (!tokenizer->IsEnd() && !tokenizer->IsNesting()) {
            throw SyntaxError("Некорректный формат");
        }
        return std::make_shared<Number>(constant_token->value);
    }
    if (const SymbolToken* symbol_token = std::get_if<SymbolToken>(&token)) {
        if (!tokenizer->IsEnd() && !tokenizer->IsNesting()) {
            throw SyntaxError("Некорректный формат");
        }
        return std::make_shared<Symbol>(symbol_token->name);
    }
    if (std::get_if<QuoteToken>(&token)) {
        return std::make_shared<Cell>(std::make_shared<Symbol>("quote"), Read(tokenizer));
    }
    throw SyntaxError("Неизвестный токен");
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    if (tokenizer->GetToken() == Token(BracketToken::CLOSE)) {
        tokenizer->Next();
        return nullptr;
    }

    auto first = Read(tokenizer);
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Ожидается еще одно значение");
    }

    auto token = tokenizer->GetToken();
    if (std::get_if<DotToken>(&token)) {
        tokenizer->Next();
        auto second = Read(tokenizer);
        if (tokenizer->IsEnd() || tokenizer->GetToken() != Token(BracketToken::CLOSE)) {
            throw SyntaxError("Ожидался символ завершения списка");
        }
        tokenizer->Next();
        return std::make_shared<Cell>(first, second);
    }

    return std::make_shared<Cell>(first, ReadList(tokenizer));
}
