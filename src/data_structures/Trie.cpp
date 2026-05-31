#include "../include/data_structures/Trie.hpp"
#include <string>

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    freeNode(root);
}

void Trie::freeNode(TrieNode* node) {
    if (!node)
        return;
    ListNode<ChildEntry>* child = node->children.getHead();
    while (child) {
        freeNode(child->value.node);
        child = child->next;
    }
    delete node;
}

TrieNode* Trie::getChild(TrieNode* node, char ch) const {
    ListNode<ChildEntry>* child = node->children.getHead();
    while (child) {
        if (child->value.ch == ch) {
            return child->value.node;
        }
        if (child->value.ch > ch) {
            return nullptr;
        }
        child = child->next;
    }
    return nullptr;
}

TrieNode* Trie::addChild(TrieNode* node, char ch) {
    ChildEntry entry;
    entry.ch = ch;
    entry.node = new TrieNode(ch);
    node->children.insertSorted(entry, [](const ChildEntry& a, const ChildEntry& b) {
        return a.ch < b.ch;
    });
    return entry.node;
}

void Trie::insert(const std::string& word, int frequency) {
    TrieNode* cur = root;
    for (size_t i = 0; i < word.size(); ++i) {
        char c = word[i];
        TrieNode* next = getChild(cur, c);
        if (!next) {
            next = addChild(cur, c);
        }
        cur = next;
    }
    cur->isWord = true;
    cur->frequency = frequency;
}

bool Trie::contains(const std::string& word) const {
    const TrieNode* cur = findNode(word);
    return cur && cur->isWord;
}

TrieNode* Trie::findNode(const std::string& prefix) const {
    TrieNode* cur = root;
    for (size_t i = 0; i < prefix.size(); ++i) {
        char c = prefix[i];
        ListNode<ChildEntry>* child = cur->children.getHead();
        bool found = false;
        while (child) {
            if (child->value.ch == c) {
                cur = child->value.node;
                found = true;
                break;
            }
            if (child->value.ch > c) {
                return nullptr;
            }
            child = child->next;
        }
        if (!found)
            return nullptr;
    }
    return cur;
}
