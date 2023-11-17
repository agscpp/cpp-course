#pragma once

#include <cctype>
#include <cstddef>
#include <cstdio>
#include <string>
#include <variant>
#include <istream>
#include "error.h"

struct SymbolToken {
    std::string name;

    SymbolToken() = default;

    SymbolToken(const std::string& name) : name(name) {
    }

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    }
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    }
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    ConstantToken(int value) : value(value) {
    }

    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    }
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

// Интерфейс позволяющий читать токены по одному из потока.
class Tokenizer {
public:
    Tokenizer(std::istream* in) : token_(SymbolToken()), in_(in), is_end_(false) {
        Next();
    }

    bool IsEnd() {
        return is_end_;
    }

    void Next() {
        while (std::isspace(in_->peek())) {
            in_->get();
        }

        if (in_->peek() == EOF) {
            is_end_ = true;
            return;
        }

        std::string curr_str;
        char first_symbol = in_->get();

        if (first_symbol == '.') {
            token_ = DotToken();
        } else if (first_symbol == '(') {
            ++number_open_brackets_;
            token_ = BracketToken::OPEN;
        } else if (first_symbol == ')') {
            ++number_close_brackets_;
            token_ = BracketToken::CLOSE;
        } else if (first_symbol == '\'') {
            token_ = QuoteToken();
        } else if (std::isdigit(first_symbol) || first_symbol == '+' || first_symbol == '-') {
            curr_str += first_symbol;
            while (std::isdigit(in_->peek())) {
                curr_str += in_->get();
            }
            if (curr_str.size() > 1 || std::isdigit(curr_str.front())) {
                token_ = ConstantToken(std::stoi(curr_str));
            } else {
                token_ = SymbolToken(curr_str);
            }
        } else {
            if (!IsSymbol(first_symbol)) {
                throw SyntaxError{"Используется неизвестный токен"};
            }
            curr_str += first_symbol;
            while (IsSymbol(in_->peek())) {
                curr_str += in_->get();
            }
            token_ = SymbolToken(curr_str);
        }
    }

    Token GetToken() {
        return token_;
    }

    bool IsNesting() {
        return number_open_brackets_ - number_close_brackets_ > 0 ||
               token_ == Token(BracketToken::CLOSE);
    }

private:
    Token token_;
    std::istream* in_;
    bool is_end_;

    std::size_t number_open_brackets_ = 0;
    std::size_t number_close_brackets_ = 0;

    inline bool IsSymbol(char c) {
        std::string s = "<=>*#?!-";
        return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') ||
               s.find(c) != std::string::npos;
    }
};
