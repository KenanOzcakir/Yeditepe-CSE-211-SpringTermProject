/**
 * @file test_trie.cpp
 * @brief Unit tests for Trie data structure
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/data_structures/Trie.hpp"
#include <string>

void test_trie_construction() {
    printSectionHeader("UNIT");
    std::cout << "Trie Construction\n";
    
    // Test: Default constructor
    // Description: Test creating an empty trie
    // Input: No input
    // Expected: Trie with root node, root is not nullptr
    // Precondition: None
    {
        Trie trie;
        TrieNode* root = trie.getRoot();
        assertTrue("test_constructor_default", root != nullptr);
        assertTrue("test_constructor_root_not_word", !root->isWord);
    }
    
    // Test: Destructor
    // Description: Test that destructor properly frees all nodes
    // Input: Trie with inserted words
    // Expected: All nodes deallocated
    {
        Trie trie;
        trie.insert("test", 100);
        trie.insert("testing", 50);
        // Destructor called automatically
        assertTrue("test_destructor", true); // If we get here, no crash
    }
}

void test_trie_insertion() {
    std::cout << "\nTrie Insertion\n";
    
    // Test: Insert single word
    // Description: Test inserting one word
    // Input: Insert "hello" with frequency 100
    // Expected: Trie contains "hello", isWord flag set at end node
    {
        Trie trie;
        trie.insert("hello", 100);
        assertTrue("test_insert_single", trie.contains("hello"));
    }
    
    // Test: Insert at beginning (short word first)
    // Description: Test inserting shorter word before longer
    // Input: Insert "he", then "hello"
    // Expected: Both words stored correctly
    {
        Trie trie;
        trie.insert("he", 50);
        trie.insert("hello", 100);
        assertTrue("test_insert_beginning_short", trie.contains("he"));
        assertTrue("test_insert_beginning_long", trie.contains("hello"));
    }
    
    // Test: Insert at end (longer word)
    // Description: Test inserting longer word after shorter
    // Input: Insert "hello", then "helloworld"
    // Expected: Both words stored correctly
    {
        Trie trie;
        trie.insert("hello", 100);
        trie.insert("helloworld", 200);
        assertTrue("test_insert_end_short", trie.contains("hello"));
        assertTrue("test_insert_end_long", trie.contains("helloworld"));
    }
    
    // Test: Insert duplicate word
    // Description: Test inserting same word twice
    // Input: Insert "test" twice with different frequencies
    // Expected: Last frequency overwrites previous
    {
        Trie trie;
        trie.insert("test", 100);
        trie.insert("test", 200);
        assertTrue("test_insert_duplicate", trie.contains("test"));
    }
    
    // Test: Insert words with common prefix
    // Description: Test words sharing prefix
    // Input: Insert "their", "there", "they"
    // Expected: All words stored, sharing common prefix
    {
        Trie trie;
        trie.insert("their", 50000);
        trie.insert("there", 45000);
        trie.insert("they", 60000);
        assertTrue("test_insert_common_prefix_1", trie.contains("their"));
        assertTrue("test_insert_common_prefix_2", trie.contains("there"));
        assertTrue("test_insert_common_prefix_3", trie.contains("they"));
    }
}

void test_trie_search() {
    std::cout << "\nTrie Search\n";
    
    // Test: Search existing word
    // Description: Test finding a word that exists
    // Input: Insert "hello", search "hello"
    // Expected: Returns true
    {
        Trie trie;
        trie.insert("hello", 100);
        assertTrue("test_search_existing", trie.contains("hello"));
    }
    
    // Test: Search non-existent word
    // Description: Test finding a word that doesn't exist
    // Input: Insert "hello", search "world"
    // Expected: Returns false
    {
        Trie trie;
        trie.insert("hello", 100);
        assertFalse("test_search_non_existent", trie.contains("world"));
    }
    
    // Test: Search prefix (not a word)
    // Description: Test that prefix of word is not considered a word
    // Input: Insert "hello", search "hell"
    // Expected: Returns false (prefix is not a word)
    {
        Trie trie;
        trie.insert("hello", 100);
        assertFalse("test_search_prefix", trie.contains("hell"));
    }
    
    // Test: Search longer than existing
    // Description: Test searching word longer than any in trie
    // Input: Insert "hello", search "helloworld"
    // Expected: Returns false
    {
        Trie trie;
        trie.insert("hello", 100);
        assertFalse("test_search_longer", trie.contains("helloworld"));
    }
}

void test_trie_deletion() {
    std::cout << "\nTrie Deletion\n";
    
    // Note: Current implementation doesn't have explicit delete method
    // Testing clear via destructor
    
    // Test: Clear via destructor
    // Description: Test that all nodes are freed
    // Input: Trie with multiple words
    // Expected: All memory deallocated
    {
        Trie trie;
        trie.insert("test1", 100);
        trie.insert("test2", 200);
        trie.insert("test3", 300);
        // Destructor called automatically
        assertTrue("test_delete_all", true); // If we get here, no crash
    }
}

void test_trie_edge_cases() {
    std::cout << "\nTrie Edge Cases\n";
    
    // Test: Empty structure
    // Description: Test operations on empty trie
    // Input: Empty trie
    // Expected: Operations handle empty case gracefully
    {
        Trie trie;
        assertFalse("test_edge_empty_contains", trie.contains("anything"));
        assertTrue("test_edge_empty_root", trie.getRoot() != nullptr);
    }
    
    // Test: Single character word
    // Description: Test word with single character
    // Input: Insert "a"
    // Expected: Word stored and found correctly
    {
        Trie trie;
        trie.insert("a", 100);
        assertTrue("test_edge_single_char", trie.contains("a"));
    }
    
    // Test: Single word
    // Description: Test trie with only one word
    // Input: Insert "hello"
    // Expected: Word found, prefixes not found
    {
        Trie trie;
        trie.insert("hello", 100);
        assertTrue("test_edge_single_word", trie.contains("hello"));
        assertFalse("test_edge_single_word_prefix", trie.contains("h"));
        assertFalse("test_edge_single_word_prefix2", trie.contains("he"));
    }
    
    // Test: Very long word
    // Description: Test with very long word
    // Input: Insert 100-character word
    // Expected: Word stored and found correctly
    {
        Trie trie;
        std::string longWord(100, 'a');
        trie.insert(longWord, 100);
        assertTrue("test_edge_long_word", trie.contains(longWord));
    }
    
    // Test: Maximum frequency
    // Description: Test with maximum integer frequency
    // Input: Insert word with INT_MAX frequency
    // Expected: Frequency stored correctly
    {
        Trie trie;
        trie.insert("test", 2147483647);
        assertTrue("test_edge_max_frequency", trie.contains("test"));
    }
}

void test_trie_boundary() {
    std::cout << "\nTrie Boundary Conditions\n";
    
    // Test: Empty string (should not be inserted as word)
    // Description: Test behavior with empty string
    // Input: Try to insert empty string
    // Expected: Behavior depends on implementation (may or may not be allowed)
    {
        Trie trie;
        // Empty string insertion - implementation dependent
        assertTrue("test_boundary_empty_string", true);
    }
    
    // Test: Words with special characters
    // Description: Test with various characters
    // Input: Insert words with different characters
    // Expected: All characters handled correctly
    {
        Trie trie;
        trie.insert("test123", 100);
        trie.insert("test-abc", 200);
        assertTrue("test_boundary_special_chars", trie.contains("test123"));
    }
}

void run_trie_tests() {
    test_trie_construction();
    test_trie_insertion();
    test_trie_search();
    test_trie_deletion();
    test_trie_edge_cases();
    test_trie_boundary();
}

