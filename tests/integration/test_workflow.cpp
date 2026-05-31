/**
 * @file test_workflow.cpp
 * @brief Integration tests for end-to-end workflow
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/core/SpellChecker.hpp"
#include "../../include/utils/Parser.hpp"
#include <string>

void test_load_and_check() {
    printSectionHeader("INTEGRATION");
    std::cout << "Workflow - Load and Check\n";
    
    // Test: Load dictionary and check words
    // Description: Test complete workflow from dictionary loading to word checking
    // Input: Dictionary with words, check existing and non-existing words
    // Expected: Correct words identified, suggestions provided for incorrect
    {
        SpellChecker checker;
        checker.addDictionaryWord("hello", 30000);
        checker.addDictionaryWord("world", 40000);
        checker.addDictionaryWord("their", 50000);
        checker.addDictionaryWord("there", 45000);
        checker.addDictionaryWord("they", 60000);
        
        assertTrue("test_workflow_correct_word", checker.isWordCorrect("hello"));
        assertTrue("test_workflow_correct_word2", checker.isWordCorrect("world"));
        assertFalse("test_workflow_incorrect_word", checker.isWordCorrect("helo"));
        assertFalse("test_workflow_incorrect_word2", checker.isWordCorrect("wrold"));
    }
}

void test_suggestion_generation() {
    std::cout << "\nWorkflow - Suggestion Generation\n";
    
    // Test: Generate suggestions for misspelled word
    // Description: Test that suggestions are generated with correct ranking
    // Input: Dictionary with words, misspelled word "teh"
    // Expected: Suggestions include "the", "they", etc., ranked by distance and frequency
    {
        SpellChecker checker;
        checker.addDictionaryWord("the", 1000000);
        checker.addDictionaryWord("they", 60000);
        checker.addDictionaryWord("there", 45000);
        checker.addDictionaryWord("their", 50000);
        
        SuggestionBuffer buffer(3);
        int count = checker.suggest("teh", 2, 3, buffer);
        
        assertTrue("test_workflow_suggestions_count", count > 0);
        if (count > 0) {
            Candidate first = buffer.get(0);
            assertTrue("test_workflow_suggestions_has_word", !first.word.empty());
            assertTrue("test_workflow_suggestions_has_distance", first.distance >= 0);
        }
    }
}

void test_full_pipeline() {
    std::cout << "\nWorkflow - Full Pipeline\n";
    
    // Test: Complete pipeline from input to suggestions
    // Description: Test entire spell checking pipeline
    // Input: Dictionary words, words to check
    // Expected: Correct workflow execution
    {
        SpellChecker checker;
        
        // Load dictionary
        checker.addDictionaryWord("hello", 30000);
        checker.addDictionaryWord("help", 25000);
        checker.addDictionaryWord("world", 40000);
        checker.addDictionaryWord("their", 50000);
        checker.addDictionaryWord("there", 45000);
        checker.addDictionaryWord("they", 60000);
        
        // Check words
        std::string words[] = {"hello", "helo", "wrold", "thier"};
        bool results[] = {true, false, false, false};
        
        for (int i = 0; i < 4; ++i) {
            bool isCorrect = checker.isWordCorrect(words[i]);
            assertEqual("test_pipeline_word_" + std::to_string(i), results[i], isCorrect);
        }
        
        // Generate suggestions for incorrect words
        SuggestionBuffer buffer(3);
        int count = checker.suggest("helo", 2, 3, buffer);
        assertTrue("test_pipeline_suggestions", count > 0);
    }
}

void test_multiple_suggestions() {
    std::cout << "\nWorkflow - Multiple Suggestions\n";
    
    // Test: Multiple suggestions with ranking
    // Description: Test that multiple suggestions are ranked correctly
    // Input: Dictionary with multiple similar words, misspelled word
    // Expected: Suggestions ranked by distance (primary) and frequency (secondary)
    {
        SpellChecker checker;
        checker.addDictionaryWord("their", 50000);
        checker.addDictionaryWord("there", 45000);
        checker.addDictionaryWord("they", 60000);
        checker.addDictionaryWord("the", 1000000);
        
        SuggestionBuffer buffer(3);
        int count = checker.suggest("thier", 2, 3, buffer);
        
        assertTrue("test_multiple_suggestions_count", count > 0);
        
        // Check that suggestions are sorted (distance ascending, then frequency descending)
        if (count > 1) {
            Candidate first = buffer.get(0);
            Candidate second = buffer.get(1);
            assertTrue("test_multiple_suggestions_sorted", 
                      first.distance <= second.distance);
            if (first.distance == second.distance) {
                assertTrue("test_multiple_suggestions_freq_sorted",
                          first.frequency >= second.frequency);
            }
        }
    }
}

void test_autocomplete_generation() {
    std::cout << "\nWorkflow - Autocomplete Generation\n";

    {
        SpellChecker checker;
        checker.addDictionaryWord("their", 50000);
        checker.addDictionaryWord("there", 45000);
        checker.addDictionaryWord("the", 1000000);
        checker.addDictionaryWord("then", 70000);

        SuggestionBuffer buffer(3);
        int count = checker.autocomplete("the", 3, buffer);

        assertTrue("test_autocomplete_count", count > 0);
        if (count > 0) {
            Candidate first = buffer.get(0);
            assertEqual("test_autocomplete_highest_frequency", std::string("the"), first.word);
        }
    }
}

void test_max_suggestions_limit() {
    std::cout << "\nWorkflow - Max Suggestions Limit\n";

    {
        SpellChecker checker;
        checker.addDictionaryWord("cat", 100);
        checker.addDictionaryWord("cot", 90);
        checker.addDictionaryWord("cut", 80);

        SuggestionBuffer buffer(5);
        int count = checker.suggest("cit", 1, 1, buffer);

        assertEqual("test_max_suggestions_limit_count", 1, count);
        assertEqual("test_max_suggestions_limit_buffer", 1, buffer.size());
    }
}

void test_repeated_query_cache() {
    std::cout << "\nWorkflow - Repeated Query Cache\n";

    {
        SpellChecker checker;
        checker.addDictionaryWord("their", 50000);
        checker.addDictionaryWord("there", 45000);
        checker.addDictionaryWord("the", 1000000);

        SuggestionBuffer first(2);
        SuggestionBuffer second(2);
        int firstCount = checker.suggest("thier", 2, 2, first);
        int secondCount = checker.suggest("thier", 2, 2, second);

        assertEqual("test_repeated_query_count", firstCount, secondCount);
        if (firstCount > 0 && secondCount > 0) {
            assertEqual("test_repeated_query_first_word", first.get(0).word, second.get(0).word);
        }
    }
}

void run_workflow_tests() {
    test_load_and_check();
    test_suggestion_generation();
    test_full_pipeline();
    test_multiple_suggestions();
    test_autocomplete_generation();
    test_max_suggestions_limit();
    test_repeated_query_cache();
}
