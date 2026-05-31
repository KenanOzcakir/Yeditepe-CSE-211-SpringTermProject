#include "../include/core/SpellChecker.hpp"
#include "../include/core/SuggestionBuffer.hpp"
#include "../include/core/EditDistance.hpp"

SpellChecker::SpellChecker() : trie(), cacheHead(nullptr), cacheSize(0) {}

SpellChecker::~SpellChecker() {
    clearCache();
}

void SpellChecker::addDictionaryWord(const std::string& word, int frequency) {
    clearCache();
    trie.insert(word, frequency);
}

bool SpellChecker::isWordCorrect(const std::string& word) const {
    return trie.contains(word);
}

int SpellChecker::suggest(const std::string& word, int maxEditDistance, int maxSuggestions,
                          SuggestionBuffer& outBuffer) const {
    if (maxSuggestions <= 0) {
        return 0;
    }

    CacheEntry* cached = findCached(word, maxEditDistance, maxSuggestions, false);
    if (cached) {
        replayCached(cached, outBuffer);
        return outBuffer.size();
    }

    SuggestionBuffer queryBuffer(maxSuggestions);
    int* initialRow = new int[word.size() + 1];
    for (size_t i = 0; i < word.size() + 1; ++i) {
        initialRow[i] = static_cast<int>(i);
    }
    searchRecursive(trie.getRoot(), word, maxEditDistance, initialRow, queryBuffer, "");
    delete[] initialRow;

    storeCache(word, maxEditDistance, maxSuggestions, false, queryBuffer);
    for (int i = 0; i < queryBuffer.size(); ++i) {
        outBuffer.tryAdd(queryBuffer.get(i));
    }
    return outBuffer.size();
}

int SpellChecker::autocomplete(const std::string& prefix, int maxSuggestions,
                               SuggestionBuffer& outBuffer) const {
    if (maxSuggestions <= 0) {
        return 0;
    }

    CacheEntry* cached = findCached(prefix, 0, maxSuggestions, true);
    if (cached) {
        replayCached(cached, outBuffer);
        return outBuffer.size();
    }

    TrieNode* start = trie.findNode(prefix);
    if (!start) {
        return 0;
    }

    SuggestionBuffer queryBuffer(maxSuggestions);
    collectCompletions(start, prefix, queryBuffer);
    storeCache(prefix, 0, maxSuggestions, true, queryBuffer);
    for (int i = 0; i < queryBuffer.size(); ++i) {
        outBuffer.tryAdd(queryBuffer.get(i));
    }
    return outBuffer.size();
}

void SpellChecker::searchRecursive(TrieNode* node, const std::string& target, int maxEditDistance,
                                   int* previousRow, SuggestionBuffer& buffer,
                                   std::string current) const {
    const int columns = static_cast<int>(target.size()) + 1;

    ListNode<ChildEntry>* childNode = node->children.getHead();
    while (childNode) {
        ChildEntry ce = childNode->value;
        TrieNode* nextNode = ce.node;
        std::string nextPrefix = current + ce.ch;

        int* currentRow = new int[columns];
        currentRow[0] = previousRow[0] + 1;
        int rowMin = currentRow[0];

        for (int column = 1; column < columns; ++column) {
            int insertCost = currentRow[column - 1] + 1;
            int deleteCost = previousRow[column] + 1;
            int replaceCost = previousRow[column - 1] + (target[column - 1] == ce.ch ? 0 : 1);
            int smallest = insertCost;
            if (deleteCost < smallest)
                smallest = deleteCost;
            if (replaceCost < smallest)
                smallest = replaceCost;
            currentRow[column] = smallest;
            if (smallest < rowMin)
                rowMin = smallest;
        }

        if (nextNode->isWord && currentRow[columns - 1] <= maxEditDistance) {
            Candidate cand;
            cand.word = nextPrefix;
            cand.distance = currentRow[columns - 1];
            cand.frequency = nextNode->frequency;
            buffer.tryAdd(cand);
        }

        if (rowMin <= maxEditDistance) {
            searchRecursive(nextNode, target, maxEditDistance, currentRow, buffer, nextPrefix);
        }

        delete[] currentRow;
        childNode = childNode->next;
    }
}

void SpellChecker::collectCompletions(TrieNode* node, const std::string& current,
                                      SuggestionBuffer& buffer) const {
    if (!node) {
        return;
    }

    if (node->isWord) {
        Candidate cand;
        cand.word = current;
        cand.distance = 0;
        cand.frequency = node->frequency;
        buffer.tryAdd(cand);
    }

    ListNode<ChildEntry>* childNode = node->children.getHead();
    while (childNode) {
        ChildEntry ce = childNode->value;
        collectCompletions(ce.node, current + ce.ch, buffer);
        childNode = childNode->next;
    }
}

SpellChecker::CacheEntry* SpellChecker::findCached(const std::string& query, int maxEditDistance,
                                                    int maxSuggestions,
                                                    bool autocompleteMode) const {
    CacheEntry* cur = cacheHead;
    while (cur) {
        if (cur->query == query && cur->maxEditDistance == maxEditDistance &&
            cur->maxSuggestions == maxSuggestions && cur->autocompleteMode == autocompleteMode) {
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
}

void SpellChecker::storeCache(const std::string& query, int maxEditDistance, int maxSuggestions,
                              bool autocompleteMode, const SuggestionBuffer& buffer) const {
    CacheEntry* entry = new CacheEntry;
    entry->query = query;
    entry->maxEditDistance = maxEditDistance;
    entry->maxSuggestions = maxSuggestions;
    entry->autocompleteMode = autocompleteMode;
    entry->count = buffer.size();
    entry->candidates = entry->count > 0 ? new Candidate[entry->count] : nullptr;
    for (int i = 0; i < entry->count; ++i) {
        entry->candidates[i] = buffer.get(i);
    }
    entry->next = cacheHead;
    cacheHead = entry;
    ++cacheSize;
    trimCache();
}

void SpellChecker::replayCached(const CacheEntry* entry, SuggestionBuffer& outBuffer) const {
    if (!entry) {
        return;
    }
    for (int i = 0; i < entry->count; ++i) {
        outBuffer.tryAdd(entry->candidates[i]);
    }
}

void SpellChecker::trimCache() const {
    while (cacheSize > CACHE_LIMIT && cacheHead) {
        CacheEntry* cur = cacheHead;
        CacheEntry* prev = nullptr;
        while (cur->next) {
            prev = cur;
            cur = cur->next;
        }

        if (prev) {
            prev->next = nullptr;
        } else {
            cacheHead = nullptr;
        }

        delete[] cur->candidates;
        delete cur;
        --cacheSize;
    }
}

void SpellChecker::clearCache() const {
    CacheEntry* cur = cacheHead;
    while (cur) {
        CacheEntry* next = cur->next;
        delete[] cur->candidates;
        delete cur;
        cur = next;
    }
    cacheHead = nullptr;
    cacheSize = 0;
}
