/**
 * @file test_filter.cpp
 * @brief Юнит-тесты для модуля фильтрации TextFilter
 * 
 * Для компиляции требуется GoogleTest.
 * Сборка: g++ test_filter.cpp -lgtest -lgtest_main -pthread
 */

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>

/**
 * @brief Класс-заглушка для фильтра (в реальном проекте импортируем настоящий)
 */
class TextFilterMock {
public:
    static std::vector<std::string> filterBySubstring(
        const std::vector<std::string>& input, 
        const std::string& pattern) {
        
        std::vector<std::string> result;
        for (const auto& item : input) {
            if (item.find(pattern) != std::string::npos) {
                result.push_back(item);
            }
        }
        return result;
    }
    
    static bool isCaseSensitive = true;
};

/**
 * @brief Тест 1: Фильтрация по подстроке
 */
TEST(FilterTest, FilterBySubstring) {
    std::vector<std::string> input = {"яблоко", "банан", "яблочный сок"};
    std::string pattern = "яблоко";
    
    auto result = TextFilterMock::filterBySubstring(input, pattern);
    
    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "яблоко");
    EXPECT_EQ(result[1], "яблочный сок");
}

/**
 * @brief Тест 2: Пустой фильтр (возвращает всё)
 */
TEST(FilterTest, EmptyPattern) {
    std::vector<std::string> input = {"a", "b", "c"};
    std::string pattern = "";
    
    auto result = TextFilterMock::filterBySubstring(input, pattern);
    
    EXPECT_EQ(result.size(), 3);
}

/**
 * @brief Тест 3: Фильтр не находит совпадений
 */
TEST(FilterTest, NoMatches) {
    std::vector<std::string> input = {"яблоко", "банан", "апельсин"};
    std::string pattern = "груша";
    
    auto result = TextFilterMock::filterBySubstring(input, pattern);
    
    EXPECT_EQ(result.size(), 0);
}

/**
 * @brief Тест 4: Регистрозависимость
 */
TEST(FilterTest, CaseSensitivity) {
    std::vector<std::string> input = {"ЯБЛОКО", "яблоко", "Банан"};
    std::string pattern = "яблоко";
    
    auto result = TextFilterMock::filterBySubstring(input, pattern);
    
    if (TextFilterMock::isCaseSensitive) {
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0], "яблоко");
    } else {
        EXPECT_EQ(result.size(), 2);
    }
}

/**
 * @brief Главная функция запуска тестов
 */
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
