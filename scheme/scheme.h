#pragma once

#include <memory>
#include <string>

#include "object.h"

class Scheme {
public:
    Scheme();
    std::string Evaluate(const std::string& expression);

private:
    std::shared_ptr<Object::State> state_;
};
