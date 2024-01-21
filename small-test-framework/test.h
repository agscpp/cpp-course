#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <string>

class AbstractTest {
public:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual void Run() = 0;
    virtual ~AbstractTest() = default;
};

class AbstractTestFactory {
public:
    virtual std::unique_ptr<AbstractTest> Create() = 0;
    virtual ~AbstractTestFactory() = default;
};

template <typename T>
class TestFactory : public AbstractTestFactory {
public:
    std::unique_ptr<AbstractTest> Create() override {
        return std::make_unique<T>();
    }
};

class Predicate {
public:
    virtual bool operator()(const std::string& str) const = 0;
    virtual ~Predicate() = default;
};

class FullMatch : public Predicate {
public:
    FullMatch(const std::string& str) : str_(str) {
    }

    bool operator()(const std::string& other) const override {
        return str_ == other;
    }

private:
    std::string str_;
};

class Substr : public Predicate {
public:
    Substr(const std::string& substr) : substr_(substr) {
    }

    bool operator()(const std::string& str) const override {
        return str.find(substr_) != std::string::npos;
    }

private:
    std::string substr_;
};

class TestRegistry {
public:
    static TestRegistry& Instance() {
        static TestRegistry test_registry;
        return test_registry;
    }

    template <class TestClass>
    void RegisterClass(const std::string& class_name) {
        auto hash = typeid(TestClass).hash_code();
        if (hash_tests_.contains(hash) || tests_.find(class_name) != tests_.end()) {
            throw std::runtime_error("Test has already been registered");
        }
        tests_[class_name] = std::make_unique<TestFactory<TestClass>>();
        hash_tests_.insert(hash);
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string& class_name) {
        auto it = tests_.find(class_name);
        if (it == tests_.end()) {
            throw std::out_of_range("Not found");
        }
        return it->second->Create();
    }

    void RunTest(const std::string& test_name) {
        std::unique_ptr<AbstractTest> test(CreateTest(test_name));
        test->SetUp();

        auto complete = [&test]() { test->TearDown(); };

        try {
            test->Run();
        } catch (...) {
            complete();
            throw;
        }

        complete();
    }

    template <class Predicate>
    std::vector<std::string> ShowTests(Predicate callback) const {
        std::vector<std::string> test_names;
        for (const auto& [name, test] : tests_) {
            if (callback(name)) {
                test_names.push_back(name);
            }
        }
        return test_names;
    }

    std::vector<std::string> ShowAllTests() const {
        std::vector<std::string> test_names;
        for (const auto& [name, test] : tests_) {
            test_names.push_back(name);
        }
        return test_names;
    }

    template <class Predicate>
    void RunTests(Predicate callback) {
        for (const auto& [name, test] : tests_) {
            if (callback(name)) {
                RunTest(name);
            }
        }
    }

    void Clear() {
        hash_tests_.clear();
        tests_.clear();
    }

private:
    std::map<std::string, std::unique_ptr<AbstractTestFactory>> tests_;
    std::unordered_set<std::size_t> hash_tests_;

    TestRegistry() = default;

    TestRegistry(const TestRegistry& other) = delete;
    TestRegistry(TestRegistry&& other) = delete;

    TestRegistry& operator=(const TestRegistry& other) = delete;
    TestRegistry& operator=(TestRegistry&& other) = delete;
};
