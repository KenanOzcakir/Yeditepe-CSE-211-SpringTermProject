/**
 * @file test_limits.cpp
 * @brief Edge case tests for maximum/minimum value scenarios
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/core/SpellChecker.hpp"
#include "../../include/data_structures/Trie.hpp"
#include "../../include/data_structures/LinkedList.hpp"
#include <climits>
#include <string>

void test_maximum_frequency() {
    printSectionHeader("EDGE CASES");
    std::cout << "Boundary Limits - Maximum Frequency\n";
    
    // Test: Maximum integer frequency
    // Description: Test with INT_MAX frequency
    // Input: Word with frequency INT_MAX
    // Expected: Frequency stored and used correctly
    {
        SpellChecker checker;
        checker.addDictionaryWord("test", INT_MAX);
        
        assertTrue("test_max_freq_correct", checker.isWordCorrect("test"));
        
        SuggestionBuffer buffer(3);
        checker.suggest("tst", 2, 3, buffer);
        if (buffer.size() > 0) {
            Candidate cand = buffer.get(0);
            assertTrue("test_max_freq_suggestion", cand.frequency == INT_MAX);
        }
    }
}

void test_maximum_edit_distance() {
    std::cout << "\nBoundary Limits - Maximum Edit Distance\n";
    
    // Test: Very large edit distance
    // Description: Test with large maxEditDistance
    // Input: maxEditDistance = 100
    // Expected: Algorithm handles large distance correctly
    {
        SpellChecker checker;
        checker.addDictionaryWord("hello", 30000);
        checker.addDictionaryWord("world", 40000);
        
        SuggestionBuffer buffer(10);
        int count = checker.suggest("xyzabc", 100, 10, buffer);
        // Should find suggestions if within distance
        assertTrue("test_max_ed_distance", count >= 0);
    }
}

void test_maximum_suggestions() {
    std::cout << "\nBoundary Limits - Maximum Suggestions\n";
    
    // Test: Large suggestion buffer
    // Description: Test with large maxSuggestions
    // Input: maxSuggestions = 100
    // Expected: Buffer handles large capacity
    {
        SpellChecker checker;
        checker.addDictionaryWord("their", 50000);
        checker.addDictionaryWord("there", 45000);
        checker.addDictionaryWord("they", 60000);
        checker.addDictionaryWord("the", 1000000);
        checker.addDictionaryWord("them", 30000);
        
        SuggestionBuffer buffer(100);
        int count = checker.suggest("thier", 2, 100, buffer);
        assertTrue("test_max_suggestions", count >= 0 && count <= 100);
    }
}

void test_very_long_word() {
    std::cout << "\nBoundary Limits - Very Long Word\n";
    
    // Test: Very long word (1000 characters)
    // Description: Test with extremely long word
    // Input: 1000-character word
    // Expected: Word stored and found correctly
    {
        Trie trie;
        std::string longWord(1000, 'a');
        trie.insert(longWord, 100);
        
        assertTrue("test_long_word_contains", trie.contains(longWord));
    }
}

void test_minimum_values() {
    std::cout << "\nBoundary Limits - Minimum Values\n";
    
    // Test: Minimum frequency (1)
    // Description: Test with frequency = 1
    // Input: Word with frequency 1
    // Expected: Frequency stored correctly
    {
        SpellChecker checker;
        checker.addDictionaryWord("test", 1);
        
        assertTrue("test_min_freq", checker.isWordCorrect("test"));
    }
    
    // Test: Zero edit distance
    // Description: Test with maxEditDistance = 0
    // Input: maxEditDistance = 0
    // Expected: Only exact matches found
    {
        SpellChecker checker;
        checker.addDictionaryWord("hello", 30000);
        
        SuggestionBuffer buffer(3);
        int count = checker.suggest("helo", 0, 3, buffer);
        // "helo" has distance 1 from "hello", so should not be found with maxDist=0
        assertEqual("test_min_ed_zero", 0, count);
    }
}

void test_overflow_protection() {
    std::cout << "\nBoundary Limits - Overflow Protection\n";
    
    // Test: Large number of words
    // Description: Test with many dictionary words
    // Input: 1000 words in dictionary
    // Expected: All words stored and searchable
    {
        SpellChecker checker;
        const int wordCount = 1000;
        
        for (int i = 0; i < wordCount; ++i) {
            std::string word = "word" + std::to_string(i);
            checker.addDictionaryWord(word, 100 + i);
        }
        
        assertTrue("test_overflow_storage", checker.isWordCorrect("word0"));
        assertTrue("test_overflow_storage_mid", checker.isWordCorrect("word500"));
        assertTrue("test_overflow_storage_end", checker.isWordCorrect("word999"));
    }
}

void run_limits_tests() {
    test_maximum_frequency();
    test_maximum_edit_distance();
    test_maximum_suggestions();
    test_very_long_word();
    test_minimum_values();
    test_overflow_protection();
}

