#pragma once

#include <string>
#include "LinkedList.hpp"
#include "../core/Types.hpp"

/**
 * @brief Child pointer stored in a trie node's linked list.
 *
 * @pre node must either be nullptr during construction or point to a valid TrieNode.
 * @post ch identifies the edge label that reaches node.
 */
struct ChildEntry {
    char ch;
    struct TrieNode* node;
};

/**
 * @brief Node in a pointer-based trie.
 *
 * @pre Children are stored only in the custom LinkedList.
 * @post Terminal nodes mark complete dictionary words and keep their frequency.
 */
struct TrieNode {
    char ch;
    bool isWord;
    int frequency;
    LinkedList<ChildEntry> children;

    TrieNode(char c = '\0') : ch(c), isWord(false), frequency(0), children() {}
};

/**
 * @brief Pointer-based trie for dictionary storage and prefix traversal.
 *
 * @pre Words should be normalized by the caller before insertion/search.
 * @post Inserted words are reachable from the root through character edges.
 */
class Trie {
   public:
    /**
     * @brief Constructs an empty trie with a root node.
     * @post getRoot() returns a valid non-null root pointer.
     */
    Trie();

    /**
     * @brief Destroys the trie and releases all nodes.
     * @post All dynamically allocated trie nodes are deleted.
     */
    ~Trie();

    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    /**
     * @brief Inserts or updates a dictionary word.
     *
     * @param word Word to insert.
     * @param frequency Frequency used for ranking.
     * @pre word may be empty only if the empty string should be accepted.
     * @post contains(word) returns true and the terminal frequency is updated.
     */
    void insert(const std::string& word, int frequency);

    /**
     * @brief Checks whether a word exists in the trie.
     *
     * @param word Word to search for.
     * @return true if word is a terminal dictionary entry.
     * @pre The trie is initialized.
     * @post The trie is not modified.
     */
    bool contains(const std::string& word) const;

    /**
     * @brief Finds the node reached by a prefix.
     *
     * @param prefix Prefix path to follow from the root.
     * @return TrieNode* for the prefix, or nullptr when absent.
     * @pre The trie is initialized.
     * @post The trie is not modified.
     */
    TrieNode* findNode(const std::string& prefix) const;

    /**
     * @brief Returns the trie root for controlled traversal.
     *
     * @return Root node pointer.
     * @pre The trie is initialized.
     * @post Ownership remains with the Trie object.
     */
    TrieNode* getRoot() const {
        return root;
    }

   private:
    TrieNode* root;

    void freeNode(TrieNode* node);

    TrieNode* getChild(TrieNode* node, char ch) const;

    TrieNode* addChild(TrieNode* node, char ch);
};
