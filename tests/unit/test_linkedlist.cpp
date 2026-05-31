/**
 * @file test_linkedlist.cpp
 * @brief Unit tests for LinkedList data structure
 * @author CSE211-PROJ25 Team
 * @date 2025
 */
#include "../fixtures/test_helpers.hpp"
#include "../../include/data_structures/LinkedList.hpp"
#include <string>

struct TestStruct {
    int value;
    std::string name;
    TestStruct(int v, const std::string& n) : value(v), name(n) {}
    bool operator==(const TestStruct& other) const {
        return value == other.value && name == other.name;
    }
};

void test_linkedlist_construction() {
    printSectionHeader("UNIT");
    std::cout << "LinkedList Construction\n";
    
    // Test: Default constructor
    // Description: Test creating an empty linked list
    // Input: No input
    // Expected: Empty list with head = nullptr
    // Precondition: None
    {
        LinkedList<int> list;
        assertTrue("test_constructor_default", list.getHead() == nullptr);
    }
    
    // Test: Destructor
    // Description: Test that destructor properly frees memory
    // Input: List with elements
    // Expected: All nodes deallocated
    {
        LinkedList<int> list;
        list.append(1);
        list.append(2);
        // Destructor called automatically
        assertTrue("test_destructor", true); // If we get here, no crash
    }
}

void test_linkedlist_insertion() {
    std::cout << "\nLinkedList Insertion\n";
    
    // Test: Insert at beginning (first element)
    // Description: Test inserting first element
    // Input: Empty list, append value 42
    // Expected: List contains [42], head points to node with value 42
    {
        LinkedList<int> list;
        list.append(42);
        ListNode<int>* head = list.getHead();
        assertTrue("test_insert_at_beginning", head != nullptr && head->value == 42);
    }
    
    // Test: Insert at end
    // Description: Test appending multiple elements
    // Input: List with [1], append 2, append 3
    // Expected: List contains [1, 2, 3]
    {
        LinkedList<int> list;
        list.append(1);
        list.append(2);
        list.append(3);
        
        ListNode<int>* node = list.getHead();
        assertTrue("test_insert_end_first", node != nullptr && node->value == 1);
        node = node->next;
        assertTrue("test_insert_end_second", node != nullptr && node->value == 2);
        node = node->next;
        assertTrue("test_insert_end_third", node != nullptr && node->value == 3);
        assertTrue("test_insert_end_null", node->next == nullptr);
    }
    
    // Test: Insert duplicate values
    // Description: Test that duplicates are allowed
    // Input: Append same value twice
    // Expected: Both values stored
    {
        LinkedList<int> list;
        list.append(5);
        list.append(5);
        ListNode<int>* first = list.getHead();
        ListNode<int>* second = first->next;
        assertTrue("test_insert_duplicate", first->value == 5 && second->value == 5);
    }
}

void test_linkedlist_search() {
    std::cout << "\nLinkedList Search\n";
    
    // Test: Find existing element
    // Description: Test finding an element that exists
    // Input: List [1, 2, 3], find value 2
    // Expected: Returns node with value 2
    {
        LinkedList<int> list;
        list.append(1);
        list.append(2);
        list.append(3);
        
        ListNode<int>* found = list.find([](const int& v) { return v == 2; });
        assertTrue("test_search_existing", found != nullptr && found->value == 2);
    }
    
    // Test: Find non-existent element
    // Description: Test finding an element that doesn't exist
    // Input: List [1, 2, 3], find value 5
    // Expected: Returns nullptr
    {
        LinkedList<int> list;
        list.append(1);
        list.append(2);
        list.append(3);
        
        ListNode<int>* found = list.find([](const int& v) { return v == 5; });
        assertTrue("test_search_non_existent", found == nullptr);
    }
    
    // Test: Find with custom predicate
    // Description: Test finding with complex predicate
    // Input: List of structs, find by name
    // Expected: Returns matching node
    {
        LinkedList<TestStruct> list;
        list.append(TestStruct(1, "first"));
        list.append(TestStruct(2, "second"));
        
        ListNode<TestStruct>* found = list.find([](const TestStruct& s) {
            return s.name == "second";
        });
        assertTrue("test_search_predicate", found != nullptr && found->value.name == "second");
    }
}

void test_linkedlist_deletion() {
    std::cout << "\nLinkedList Deletion\n";
    
    // Test: Clear empty list
    // Description: Test clearing an already empty list
    // Input: Empty list
    // Expected: No crash, head remains nullptr
    {
        LinkedList<int> list;
        list.clear();
        assertTrue("test_delete_empty", list.getHead() == nullptr);
    }
    
    // Test: Clear list with elements
    // Description: Test clearing a list with multiple elements
    // Input: List [1, 2, 3], call clear()
    // Expected: All nodes deallocated, head is nullptr
    {
        LinkedList<int> list;
        list.append(1);
        list.append(2);
        list.append(3);
        list.clear();
        assertTrue("test_delete_all", list.getHead() == nullptr);
    }
}

void test_linkedlist_edge_cases() {
    std::cout << "\nLinkedList Edge Cases\n";
    
    // Test: Empty structure
    // Description: Test operations on empty list
    // Input: Empty list
    // Expected: Operations handle empty case gracefully
    {
        LinkedList<int> list;
        ListNode<int>* found = list.find([](const int& v) { return v == 1; });
        assertTrue("test_edge_empty_find", found == nullptr);
        assertTrue("test_edge_empty_head", list.getHead() == nullptr);
    }
    
    // Test: Single element
    // Description: Test list with only one element
    // Input: List with [42]
    // Expected: Operations work correctly
    {
        LinkedList<int> list;
        list.append(42);
        ListNode<int>* head = list.getHead();
        assertTrue("test_edge_single_head", head != nullptr && head->value == 42);
        assertTrue("test_edge_single_next", head->next == nullptr);
        
        ListNode<int>* found = list.find([](const int& v) { return v == 42; });
        assertTrue("test_edge_single_find", found == head);
    }
    
    // Test: Large number of elements
    // Description: Test with many elements
    // Input: Append 1000 elements
    // Expected: All elements stored correctly
    {
        LinkedList<int> list;
        const int count = 1000;
        for (int i = 0; i < count; ++i) {
            list.append(i);
        }
        
        int foundCount = 0;
        ListNode<int>* node = list.getHead();
        while (node) {
            foundCount++;
            node = node->next;
        }
        assertEqual("test_edge_large_count", count, foundCount);
    }
}

void run_linkedlist_tests() {
    test_linkedlist_construction();
    test_linkedlist_insertion();
    test_linkedlist_search();
    test_linkedlist_deletion();
    test_linkedlist_edge_cases();
}

