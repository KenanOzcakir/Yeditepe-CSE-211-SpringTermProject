#pragma once

#include <string>
#include "../data_structures/Trie.hpp"
#include "Types.hpp"
#include "SuggestionBuffer.hpp"

/**
 * @brief Spell checker built on a custom trie and edit-distance search.
 *
 * @pre Dictionary words should be inserted before checking or suggesting.
 * @post The object can answer exact, correction, and autocomplete queries.
 */
class SpellChecker {
   public:
    /**
     * @brief Constructs an empty spell checker.
     * @post The internal dictionary trie is empty.
     */
    SpellChecker();

    /**
     * @brief Releases cached query results owned by the spell checker.
     * @post All internal cache entries are deleted.
     */
    ~SpellChecker();

    SpellChecker(const SpellChecker&) = delete;
    SpellChecker& operator=(const SpellChecker&) = delete;

    /**
     * @brief Adds a dictionary word with its frequency.
     *
     * @param word Dictionary word.
     * @param frequency Frequency used to rank equal-distance suggestions.
     * @pre frequency should be non-negative.
     * @post isWordCorrect(word) returns true.
     */
    void addDictionaryWord(const std::string& word, int frequency);

    /**
     * @brief Checks whether a word is exactly in the dictionary.
     *
     * @param word Word to check.
     * @return true if the word exists in the trie.
     * @post The dictionary is not modified.
     */
    bool isWordCorrect(const std::string& word) const;

    /**
     * @brief Generates ranked correction suggestions.
     *
     * @param word Misspelled or query word.
     * @param maxEditDistance Maximum Levenshtein distance to accept.
     * @param maxSuggestions Maximum requested suggestions; the output buffer capacity is authoritative.
     * @param outBuffer Buffer that receives ranked candidates.
     * @return Number of candidates stored in outBuffer.
     * @pre outBuffer should have the desired capacity.
     * @post Suggestions are ranked by edit distance, then frequency.
     */
    int suggest(const std::string& word, int maxEditDistance, int maxSuggestions,
                SuggestionBuffer& outBuffer) const;

    /**
     * @brief Generates autocomplete candidates for a prefix.
     *
     * @param prefix Prefix typed by the user.
     * @param maxSuggestions Maximum requested completions; the output buffer capacity is authoritative.
     * @param outBuffer Buffer that receives ranked completions.
     * @return Number of completions stored in outBuffer.
     * @pre outBuffer should have the desired capacity.
     * @post Completions are ranked by frequency.
     */
    int autocomplete(const std::string& prefix, int maxSuggestions, SuggestionBuffer& outBuffer) const;

   private:
    Trie trie;

    struct CacheEntry {
        std::string query;
        int maxEditDistance;
        int maxSuggestions;
        bool autocompleteMode;
        Candidate* candidates;
        int count;
        CacheEntry* next;
    };

    static const int CACHE_LIMIT = 32;

    mutable CacheEntry* cacheHead;
    mutable int cacheSize;

    /**
     * @brief Recursively traverses the trie while maintaining a Levenshtein row.
     */
    void searchRecursive(TrieNode* node, const std::string& target, int maxEditDistance,
                         int* previousRow, SuggestionBuffer& buffer, std::string current) const;

    /**
     * @brief Collects ranked completions below a prefix node.
     */
    void collectCompletions(TrieNode* node, const std::string& current,
                            SuggestionBuffer& buffer) const;

    /**
     * @brief Finds a cached correction or autocomplete result.
     */
    CacheEntry* findCached(const std::string& query, int maxEditDistance, int maxSuggestions,
                           bool autocompleteMode) const;

    /**
     * @brief Stores a bounded copy of query results for repeated lookups.
     */
    void storeCache(const std::string& query, int maxEditDistance, int maxSuggestions,
                    bool autocompleteMode, const SuggestionBuffer& buffer) const;

    /**
     * @brief Replays cached candidates into the caller's output buffer.
     */
    void replayCached(const CacheEntry* entry, SuggestionBuffer& outBuffer) const;

    /**
     * @brief Keeps the repeated-query cache within CACHE_LIMIT entries.
     */
    void trimCache() const;

    /**
     * @brief Clears every cached query result.
     */
    void clearCache() const;
};
