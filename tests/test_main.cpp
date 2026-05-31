/**
 * @file test_main.cpp
 * @brief Main test runner for all test suites
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include <ctime>
#include <iostream>
#include "fixtures/test_helpers.hpp"

// Unit tests
extern void run_linkedlist_tests();
extern void run_trie_tests();
extern void run_editdistance_tests();

// Integration tests
extern void run_workflow_tests();
extern void run_io_tests();

// Edge case tests
extern void run_empty_tests();
extern void run_single_tests();
extern void run_limits_tests();

// Performance tests
extern void run_stress_tests();
extern void run_memory_tests();

int main() {
    std::cout
        << "===============================================================================\n";
    std::cout << "                              PROJECT TEST SUITE\n";
    std::cout
        << "===============================================================================\n";

    clock_t start = clock();
    TestRunner::reset();

    // Unit tests
    printSectionHeader("UNIT");
    std::cout << "Data Structures\n";
    run_linkedlist_tests();
    run_trie_tests();

    std::cout << "\nAlgorithms\n";
    run_editdistance_tests();

    // Integration tests
    printSectionHeader("INTEGRATION");
    std::cout << "Workflow\n";
    run_workflow_tests();

    std::cout << "\nI/O\n";
    run_io_tests();

    // Edge case tests
    printSectionHeader("EDGE CASES");
    std::cout << "Boundary Conditions\n";
    run_empty_tests();
    run_single_tests();
    run_limits_tests();

    // Performance tests
    printSectionHeader("PERFORMANCE");
    std::cout << "Stress Tests\n";
    run_stress_tests();

    std::cout << "\nMemory Usage\n";
    run_memory_tests();

    clock_t end = clock();
    double timeElapsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    // Print summary
    std::cout << "\n";
    std::cout
        << "===============================================================================\n";
    TestRunner::printSummary();
    std::cout << "TIME: " << timeElapsed << " seconds\n";
    std::cout
        << "===============================================================================\n";

    return (TestRunner::failedTests == 0) ? 0 : 1;
}
