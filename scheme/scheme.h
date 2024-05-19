#pragma once

#include "object.h"

#include <memory>
#include <string>

class Scheme {
public:
    Scheme();
    std::string Evaluate(const std::string& expression);

private:
    std::shared_ptr<Object::State> state_;
};
