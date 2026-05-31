/**
 * @file test_io.cpp
 * @brief Integration tests for input/output handling
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/utils/Parser.hpp"
#include <cstdio>
#include <fstream>
#include <string>

void test_file_parsing() {
    printSectionHeader("INTEGRATION");
    std::cout << "I/O - File Parsing\n";
    
    // Test: Parse valid input file
    // Description: Test parsing a valid JSON-like input file
    // Input: Valid input file with dictionary and check words
    // Expected: Data parsed correctly
    {
        InputData data;
        bool success = Parser::loadFromFile("data/input_sample.json", data);
        
        assertTrue("test_io_parse_success", success);
        if (success) {
            assertTrue("test_io_parse_dictionary_count", data.dictionaryCount > 0);
            assertTrue("test_io_parse_check_count", data.checkCount > 0);
            assertTrue("test_io_parse_max_distance", data.maxEditDistance > 0);
            assertTrue("test_io_parse_max_suggestions", data.maxSuggestions > 0);
            
            Parser::free(data);
        }
    }
}

void test_file_not_found() {
    std::cout << "\nI/O - File Not Found\n";
    
    // Test: Handle non-existent file
    // Description: Test behavior when file doesn't exist
    // Input: Non-existent file path
    // Expected: Returns false, no crash
    {
        InputData data;
        bool success = Parser::loadFromFile("data/nonexistent.json", data);
        assertFalse("test_io_file_not_found", success);
    }
}

void test_parse_dictionary_entries() {
    std::cout << "\nI/O - Parse Dictionary Entries\n";
    
    // Test: Parse dictionary entries correctly
    // Description: Test that dictionary words and frequencies are parsed
    // Input: Valid input file
    // Expected: Dictionary entries have correct words and frequencies
    {
        InputData data;
        bool success = Parser::loadFromFile("data/input_sample.json", data);
        
        if (success && data.dictionaryCount > 0) {
            assertTrue("test_io_dict_entry_word", !data.dictionary[0].word.empty());
            assertTrue("test_io_dict_entry_freq", data.dictionary[0].frequency > 0);
            
            Parser::free(data);
        } else {
            assertTrue("test_io_dict_entry_skip", true); // Skip if file not available
        }
    }
}

void test_parse_check_words() {
    std::cout << "\nI/O - Parse Check Words\n";
    
    // Test: Parse check words correctly
    // Description: Test that words to check are parsed
    // Input: Valid input file
    // Expected: Check words array populated correctly
    {
        InputData data;
        bool success = Parser::loadFromFile("data/input_sample.json", data);
        
        if (success && data.checkCount > 0) {
            assertTrue("test_io_check_word", !data.checkWords[0].empty());
            
            Parser::free(data);
        } else {
            assertTrue("test_io_check_word_skip", true); // Skip if file not available
        }
    }
}

void test_memory_cleanup() {
    std::cout << "\nI/O - Memory Cleanup\n";
    
    // Test: Memory is properly freed
    // Description: Test that Parser::free() deallocates memory
    // Input: Parsed data
    // Expected: No memory leaks after free()
    {
        InputData data;
        bool success = Parser::loadFromFile("data/input_sample.json", data);
        
        if (success) {
            Parser::free(data);
            assertTrue("test_io_memory_cleanup", 
                      data.dictionary == nullptr && data.checkWords == nullptr);
        } else {
            assertTrue("test_io_memory_cleanup_skip", true);
        }
    }
}

void test_parse_inline_pdf_style_input() {
    std::cout << "\nI/O - Inline PDF Style Input\n";

    {
        const std::string path = "data/test_inline_tmp.json";
        std::ofstream out(path.c_str());
        out << "{\n";
        out << "  \"dictionary\": [{\"word\": \"hello\", \"frequency\": 100}, {\"word\": \"world\", \"frequency\": 80}],\n";
        out << "  \"check_words\": [\"helo\", \"wrld\"],\n";
        out << "  \"max_edit_distance\": 2,\n";
        out << "  \"max_suggestions\": 5\n";
        out << "}\n";
        out.close();

        InputData data;
        bool success = Parser::loadFromFile(path, data);

        assertTrue("test_io_inline_parse_success", success);
        if (success) {
            assertEqual("test_io_inline_dict_count", 2, data.dictionaryCount);
            assertEqual("test_io_inline_check_count", 2, data.checkCount);
            assertEqual("test_io_inline_first_word", std::string("hello"), data.dictionary[0].word);
            assertEqual("test_io_inline_first_check", std::string("helo"), data.checkWords[0]);
            assertEqual("test_io_inline_max_suggestions", 5, data.maxSuggestions);
            Parser::free(data);
        }

        std::remove(path.c_str());
    }
}

void run_io_tests() {
    test_file_parsing();
    test_file_not_found();
    test_parse_dictionary_entries();
    test_parse_check_words();
    test_memory_cleanup();
    test_parse_inline_pdf_style_input();
}
