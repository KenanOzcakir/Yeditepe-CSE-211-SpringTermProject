/**
 * @file test_single.cpp
 * @brief Edge case tests for single element scenarios
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/core/SpellChecker.hpp"
#include "../../include/data_structures/Trie.hpp"
#include "../../include/data_structures/LinkedList.hpp"

void test_single_word_dictionary() {
    printSectionHeader("EDGE CASES");
    std::cout << "Single Element - Single Word Dictionary\n";
    
    // Test: Dictionary with only one word
    // Description: Test spell checker with single word
    // Input: Dictionary with "hello" only
    // Expected: Only "hello" is correct, suggestions work for other words
    {
        SpellChecker checker;
        checker.addDictionaryWord("hello", 30000);
        
        assertTrue("test_single_dict_correct", checker.isWordCorrect("hello"));
        assertFalse("test_single_dict_incorrect", checker.isWordCorrect("world"));
        
        SuggestionBuffer buffer(3);
        int count = checker.suggest("helo", 2, 3, buffer);
        assertTrue("test_single_dict_suggestions", count > 0);
    }
}

void test_single_element_linkedlist() {
    std::cout << "\nSingle Element - Single Element LinkedList\n";
    
    // Test: LinkedList with one element
    // Description: Test operations on list with single element
    // Input: List with [42]
    // Expected: Operations work correctly
    {
        LinkedList<int> list;
        list.append(42);
        
        ListNode<int>* head = list.getHead();
        assertTrue("test_single_list_head", head != nullptr && head->value == 42);
        assertTrue("test_single_list_next", head->next == nullptr);
        
        ListNode<int>* found = list.find([](const int& v) { return v == 42; });
        assertTrue("test_single_list_find", found == head);
    }
}

void test_single_character_word() {
    std::cout << "\nSingle Element - Single Character Word\n";
    
    // Test: Word with single character
    // Description: Test with one-character words
    // Input: Dictionary with "a", check "a" and "b"
    // Expected: "a" is correct, "b" gets suggestions
    {
        SpellChecker checker;
        checker.addDictionaryWord("a", 100);
        
        assertTrue("test_single_char_correct", checker.isWordCorrect("a"));
        assertFalse("test_single_char_incorrect", checker.isWordCorrect("b"));
    }
}

void test_single_suggestion() {
    std::cout << "\nSingle Element - Single Suggestion\n";
    
    // Test: Only one suggestion available
    // Description: Test when only one suggestion matches
    // Input: Dictionary with "hello", check "helo"
    // Expected: One suggestion returned
    {
        SpellChecker checker;
        checker.addDictionaryWord("hello", 30000);
        
        SuggestionBuffer buffer(3);
        int count = checker.suggest("helo", 2, 3, buffer);
        assertTrue("test_single_suggestion_count", count >= 1);
        
        if (count > 0) {
            Candidate cand = buffer.get(0);
            assertTrue("test_single_suggestion_word", !cand.word.empty());
        }
    }
}

void test_single_trie_node() {
    std::cout << "\nSingle Element - Single Trie Node\n";
    
    // Test: Trie with single word path
    // Description: Test trie with one word (no shared prefixes)
    // Input: Insert "xyz" (unique path)
    // Expected: Word stored and found correctly
    {
        Trie trie;
        trie.insert("xyz", 100);
        
        assertTrue("test_single_trie_contains", trie.contains("xyz"));
        assertFalse("test_single_trie_prefix", trie.contains("xy"));
    }
}

void run_single_tests() {
    test_single_word_dictionary();
    test_single_element_linkedlist();
    test_single_character_word();
    test_single_suggestion();
    test_single_trie_node();
}

