#include "scheme.h"

#include "error.h"
#include "parser.h"
#include "tokenizer.h"

#include <sstream>
#include <stdexcept>

Scheme::Scheme() : state_(std::make_shared<Object::State>()) {
}

std::string Scheme::Evaluate(const std::string& expression) {
    std::stringstream stream(expression);
    Tokenizer tokenizer(&stream);

    auto expr = Read(&tokenizer);
    if (!expr) {
        throw RuntimeError("Некорректное выражение");
    }
    tokenizer.ThrowExceptionIfNeeded();

    auto answer = expr->Evaluate(state_);
    return answer->Print();
}
