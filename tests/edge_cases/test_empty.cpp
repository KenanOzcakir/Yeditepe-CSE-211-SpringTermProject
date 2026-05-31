/**
 * @file test_empty.cpp
 * @brief Edge case tests for empty input scenarios
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/core/SpellChecker.hpp"
#include "../../include/data_structures/Trie.hpp"
#include "../../include/data_structures/LinkedList.hpp"
#include "../../include/core/EditDistance.hpp"

void test_empty_dictionary() {
    printSectionHeader("EDGE CASES");
    std::cout << "Empty Input - Empty Dictionary\n";
    
    // Test: Spell checker with empty dictionary
    // Description: Test behavior when dictionary is empty
    // Input: SpellChecker with no words added
    // Expected: No words are correct, no suggestions generated
    {
        SpellChecker checker;
        assertFalse("test_empty_dict_correct", checker.isWordCorrect("anyword"));
        
        SuggestionBuffer buffer(3);
        int count = checker.suggest("anyword", 2, 3, buffer);
        assertEqual("test_empty_dict_suggestions", 0, count);
    }
}

void test_empty_trie() {
    std::cout << "\nEmpty Input - Empty Trie\n";
    
    // Test: Trie with no words
    // Description: Test operations on empty trie
    // Input: Empty trie
    // Expected: Contains returns false for any word
    {
        Trie trie;
        assertFalse("test_empty_trie_contains", trie.contains("anything"));
        assertFalse("test_empty_trie_contains_empty", trie.contains(""));
    }
}

void test_empty_linkedlist() {
    std::cout << "\nEmpty Input - Empty LinkedList\n";
    
    // Test: LinkedList with no elements
    // Description: Test operations on empty list
    // Input: Empty LinkedList
    // Expected: getHead returns nullptr, find returns nullptr
    {
        LinkedList<int> list;
        assertTrue("test_empty_list_head", list.getHead() == nullptr);
        
        ListNode<int>* found = list.find([](const int& v) { return v == 1; });
        assertTrue("test_empty_list_find", found == nullptr);
    }
}

void test_empty_string_operations() {
    std::cout << "\nEmpty Input - Empty String Operations\n";
    
    // Test: Edit distance with empty strings
    // Description: Test edit distance with empty strings
    // Input: Empty string and non-empty string
    // Expected: Distance equals length of non-empty string
    {
        int dist1 = EditDistance::computeBounded("", "test", 10);
        assertEqual("test_empty_string_ed1", 4, dist1);
        
        int dist2 = EditDistance::computeBounded("test", "", 10);
        assertEqual("test_empty_string_ed2", 4, dist2);
        
        int dist3 = EditDistance::computeBounded("", "", 10);
        assertEqual("test_empty_string_ed3", 0, dist3);
    }
}

void test_empty_suggestion_buffer() {
    std::cout << "\nEmpty Input - Empty Suggestion Buffer\n";
    
    // Test: SuggestionBuffer with zero capacity
    // Description: Test buffer with capacity 0
    // Input: SuggestionBuffer(0)
    // Expected: Size is 0, no suggestions stored
    {
        SuggestionBuffer buffer(0);
        Candidate candidate;
        candidate.word = "hello";
        candidate.distance = 0;
        candidate.frequency = 100;
        buffer.tryAdd(candidate);
        assertEqual("test_empty_buffer_size", 0, buffer.size());
    }
}

void run_empty_tests() {
    test_empty_dictionary();
    test_empty_trie();
    test_empty_linkedlist();
    test_empty_string_operations();
    test_empty_suggestion_buffer();
}
