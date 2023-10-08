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
        Student{"Ivan", "Ivanov", 2000, 12, 31},
        Student{"Ray", "William", 2000, 1, 12},
        Student{"Peter", "Harper", 2010, 5, 30},
        Student{"Ethan", "Mosley", 2000, 12, 31},
    };

    {
        SortStudents(&students, SortType::kByDate);
        CHECK(students == std::vector<Student>({
                              Student{"Ray", "William", 2000, 1, 12},
                              Student{"Ivan", "Ivanov", 2000, 12, 31},
                              Student{"Ethan", "Mosley", 2000, 12, 31},
                              Student{"Peter", "Harper", 2010, 5, 30},
                          }));
    }
    {
        SortStudents(&students, SortType::kByName);
        CHECK(students == std::vector<Student>({
                              Student{"Peter", "Harper", 2010, 5, 30},
                              Student{"Ivan", "Ivanov", 2000, 12, 31},
                              Student{"Ethan", "Mosley", 2000, 12, 31},
                              Student{"Ray", "William", 2000, 1, 12},
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
