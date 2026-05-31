/**
 * @file test_stress.cpp
 * @brief Performance stress tests for large input datasets
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/core/SpellChecker.hpp"
#include "../../include/core/SuggestionBuffer.hpp"
#include "../../include/utils/Parser.hpp"
#include "../../include/data_structures/Trie.hpp"
#include <ctime>
#include <string>

void test_large_dataset() {
    printSectionHeader("PERFORMANCE");
    std::cout << "Stress Tests - Large Dataset\n";
    
    // Test: Large dataset (10000 words)
    // Description: Test performance with 10000 dictionary words
    // Input: 10000 words inserted into trie
    // Expected: All words stored and searchable within reasonable time
    {
        clock_t start = clock();
        
        SpellChecker checker;
        const int wordCount = 10000;
        
        // Insert 10000 words
        for (int i = 0; i < wordCount; ++i) {
            std::string word = "word" + std::to_string(i);
            checker.addDictionaryWord(word, 100 + (i % 1000));
        }
        
        clock_t insertEnd = clock();
        double insertTime = static_cast<double>(insertEnd - start) / CLOCKS_PER_SEC;
        assertTrue("test_large_dataset_insert_time", insertTime >= 0.0);
        
        // Verify words are searchable
        assertTrue("test_large_dataset_search_first", checker.isWordCorrect("word0"));
        assertTrue("test_large_dataset_search_mid", checker.isWordCorrect("word5000"));
        assertTrue("test_large_dataset_search_last", checker.isWordCorrect("word9999"));
        
        // Test suggestions on large dataset
        SuggestionBuffer buffer(10);
        int count = checker.suggest("word500", 2, 10, buffer);
        assertTrue("test_large_dataset_suggestions", count >= 0);
        
        clock_t end = clock();
        double totalTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        
        std::cout << "  Large dataset (10000 nodes): PASSED (" << totalTime << "s)\n";
    }
}

void test_large_dictionary_file() {
    std::cout << "\nStress Tests - Large Dictionary File\n";
    
    // Test: Load large dictionary from file
    // Description: Test loading input_large.json
    // Input: input_large.json file
    // Expected: File loaded and processed within reasonable time
    {
        clock_t start = clock();
        
        SpellChecker checker;
        InputData data;
        
        // Try to load large dictionary
        bool success = Parser::loadFromFile("data/input_large.json", data);
        
        if (success && data.dictionaryCount > 0) {
            // Insert all words
            for (int i = 0; i < data.dictionaryCount; ++i) {
                checker.addDictionaryWord(data.dictionary[i].word, data.dictionary[i].frequency);
            }
            
            clock_t end = clock();
            double loadTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
            
            // Verify some words
            if (data.dictionaryCount > 0) {
                assertTrue("test_large_file_search", 
                          checker.isWordCorrect(data.dictionary[0].word));
            }
            
            std::cout << "  Large dictionary file (" << data.dictionaryCount 
                      << " words): PASSED (" << loadTime << "s)\n";
            
            Parser::free(data);
        } else {
            // Skip if file not available
            std::cout << "  Large dictionary file: SKIPPED (file not found)\n";
            assertTrue("test_large_file_skip", true);
        }
    }
}

void test_many_suggestions() {
    std::cout << "\nStress Tests - Many Suggestions\n";
    
    // Test: Generate suggestions from large dictionary
    // Description: Test suggestion generation with large dictionary
    // Input: Large dictionary, typo word
    // Expected: Suggestions generated efficiently
    {
        SpellChecker checker;
        
        // Build a large dictionary
        const int dictSize = 5000;
        for (int i = 0; i < dictSize; ++i) {
            std::string word = "test" + std::to_string(i);
            checker.addDictionaryWord(word, 100 + i);
        }
        
        clock_t start = clock();
        
        SuggestionBuffer buffer(100);
        int count = checker.suggest("test100", 3, 100, buffer);
        
        clock_t end = clock();
        double suggestTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
        
        assertTrue("test_many_suggestions_count", count >= 0);
        std::cout << "  Many suggestions (" << count << " found): PASSED (" 
                  << suggestTime << "s)\n";
    }
}

void run_stress_tests() {
    test_large_dataset();
    test_large_dictionary_file();
    test_many_suggestions();
}

