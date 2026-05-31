/**
 * @file test_editdistance.cpp
 * @brief Unit tests for EditDistance algorithm
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/core/EditDistance.hpp"
#include <string>

void test_editdistance_basic() {
    printSectionHeader("UNIT");
    std::cout << "EditDistance Basic Operations\n";
    
    // Test: Exact match
    // Description: Test distance between identical strings
    // Input: "cat" and "cat"
    // Expected: Distance = 0
    {
        int dist = EditDistance::computeBounded("cat", "cat", 10);
        assertEqual("test_ed_exact_match", 0, dist);
    }
    
    // Test: Single substitution
    // Description: Test one character substitution
    // Input: "cat" and "cut"
    // Expected: Distance = 1
    {
        int dist = EditDistance::computeBounded("cat", "cut", 10);
        assertEqual("test_ed_substitution", 1, dist);
    }
    
    // Test: Single insertion
    // Description: Test one character insertion
    // Input: "cat" and "cats"
    // Expected: Distance = 1
    {
        int dist = EditDistance::computeBounded("cat", "cats", 10);
        assertEqual("test_ed_insertion", 1, dist);
    }
    
    // Test: Single deletion
    // Description: Test one character deletion
    // Input: "cats" and "cat"
    // Expected: Distance = 1
    {
        int dist = EditDistance::computeBounded("cats", "cat", 10);
        assertEqual("test_ed_deletion", 1, dist);
    }
}

void test_editdistance_multiple_operations() {
    std::cout << "\nEditDistance Multiple Operations\n";
    
    // Test: Multiple substitutions
    // Description: Test multiple character substitutions
    // Input: "kitten" and "sitting"
    // Expected: Distance = 3 (k->s, e->i, n->g)
    {
        int dist = EditDistance::computeBounded("kitten", "sitting", 10);
        assertEqual("test_ed_multiple_ops", 3, dist);
    }
    
    // Test: Mixed operations
    // Description: Test combination of insert, delete, substitute
    // Input: "saturday" and "sunday"
    // Expected: Distance = 3
    {
        int dist = EditDistance::computeBounded("saturday", "sunday", 10);
        assertEqual("test_ed_mixed_ops", 3, dist);
    }
}

void test_editdistance_bounded() {
    std::cout << "\nEditDistance Bounded Computation\n";
    
    // Test: Distance within bound
    // Description: Test when distance is less than maxDist
    // Input: "cat" and "cut" with maxDist=2
    // Expected: Returns actual distance (1)
    {
        int dist = EditDistance::computeBounded("cat", "cut", 2);
        assertEqual("test_ed_within_bound", 1, dist);
    }
    
    // Test: Distance exceeds bound
    // Description: Test when distance exceeds maxDist
    // Input: "kitten" and "sitting" with maxDist=2
    // Expected: Returns maxDist+1 (3)
    {
        int dist = EditDistance::computeBounded("kitten", "sitting", 2);
        assertEqual("test_ed_over_bound", 3, dist); // Should return 3 (maxDist+1)
    }
    
    // Test: Early termination
    // Description: Test that algorithm terminates early when bound exceeded
    // Input: Very different strings with small maxDist
    // Expected: Returns maxDist+1 quickly
    {
        int dist = EditDistance::computeBounded("abcdefghij", "klmnopqrst", 2);
        assertEqual("test_ed_early_termination", 3, dist); // maxDist+1
    }
}

void test_editdistance_edge_cases() {
    std::cout << "\nEditDistance Edge Cases\n";
    
    // Test: Empty strings
    // Description: Test with empty strings
    // Input: "" and "test"
    // Expected: Distance = 4 (length of "test")
    {
        int dist = EditDistance::computeBounded("", "test", 10);
        assertEqual("test_ed_empty_first", 4, dist);
        
        dist = EditDistance::computeBounded("test", "", 10);
        assertEqual("test_ed_empty_second", 4, dist);
        
        dist = EditDistance::computeBounded("", "", 10);
        assertEqual("test_ed_both_empty", 0, dist);
    }
    
    // Test: Single character
    // Description: Test with single character strings
    // Input: "a" and "b"
    // Expected: Distance = 1
    {
        int dist = EditDistance::computeBounded("a", "b", 10);
        assertEqual("test_ed_single_char", 1, dist);
    }
    
    // Test: Very long strings
    // Description: Test with long strings
    // Input: Two 100-character strings
    // Expected: Computes distance correctly
    {
        std::string s1(100, 'a');
        std::string s2(100, 'b');
        int dist = EditDistance::computeBounded(s1, s2, 200);
        assertEqual("test_ed_long_strings", 100, dist);
    }
    
    // Test: Identical long strings
    // Description: Test with long identical strings
    // Input: Two identical 100-character strings
    // Expected: Distance = 0
    {
        std::string s(100, 'a');
        int dist = EditDistance::computeBounded(s, s, 200);
        assertEqual("test_ed_identical_long", 0, dist);
    }
}

void test_editdistance_boundary() {
    std::cout << "\nEditDistance Boundary Conditions\n";
    
    // Test: Zero maxDist
    // Description: Test with maxDist = 0
    // Input: "a" and "b" with maxDist=0
    // Expected: Returns 1 (maxDist+1) since distance is 1
    {
        int dist = EditDistance::computeBounded("a", "b", 0);
        assertEqual("test_ed_boundary_zero_max", 1, dist);
    }
    
    // Test: Large length difference
    // Description: Test when length difference exceeds maxDist
    // Input: "a" and "abcdefghij" with maxDist=2
    // Expected: Returns maxDist+1 (early exit)
    {
        int dist = EditDistance::computeBounded("a", "abcdefghij", 2);
        assertEqual("test_ed_boundary_length_diff", 3, dist); // maxDist+1
    }
}

void run_editdistance_tests() {
    test_editdistance_basic();
    test_editdistance_multiple_operations();
    test_editdistance_bounded();
    test_editdistance_edge_cases();
    test_editdistance_boundary();
}

