#include <sort_students.h>

#include <catch2/catch_test_macros.hpp>
#include <vector>

TEST_CASE("Signature") {
    INFO("Do not change SortStydents signature");
    STATIC_CHECK(
        std::is_same_v<decltype(&SortStudents), void (*)(std::vector<Student>*, SortType)>);
}

TEST_CASE("Simple") {
    std::vector<Student> students = {
        Student{"Ivan", "Ivanov", 31, 12, 2000},
        Student{"Ray", "William", 12, 1, 2000},
        Student{"Peter", "Harper", 30, 5, 2010},
        Student{"Ethan", "Mosley", 31, 12, 2000},
    };

    {
        SortStudents(&students, SortType::kByDate);
        CHECK(students == std::vector<Student>({
                              Student{"Ray", "William", 12, 1, 2000},
                              Student{"Ivan", "Ivanov", 31, 12, 2000},
                              Student{"Ethan", "Mosley", 31, 12, 2000},
                              Student{"Peter", "Harper", 30, 5, 2010},
                          }));
    }
    {
        SortStudents(&students, SortType::kByName);
        CHECK(students == std::vector<Student>({
                              Student{"Peter", "Harper", 30, 5, 2010},
                              Student{"Ivan", "Ivanov", 31, 12, 2000},
                              Student{"Ethan", "Mosley", 31, 12, 2000},
                              Student{"Ray", "William", 12, 1, 2000},
                          }));
    }
}

TEST_CASE("Empty") {
    {
        std::vector<Student> students;
        SortStudents(&students, SortType::kByDate);
        SortStudents(&students, SortType::kByName);
        CHECK(students.empty());
    }
}
