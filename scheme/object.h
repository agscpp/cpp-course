#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "error.h"
#include "tokenizer.h"

class Object : public std::enable_shared_from_this<Object> {
public:
    class State {
    public:
        State() = default;

        void SetParentState(std::shared_ptr<State> parent) {
            parent_ = parent;
        }

        void AddVariable(const std::string& name, std::shared_ptr<Object> value) {
            variables_[name] = value;
        }

        void UpdateVariable(const std::string& name, std::shared_ptr<Object> value) {
            if (variables_.contains(name)) {
                variables_[name] = value;
                return;
            }
            auto parent = parent_.lock();
            while (parent) {
                if (parent->variables_.contains(name)) {
                    parent->variables_[name] = value;
                    return;
                }
                parent = parent->parent_.lock();
            }
        }

        std::shared_ptr<Object> FindVariable(const std::string& key) {
            if (variables_.contains(key)) {
                return variables_[key];
            }
            if (auto parent = parent_.lock()) {
                return parent->FindVariable(key);
            }
            return nullptr;
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<Object>> variables_;
        std::weak_ptr<State> parent_;
    };

    virtual ~Object() = default;

    virtual std::shared_ptr<Object> Evaluate(std::shared_ptr<State> state) = 0;
    virtual std::string Print() = 0;
};

class Function : public Object {
public:
    virtual std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                          std::vector<std::shared_ptr<Object>>&& args) = 0;
    std::shared_ptr<Object> Evaluate(std::shared_ptr<State> state) override;
    std::string Print() override;

    virtual bool ShouldRunArgsImmediately() const {
        return true;
    }
};

class Number : public Object {
public:
    Number(int value) : value_(value) {
    }

    int GetValue() const;
    std::shared_ptr<Object> Evaluate(std::shared_ptr<State> state) override;
    std::string Print() override;

private:
    int value_;
};

class Boolean : public Object {
public:
    Boolean(bool value) : value_(value) {
    }

    bool GetValue() const;
    std::shared_ptr<Object> Evaluate(std::shared_ptr<State> state) override;
    std::string Print() override;

private:
    bool value_;
};

class Symbol : public Object {
public:
    Symbol(const std::string& name) : name_(name) {
    }

    const std::string& GetName() const;
    std::shared_ptr<Object> Evaluate(std::shared_ptr<State> state) override;
    std::string Print() override;

private:
    std::string name_;

    std::shared_ptr<Object> GetFunction();
};

class Cell : public Object {
public:
    Cell() = default;

    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
        : first_(first), second_(second), state_(std::make_shared<State>()) {
    }

    std::shared_ptr<Object> GetFirst() const;
    std::shared_ptr<Object> GetSecond() const;

    void SetFirst(std::shared_ptr<Object> obj);
    void SetSecond(std::shared_ptr<Object> obj);

    std::shared_ptr<Object> Evaluate(std::shared_ptr<State> state) override;
    std::string Print() override;

    void ParseVector(std::shared_ptr<State> state, std::vector<std::shared_ptr<Object>>* args,
                     bool should_run_args_immediately);

    void AddVariable(std::string name, std::shared_ptr<Number> value) {
        state_->AddVariable(name, value);
    }

    void SetParentState(std::shared_ptr<State> parent_state) {
        state_->SetParentState(parent_state);
    }

private:
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;
    std::shared_ptr<State> state_;
};

class Empty : public Object {
public:
    Empty() = default;

    std::shared_ptr<Object> Evaluate(std::shared_ptr<State> state) override;
    std::string Print() override;
};

class Arithmetic : public Function {
public:
    enum Operator { Plus, Minus, Divide, Multiply };

    Arithmetic(Operator opr) : operator_(opr) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;

private:
    Operator operator_;
};

class Comparison : public Function {
public:
    enum Operator { Equal, More, Less, MoreOrEqual, LessOrEqual };

    Comparison(Operator opr) : operator_(opr) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;

private:
    Operator operator_;
};

class Logic : public Function {
public:
    enum Operator { And, Or, Not };

    Logic(Operator opr) : operator_(opr) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;
    bool ShouldRunArgsImmediately() const override {
        return operator_ != Operator::And && operator_ != Operator::Or;
    }

private:
    Operator operator_;
};

class Checker : public Function {
public:
    enum Operator { If, Null, Pair, Numb, Bool, Symb, List };

    Checker(Operator opr) : operator_(opr) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;
    bool ShouldRunArgsImmediately() const override {
        return operator_ != Operator::If;
    }

private:
    Operator operator_;
};

class Quote : public Function {
public:
    Quote() = default;
    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;
};

class MinMax : public Function {
public:
    enum Operator { Min, Max };

    MinMax(Operator opr) : operator_(opr) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;

private:
    Operator operator_;
};

class CarCdr : public Function {
public:
    enum Operator { Car, Cdr, SetCar, SetCdr };

    CarCdr(Operator opr) : operator_(opr) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;

private:
    Operator operator_;
};

class Abs : public Function {
public:
    Abs() = default;
    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;
};

class Cons : public Function {
public:
    Cons() = default;
    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;
};

class Variable : public Function {
public:
    enum Operator { Define, Set };

    Variable(Operator opr) : operator_(opr) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;

private:
    Operator operator_;
};

class Lambda : public Function {
public:
    Lambda() : state_(std::make_shared<State>()) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;

    bool ShouldRunArgsImmediately() const override {
        return false;
    }

private:
    std::shared_ptr<State> state_;
    std::vector<std::shared_ptr<Object>> parameters_;
    std::vector<std::shared_ptr<Object>> lines_;
};

class DataStructure : public Function {
public:
    enum Operator { List, ListTail, ListRef };

    DataStructure(Operator opr) : operator_(opr) {
    }

    std::shared_ptr<Object> Apply(std::shared_ptr<State> state,
                                  std::vector<std::shared_ptr<Object>>&& args) override;

private:
    Operator operator_;
};

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return dynamic_cast<T*>(obj.get());
}
