#include "object.h"

std::shared_ptr<Object> Function::Evaluate(std::shared_ptr<State> state) {
    throw RuntimeError("Не имеет реализации");
}

std::string Function::Print() {
    throw RuntimeError("Не имеет реализации");
}

int Number::GetValue() const {
    return value_;
}

std::shared_ptr<Object> Number::Evaluate(std::shared_ptr<State> state) {
    return shared_from_this();
}

std::string Number::Print() {
    return std::to_string(value_);
}

bool Boolean::GetValue() const {
    return value_;
}

std::shared_ptr<Object> Boolean::Evaluate(std::shared_ptr<State> state) {
    return shared_from_this();
}

std::string Boolean::Print() {
    return value_ ? "#t" : "#f";
}

const std::string& Symbol::GetName() const {
    return name_;
}

std::shared_ptr<Object> Symbol::Evaluate(std::shared_ptr<State> state) {
    if (auto function = GetFunction()) {
        return function;
    } else if (GetName() == "#f" || GetName() == "#t") {
        return std::make_shared<Boolean>(GetName() == "#t");
    } else if (auto variable = state->FindVariable(name_)) {
        if (Is<Function>(variable)) {
            return variable;
        }
        auto result = variable->Evaluate(state);
        if (Is<Lambda>(result) || Is<Number>(result)) {
            state->UpdateVariable(name_, result);
        }
        return result;
    }
    throw NameError("Неизвестный символ");
}

std::string Symbol::Print() {
    return name_;
}

std::shared_ptr<Object> Symbol::GetFunction() {
    if (name_ == "+") {
        return std::make_shared<Arithmetic>(Arithmetic::Operator::Plus);
    } else if (name_ == "-") {
        return std::make_shared<Arithmetic>(Arithmetic::Operator::Minus);
    } else if (name_ == "*") {
        return std::make_shared<Arithmetic>(Arithmetic::Operator::Multiply);
    } else if (name_ == "/") {
        return std::make_shared<Arithmetic>(Arithmetic::Operator::Divide);
    } else if (name_ == "=") {
        return std::make_shared<Comparison>(Comparison::Operator::Equal);
    } else if (name_ == ">") {
        return std::make_shared<Comparison>(Comparison::Operator::More);
    } else if (name_ == "<") {
        return std::make_shared<Comparison>(Comparison::Operator::Less);
    } else if (name_ == ">=") {
        return std::make_shared<Comparison>(Comparison::Operator::MoreOrEqual);
    } else if (name_ == "<=") {
        return std::make_shared<Comparison>(Comparison::Operator::LessOrEqual);
    } else if (name_ == "and") {
        return std::make_shared<Logic>(Logic::Operator::And);
    } else if (name_ == "or") {
        return std::make_shared<Logic>(Logic::Operator::Or);
    } else if (name_ == "not") {
        return std::make_shared<Logic>(Logic::Operator::Not);
    } else if (name_ == "quote") {
        return std::make_shared<Quote>();
    } else if (name_ == "min") {
        return std::make_shared<MinMax>(MinMax::Operator::Min);
    } else if (name_ == "max") {
        return std::make_shared<MinMax>(MinMax::Operator::Max);
    } else if (name_ == "car") {
        return std::make_shared<CarCdr>(CarCdr::Operator::Car);
    } else if (name_ == "cdr") {
        return std::make_shared<CarCdr>(CarCdr::Operator::Cdr);
    } else if (name_ == "set-car!") {
        return std::make_shared<CarCdr>(CarCdr::Operator::SetCar);
    } else if (name_ == "set-cdr!") {
        return std::make_shared<CarCdr>(CarCdr::Operator::SetCdr);
    } else if (name_ == "if") {
        return std::make_shared<Checker>(Checker::Operator::If);
    } else if (name_ == "null?") {
        return std::make_shared<Checker>(Checker::Operator::Null);
    } else if (name_ == "pair?") {
        return std::make_shared<Checker>(Checker::Operator::Pair);
    } else if (name_ == "number?") {
        return std::make_shared<Checker>(Checker::Operator::Numb);
    } else if (name_ == "boolean?") {
        return std::make_shared<Checker>(Checker::Operator::Bool);
    } else if (name_ == "symbol?") {
        return std::make_shared<Checker>(Checker::Operator::Symb);
    } else if (name_ == "list?") {
        return std::make_shared<Checker>(Checker::Operator::List);
    } else if (name_ == "abs") {
        return std::make_shared<Abs>();
    } else if (name_ == "cons") {
        return std::make_shared<Cons>();
    } else if (name_ == "define") {
        return std::make_shared<Variable>(Variable::Operator::Define);
    } else if (name_ == "set!") {
        return std::make_shared<Variable>(Variable::Operator::Set);
    } else if (name_ == "list") {
        return std::make_shared<DataStructure>(DataStructure::Operator::List);
    } else if (name_ == "list-tail") {
        return std::make_shared<DataStructure>(DataStructure::Operator::ListTail);
    } else if (name_ == "list-ref") {
        return std::make_shared<DataStructure>(DataStructure::Operator::ListRef);
    } else if (name_ == "lambda") {
        return std::make_shared<Lambda>();
    }
    return nullptr;
}

std::shared_ptr<Object> Cell::GetFirst() const {
    return first_;
}
std::shared_ptr<Object> Cell::GetSecond() const {
    return second_;
}

void Cell::SetFirst(std::shared_ptr<Object> obj) {
    first_ = std::move(obj);
}

void Cell::SetSecond(std::shared_ptr<Object> obj) {
    second_ = std::move(obj);
}

std::shared_ptr<Object> Cell::Evaluate(std::shared_ptr<State> state) {
    if (!first_) {
        throw RuntimeError("Ожидается функция");
    }
    auto func = As<Function>(first_->Evaluate(state));
    if (!func) {
        throw RuntimeError("Ошибка при работе с функцией");
    }

    std::vector<std::shared_ptr<Object>> args;
    if (second_) {
        if (Is<Cell>(second_)) {
            auto cell = As<Cell>(second_);
            if (!Is<Quote>(func) && !Is<Variable>(func)) {
                state_->SetParentState(state);
                cell->ParseVector(state_, &args, func->ShouldRunArgsImmediately());
            } else {
                args.push_back(cell);
            }
        } else if (Is<Number>(second_)) {
            args.push_back(As<Number>(second_));
        } else if (Is<Boolean>(second_)) {
            args.push_back(As<Boolean>(second_));
        } else if (Is<Symbol>(second_)) {
            args.push_back(As<Symbol>(second_));
        }
    }

    return func->Apply(state, std::move(args));
}

std::string Cell::Print() {
    std::string str;
    if (second_ || As<Number>(first_)) {
        str += "(";
    }
    if (first_) {
        str += first_->Print();
        if (auto next = As<Cell>(second_)) {
            while (next) {
                str += " " + next->first_->Print();
                if (next->second_ && !Is<Cell>(next->second_)) {
                    str += " . " + next->second_->Print();
                }
                next = As<Cell>(next->second_);
            }
        } else if (second_) {
            str += " . " + second_->Print();
        }
    }
    if (second_ || As<Number>(first_)) {
        str += ")";
    }
    return str;
}

void Cell::ParseVector(std::shared_ptr<State> state, std::vector<std::shared_ptr<Object>>* args,
                       bool should_run_args_immediately) {
    auto cell = As<Cell>(shared_from_this());
    while (cell) {
        auto arg = cell->GetFirst();
        if (should_run_args_immediately) {
            arg = arg->Evaluate(state);
        }
        args->push_back(arg);
        cell = As<Cell>(cell->GetSecond());
    }
}

std::shared_ptr<Object> Empty::Evaluate(std::shared_ptr<State> state) {
    return shared_from_this();
}
std::string Empty::Print() {
    return "()";
}

std::shared_ptr<Object> Arithmetic::Apply(std::shared_ptr<State> state,
                                          std::vector<std::shared_ptr<Object>>&& args) {
    if (operator_ == Operator::Minus || operator_ == Operator::Divide) {
        if (args.empty()) {
            throw RuntimeError("Отсутствуют аргументы выражения");
        }
    }

    int64_t answer = 0;
    if (operator_ == Operator::Minus || operator_ == Operator::Divide) {
        if (!Is<Number>(args.front())) {
            throw RuntimeError("Ожидалось число");
        }
        answer = As<Number>(args.front())->GetValue();
        args.erase(args.begin());
    } else if (operator_ == Operator::Multiply) {
        answer = 1;
    }

    for (size_t i = 0; i < args.size(); ++i) {
        if (!Is<Number>(args[i])) {
            throw RuntimeError("Ожидалось число");
        }
        switch (operator_) {
            case Operator::Plus:
                answer += As<Number>(args[i])->GetValue();
                break;
            case Operator::Minus:
                answer -= As<Number>(args[i])->GetValue();
                break;
            case Operator::Multiply:
                answer *= As<Number>(args[i])->GetValue();
                break;
            case Operator::Divide:
                answer /= As<Number>(args[i])->GetValue();
                break;
        }
    }

    return std::make_shared<Number>(answer);
}

std::shared_ptr<Object> Comparison::Apply(std::shared_ptr<State> state,
                                          std::vector<std::shared_ptr<Object>>&& args) {
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i - 1]) || !Is<Number>(args[i])) {
            throw RuntimeError("Ожидались числа");
        }
        switch (operator_) {
            case Operator::Equal:
                if (As<Number>(args[i - 1])->GetValue() != As<Number>(args[i])->GetValue()) {
                    return std::make_shared<Boolean>(false);
                }
                break;
            case Operator::More:
                if (As<Number>(args[i - 1])->GetValue() <= As<Number>(args[i])->GetValue()) {
                    return std::make_shared<Boolean>(false);
                }
                break;
            case Operator::Less:
                if (As<Number>(args[i - 1])->GetValue() >= As<Number>(args[i])->GetValue()) {
                    return std::make_shared<Boolean>(false);
                }
                break;
            case Operator::MoreOrEqual:
                if (As<Number>(args[i - 1])->GetValue() < As<Number>(args[i])->GetValue()) {
                    return std::make_shared<Boolean>(false);
                }
                break;
            case Operator::LessOrEqual:
                if (As<Number>(args[i - 1])->GetValue() > As<Number>(args[i])->GetValue()) {
                    return std::make_shared<Boolean>(false);
                }
                break;
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> Logic::Apply(std::shared_ptr<State> state,
                                     std::vector<std::shared_ptr<Object>>&& args) {
    if (operator_ == Operator::Not && args.size() != 1) {
        throw RuntimeError("Для метода Not ожидается ровно один аргумент");
    }

    std::shared_ptr<Object> buffer;
    for (size_t i = 0; i < args.size(); ++i) {
        auto arg = args[i]->Evaluate(state);
        switch (operator_) {
            case Operator::And:
                if (Is<Boolean>(arg) && !(As<Boolean>(arg)->GetValue())) {
                    return std::make_shared<Boolean>(false);
                }
                buffer = arg;
                break;
            case Operator::Or:
                if (Is<Number>(arg)) {
                    return arg;
                }
                if (Is<Boolean>(arg)) {
                    if (!As<Boolean>(arg)->GetValue()) {
                        continue;
                    }
                    return std::make_shared<Boolean>(true);
                }
                throw RuntimeError("Нельзя преобразовать условие в Boolean");
            case Operator::Not:
                if (Is<Number>(arg) || Is<Empty>(arg)) {
                    return std::make_shared<Boolean>(false);
                }
                return std::make_shared<Boolean>(!(As<Boolean>(arg)->GetValue()));
        }
    }
    if (buffer) {
        return buffer;
    }
    return std::make_shared<Boolean>(operator_ == Operator::And);
}

std::shared_ptr<Object> Checker::Apply(std::shared_ptr<State> state,
                                       std::vector<std::shared_ptr<Object>>&& args) {
    switch (operator_) {
        case Operator::If: {
            if (args.size() < 2 || args.size() > 3) {
                throw SyntaxError("Неккоректное условие");
            }
            auto answer = As<Boolean>(args.front()->Evaluate(state));
            if (answer && answer->GetValue()) {
                return args[1]->Evaluate(state);
            }
            return args.size() == 3 ? args[2]->Evaluate(state) : std::make_shared<Empty>();
        }
        case Operator::Null:
            if (args.front() == nullptr) {
                return std::make_shared<Boolean>(false);
            }
            return std::make_shared<Boolean>(Is<Empty>(args.front()));
        case Operator::Pair:
            return std::make_shared<Boolean>(Is<Cell>(args.front()));
        case Operator::Numb:
            return std::make_shared<Boolean>(Is<Number>(args.front()));
        case Operator::Bool:
            return std::make_shared<Boolean>(Is<Boolean>(args.front()));
        case Operator::Symb:
            return std::make_shared<Boolean>(Is<Symbol>(args.front()));
        case Operator::List: {
            auto value = args.front();
            while (value) {
                if (!Is<Cell>(value)) {
                    return std::make_shared<Boolean>(Is<Empty>(value));
                }
                value = As<Cell>(value)->GetSecond();
            }
            break;
        }
    }
    return std::make_shared<Boolean>(true);
}

std::shared_ptr<Object> Quote::Apply(std::shared_ptr<State> state,
                                     std::vector<std::shared_ptr<Object>>&& args) {
    if (args.size() != 1) {
        if (args.empty()) {
            return std::make_shared<Empty>();
        }
        throw SyntaxError("Ожидается один аргумент");
    }
    return args.front();
}

std::shared_ptr<Object> MinMax::Apply(std::shared_ptr<State> state,
                                      std::vector<std::shared_ptr<Object>>&& args) {
    if (args.empty()) {
        throw RuntimeError("Ожидаются аргументы для функции min/max");
    }

    int64_t max = INT_MIN;
    int64_t min = INT_MAX;

    for (auto arg : args) {
        if (!Is<Number>(arg)) {
            throw RuntimeError("Функция min/max работает только с числами");
        }
        auto value = As<Number>(arg)->GetValue();
        min = min > value ? value : min;
        max = max < value ? value : max;
    }

    int64_t answer;
    switch (operator_) {
        case Max:
            answer = max;
            break;
        case Min:
            answer = min;
            break;
    }

    return std::make_shared<Number>(answer);
}

std::shared_ptr<Object> CarCdr::Apply(std::shared_ptr<State> state,
                                      std::vector<std::shared_ptr<Object>>&& args) {
    switch (operator_) {
        case Cdr:
            if (args.size() != 1) {
                RuntimeError("Cdr ожидает ровно 1 аргумент");
            }
            return As<Cell>(args.front())->GetSecond();
        case Car:
            if (args.size() != 1) {
                RuntimeError("Car ожидает ровно 1 аргумент");
            }
            return As<Cell>(args.front())->GetFirst();
        case SetCdr:
            if (args.size() != 2) {
                RuntimeError("SetCdr ожидает ровно 2 аргумент");
            }
            As<Cell>(args.front())->SetSecond(args[1]);
            return args.front();
        case SetCar:
            if (args.size() != 2) {
                RuntimeError("SetCar ожидает ровно 2 аргумент");
            }
            As<Cell>(args.front())->SetFirst(args[1]);
            return args.front();
    }
    return nullptr;
}

std::shared_ptr<Object> Abs::Apply(std::shared_ptr<State> state,
                                   std::vector<std::shared_ptr<Object>>&& args) {
    if (args.size() != 1) {
        throw RuntimeError("Функция abs ожидает ровно один аргумент");
    }
    if (!Is<Number>(args.front())) {
        throw RuntimeError("abs ожидает число");
    }
    auto value = std::abs(As<Number>(args.front())->GetValue());
    return std::make_shared<Number>(value);
}

std::shared_ptr<Object> Cons::Apply(std::shared_ptr<State> state,
                                    std::vector<std::shared_ptr<Object>>&& args) {
    if (args.size() != 2) {
        throw RuntimeError("Функция cons ожидается два аргумента");
    }
    auto new_cell = std::make_shared<Cell>(args[0], args[1]);
    return new_cell;
}

std::shared_ptr<Object> Variable::Apply(std::shared_ptr<State> state,
                                        std::vector<std::shared_ptr<Object>>&& args) {
    if (args.size() != 1 || !Is<Cell>(args.front())) {
        throw SyntaxError("При создании новой переменной ожидается два аргумента");
    }

    auto cell = As<Cell>(args.front());
    if (auto symbol = As<Symbol>(cell->GetFirst())) {
        if (Is<Cell>(cell->GetSecond()) && As<Cell>(cell->GetSecond())->GetSecond()) {
            throw SyntaxError("define ожидал два аргумента");
        }
        auto new_value = As<Cell>(cell->GetSecond())->GetFirst();
        if (operator_ == Operator::Set) {
            auto old_value = state->FindVariable(symbol->GetName());
            if (!old_value) {
                throw NameError("Переменная не была объявлена");
            }
            if (Is<Number>(old_value) && Is<Cell>(new_value)) {
                As<Cell>(new_value)->AddVariable(symbol->GetName(), As<Number>(old_value));
            }
        }

        state->AddVariable(symbol->GetName(), new_value);
    } else if (auto func = As<Cell>(cell->GetFirst())) {
        if (!Is<Symbol>(func->GetFirst())) {
            throw SyntaxError("Ожидалось имя функции");
        }
        std::shared_ptr<Lambda> lambda = std::make_shared<Lambda>();
        std::vector<std::shared_ptr<Object>> lines;
        lines.push_back(std::move(func->GetSecond()));
        if (auto commands = As<Cell>(cell->GetSecond())) {
            commands->ParseVector(state, &lines, false);
        }
        lambda->Apply(state, std::move(lines));
        state->AddVariable(As<Symbol>(func->GetFirst())->GetName(), lambda);
    } else {
        throw SyntaxError("Ошибка создания переменной");
    }

    return std::make_shared<Empty>();
}

std::shared_ptr<Object> Lambda::Apply(std::shared_ptr<State> state,
                                      std::vector<std::shared_ptr<Object>>&& args) {
    if (lines_.empty()) {
        if (args.size() < 2) {
            throw SyntaxError("Lambda функция ожидает минимум 2 блока значений");
        }

        state_->SetParentState(state);
        if (Is<Cell>(args.front())) {
            As<Cell>(args.front())->ParseVector(state_, &parameters_, false);
        }

        lines_.insert(lines_.end(), std::make_move_iterator(args.begin() + 1),
                      std::make_move_iterator(args.end()));
        return shared_from_this();
    } else {
        if (args.size() != parameters_.size()) {
            throw RuntimeError("Количество аргументов не равно количеству ожидаемых параметров");
        }
        for (size_t index = 0; index < parameters_.size(); ++index) {
            if (!Is<Symbol>(parameters_.at(index))) {
                throw SyntaxError("Ожидалось имя переменной в параметрах");
            }
            state_->AddVariable(As<Symbol>(parameters_.at(index))->GetName(),
                                args.at(index)->Evaluate(state));
        }
        std::shared_ptr<Object> result;
        for (size_t index = 0; index < lines_.size(); ++index) {
            result = lines_.at(index)->Evaluate(state_);
        }
        return result;
    }
}

std::shared_ptr<Object> DataStructure::Apply(std::shared_ptr<State> state,
                                             std::vector<std::shared_ptr<Object>>&& args) {
    if (args.empty()) {
        return std::make_shared<Empty>();
    }

    switch (operator_) {
        case Operator::List: {
            std::shared_ptr<Cell> head = std::make_shared<Cell>();
            std::shared_ptr<Cell> tail = head;

            for (size_t i = 0; i < args.size(); ++i) {
                tail->SetFirst(args[i]);
                if (i < args.size() - 1) {
                    tail->SetSecond(std::make_shared<Cell>());
                    tail = As<Cell>(tail->GetSecond());
                }
            }

            return head;
        }
        case Operator::ListRef:
        case Operator::ListTail: {
            if (args.size() != 2 || !Is<Cell>(args[0]) || !Is<Number>(args[1])) {
                throw RuntimeError("ListRef ожидает два аргумента (1 - список, 2 - число)");
            }

            auto cell = args[0];
            auto value = As<Number>(args[1])->GetValue();

            while (value > 0) {
                if (!Is<Cell>(cell)) {
                    throw RuntimeError("В списке меньше значений, чем ожидалось");
                }
                cell = As<Cell>(cell)->GetSecond();
                --value;
            }

            if (!Is<Cell>(cell)) {
                if (!cell && operator_ == Operator::ListTail) {
                    return std::make_shared<Empty>();
                }
                throw RuntimeError("Ожидалось значение другого типа");
            }

            return operator_ == Operator::ListRef ? As<Cell>(cell)->GetFirst() : As<Cell>(cell);
        }
    }

    return nullptr;
}