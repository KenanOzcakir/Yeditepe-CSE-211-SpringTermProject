/**
 * @file test_helpers.hpp
 * @brief Common test utilities and helper functions
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#pragma once

#include <iostream>
#include <string>
#include <cassert>

/**
 * @brief Test result tracking
 */
class TestRunner {
public:
    static int totalTests;
    static int passedTests;
    static int failedTests;

    static void reset() {
        totalTests = 0;
        passedTests = 0;
        failedTests = 0;
    }

    static void printSummary() {
        std::cout << "\n";
        std::cout << "SUMMARY: " << passedTests << "/" << totalTests 
                  << " tests passed | " << failedTests << " failed | 0 skipped\n";
    }
};

/**
 * @brief Assert two values are equal
 * @param testName Name of the test
 * @param expected Expected value
 * @param actual Actual value
 * @return True if equal, false otherwise
 */
template <typename T>
inline bool assertEqual(const std::string& testName, const T& expected, const T& actual) {
    TestRunner::totalTests++;
    if (expected == actual) {
        std::cout << "  " << testName;
        int dots = 60 - static_cast<int>(testName.length());
        for (int i = 0; i < dots && i < 60; ++i) {
            std::cout << ".";
        }
        std::cout << " PASSED\n";
        TestRunner::passedTests++;
        return true;
    } else {
        std::cout << "  " << testName;
        int dots = 60 - static_cast<int>(testName.length());
        for (int i = 0; i < dots && i < 60; ++i) {
            std::cout << ".";
        }
        std::cout << " FAILED (expected=" << expected << ", got=" << actual << ")\n";
        TestRunner::failedTests++;
        return false;
    }
}

/**
 * @brief Assert a condition is true
 * @param testName Name of the test
 * @param condition Condition to check
 * @return True if condition is true, false otherwise
 */
inline bool assertTrue(const std::string& testName, bool condition) {
    TestRunner::totalTests++;
    if (condition) {
        std::cout << "  " << testName;
        int dots = 60 - static_cast<int>(testName.length());
        for (int i = 0; i < dots && i < 60; ++i) {
            std::cout << ".";
        }
        std::cout << " PASSED\n";
        TestRunner::passedTests++;
        return true;
    } else {
        std::cout << "  " << testName;
        int dots = 60 - static_cast<int>(testName.length());
        for (int i = 0; i < dots && i < 60; ++i) {
            std::cout << ".";
        }
        std::cout << " FAILED\n";
        TestRunner::failedTests++;
        return false;
    }
}

/**
 * @brief Assert a condition is false
 * @param testName Name of the test
 * @param condition Condition to check
 * @return True if condition is false, false otherwise
 */
inline bool assertFalse(const std::string& testName, bool condition) {
    return assertTrue(testName, !condition);
}

/**
 * @brief Print test section header
 * @param sectionName Name of the test section
 */
inline void printSectionHeader(const std::string& sectionName) {
    std::cout << "\n[" << sectionName << "]\n";
}

