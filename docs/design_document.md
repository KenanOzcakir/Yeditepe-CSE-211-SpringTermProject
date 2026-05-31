# CSE211-PROJ25: Spell Checker & Corrector - Design Document

## 1. Architecture Overview

The spell checker application follows a modular architecture with clear separation of concerns:

- **Data Layer**: Custom data structures (Trie, LinkedList) for dictionary storage
- **Algorithm Layer**: Edit distance computation and fuzzy search algorithms
- **Application Layer**: Spell checker logic that integrates data structures and algorithms
- **Interface Layer**: Qt6 graphical interface for visual interaction, with a command-line mode retained for testing
- **I/O Layer**: Parser for reading input files

The architecture emphasizes:
- Pointer-based implementations (no array-based storage)
- Custom data structures (no STL containers)
- Modular design with single responsibility principle
- Efficient algorithms with early termination optimizations
- Doxygen-documented public interfaces for Spring 2026 documentation requirements

## 2. Module Descriptions

### 2.1 Trie Module (`include/Trie.hpp`, `src/Trie.cpp`)

**Purpose**: Prefix tree for dictionary storage

**Key Components**:
- `TrieNode`: Represents a node with character, word flag, frequency, and children
- `Trie`: Main trie class with insert and search operations

**Responsibilities**:
- Store dictionary words efficiently
- Support O(k) word lookup
- Enable prefix-based traversal for fuzzy search and autocomplete

**Complexity**:
- Insert: O(k) where k is word length
- Search: O(k) where k is word length
- Space: O(ALPHABET_SIZE * N * k) worst case

### 2.2 LinkedList Module (`include/LinkedList.hpp`)

**Purpose**: Singly linked list for storing trie children

**Key Components**:
- `ListNode<T>`: Node structure with value and next pointer
- `LinkedList<T>`: List class with append, find, and clear operations

**Responsibilities**:
- Store child entries for each trie node
- Provide iteration capabilities
- Manage memory for dynamic child lists

**Complexity**:
- Append: O(n) where n is current list size
- Find: O(n) worst case
- Clear: O(n)

### 2.3 EditDistance Module (`include/EditDistance.hpp`, `src/EditDistance.cpp`)

**Purpose**: Compute Levenshtein distance between strings

**Key Algorithm**: Dynamic programming with early termination

**Optimizations**:
- Bounded distance computation (returns maxDist+1 if exceeded)
- Row-min pruning for early exit
- Two-row optimization for space efficiency

**Complexity**:
- Time: O(m*n) worst case, optimized with early exit
- Space: O(min(m,n)) using two-row DP

### 2.4 SpellChecker Module (`include/SpellChecker.hpp`, `src/SpellChecker.cpp`)

**Purpose**: Main spell checking logic integrating trie and edit distance

**Key Components**:
- `SuggestionBuffer`: Maintains sorted list of suggestions
- `SpellChecker`: Core class with dictionary management and suggestion generation

**Responsibilities**:
- Load dictionary into trie
- Check word correctness
- Generate ranked suggestions using fuzzy search
- Generate ranked autocomplete completions from trie prefixes
- Cache repeated correction and autocomplete queries in a bounded pointer-based cache

**Fuzzy Search Algorithm**:
- Recursive trie traversal
- DP row tracking per depth level
- Pruning branches where rowMin > maxEditDistance
- Ranking by distance (primary) and frequency (secondary)

**Autocomplete Algorithm**:
- Locate the trie node that represents the typed prefix
- Traverse the subtree below that node
- Add every terminal word to `SuggestionBuffer`
- Rank completions by frequency because all autocomplete candidates have distance 0

**Repeated-Query Cache**:
- Cache key includes query text, edit-distance threshold, suggestion limit, and query type
- Cached entries store a bounded copy of ranked `Candidate` results
- Cache is cleared when a dictionary word is inserted so stale suggestions cannot remain

**Complexity**:
- Fuzzy search: O(alpha * k * |dict-prefixes|) where alpha is alphabet size, k is max distance

### 2.5 Parser Module (`include/Parser.hpp`, `src/Parser.cpp`)

**Purpose**: Parse input files in JSON-like format

**Responsibilities**:
- Read dictionary entries
- Parse words to check
- Extract configuration (max edit distance, max suggestions)
- Memory management for parsed data

**Format support**:
- Multiline dictionary and check_words arrays
- Inline PDF-style arrays such as `"check_words": ["hello", "helo"]`
- Configuration fields for `max_edit_distance` and `max_suggestions`

**Limitations**: Focused parser for the documented project schema, not a full general-purpose JSON parser

### 2.6 Frontend Modules (`frontend/gui`, `frontend/Tui.hpp`, `frontend/tui.cpp`)

**Purpose**: Provide the required visual frontend and a simple command-line fallback

**Modes**:
- GUI mode: Qt6 widget checks the last typed word, displays correction buttons, and shows autocomplete completions
- Batch mode: Process words from input file for testing and demonstration
- Interactive mode: Developer/debug command-line checking

## 3. Data Structure Choices and Rationale

### 3.1 Trie Selection

**Why Trie?**
- **Efficiency**: O(k) lookup vs O(n) for array/list
- **Prefix Operations**: Natural support for prefix matching
- **Fuzzy Search**: Enables efficient traversal with edit distance tracking
- **Scalability**: Handles large dictionaries efficiently

**Alternative Considered**: Hash table
- **Rejected**: Does not support prefix-based fuzzy search efficiently
- **Rejected**: Would require checking all dictionary words for suggestions

### 3.2 LinkedList for Children

**Why LinkedList?**
- **Dynamic Size**: No need to pre-allocate fixed-size arrays
- **Memory Efficiency**: Only allocates nodes for existing children
- **Custom Requirement**: Must implement custom data structure (no STL)

**Alternative Considered**: Array-based storage
- **Rejected**: Violates pointer-based requirement
- **Rejected**: Less memory efficient for sparse children

### 3.3 SuggestionBuffer Design

**Why Fixed-Size Buffer?**
- **Memory Bounded**: Prevents unbounded memory growth
- **Top-K Selection**: Only keeps best suggestions
- **Efficiency**: O(k) insertion where k is buffer size

**Sorting Strategy**:
- Primary: Edit distance (lower is better)
- Secondary: Frequency (higher is better)
- Maintains sorted order during insertion

## 4. Algorithm Explanations with Complexity

### 4.1 Levenshtein Distance Algorithm

**Algorithm**: Dynamic Programming

```
dp[i][j] = minimum edits to transform a[0..i] to b[0..j]

Base cases:
- dp[0][j] = j (insert j characters)
- dp[i][0] = i (delete i characters)

Recurrence:
dp[i][j] = min(
    dp[i-1][j] + 1,           // deletion
    dp[i][j-1] + 1,           // insertion
    dp[i-1][j-1] + cost        // substitution (cost = 0 if match, 1 otherwise)
)
```

**Optimization**: Early termination when row minimum exceeds maxDist

**Complexity**:
- Time: O(m*n) worst case, O(m*maxDist) with early exit
- Space: O(min(m,n)) with two-row optimization

### 4.2 Fuzzy Search in Trie

**Algorithm**: Recursive DFS with DP row tracking

```
searchRecursive(node, target, maxDist, previousRow, buffer, current):
    For each child of node:
        Compute currentRow using DP from previousRow
        If currentRow[rowMin] <= maxDist:
            If node.isWord and currentRow[target.length] <= maxDist:
                Add to buffer
            Recursively search child
```

**Pruning**: Skip branches where rowMin > maxDist

**Complexity**:
- Time: O(alpha * k * |dict-prefixes|) where:
  - alpha = alphabet size
  - k = max edit distance
  - |dict-prefixes| = number of prefixes in dictionary
- Space: O(k * target_length) for DP rows per recursion level

### 4.3 Suggestion Ranking

**Algorithm**: Two-level sorting

1. **Primary Sort**: Edit distance (ascending)
2. **Secondary Sort**: Frequency (descending)

**Implementation**: Insertion sort maintaining sorted order

**Complexity**: O(k) per insertion where k is buffer size

### 4.4 Autocomplete

**Algorithm**: Prefix lookup plus recursive trie traversal

```
autocomplete(prefix, buffer):
    start = trie.findNode(prefix)
    if start is null:
        return 0
    collectCompletions(start, prefix, buffer)

collectCompletions(node, current, buffer):
    if node.isWord:
        add current to buffer with distance 0 and node.frequency
    for each child:
        collectCompletions(child, current + child.character, buffer)
```

**Complexity**:
- Prefix lookup: O(p) where p is prefix length
- Completion traversal: O(s) where s is the number of nodes in the matching subtree
- Candidate insertion: O(k) per terminal word, where k is maximum suggestions

## 5. Design Decisions and Trade-offs

### 5.1 Pointer-Based vs Array-Based

**Decision**: Pointer-based implementation

**Rationale**:
- Course requirement (no array-based storage)
- More flexible memory management
- Better for dynamic structures

**Trade-off**:
- More complex memory management
- Potential for memory leaks (mitigated with careful design)

### 5.2 Custom Data Structures vs STL

**Decision**: Custom implementations

**Rationale**:
- Course requirement (demonstrate data structure knowledge)
- Full control over implementation
- Educational value

**Trade-off**:
- More code to maintain
- Less tested than STL
- Potential for bugs

### 5.3 Early Termination in Edit Distance

**Decision**: Implement row-min pruning

**Rationale**:
- Significant performance improvement for large strings
- Reduces unnecessary computation
- Maintains correctness

**Trade-off**:
- Slightly more complex code
- Small overhead for row-min calculation

### 5.4 Fixed Buffer Size

**Decision**: Limit suggestion buffer capacity

**Rationale**:
- Memory bounded
- Most users only need top suggestions
- Simpler implementation

**Trade-off**:
- May discard valid suggestions beyond capacity
- Could use heap for better top-k selection (more complex)

### 5.5 No Transposition Operation

**Decision**: Exclude transposition from edit operations

**Rationale**:
- Simpler implementation
- Standard Levenshtein distance definition
- Sufficient for most use cases

**Trade-off**:
- Some common typos require 2 operations instead of 1
- Example: "teh" → "the" needs 2 substitutions instead of 1 transposition

## 6. Memory Management

### 6.1 Allocation Strategy

- **Trie Nodes**: Allocated with `new`, freed recursively in destructor
- **LinkedList Nodes**: Allocated with `new`, freed in `clear()`
- **DP Rows**: Allocated per recursion level, freed after use
- **SuggestionBuffer**: Custom linked list bounded by fixed capacity
- **Repeated-Query Cache**: Bounded to 32 entries and released by `SpellChecker`

### 6.2 Memory Leak Prevention

- Destructors properly free all allocated memory
- Rule of Five: Copy operations deleted (not needed for this use case)
- RAII principles followed where possible
- Cache entries are invalidated whenever the dictionary changes

## 7. Testing Strategy

### 7.1 Unit Tests

- Trie insertion and search
- LinkedList operations
- Edit distance computation
- Edge cases (empty strings, single characters)

### 7.2 Integration Tests

- End-to-end spell checking workflow
- File parsing and processing, including inline PDF-style input arrays
- Batch and interactive modes
- Autocomplete and repeated-query cache behavior

### 7.3 Edge Cases

- Empty dictionary
- Single word dictionary
- Very long words
- Maximum edit distance scenarios
- Invalid input handling

## 8. Future Improvements

1. **Transposition Support**: Add character transposition to edit operations
2. **Better Parser**: Full JSON parser with detailed error reporting
3. **Concurrency**: Parallel processing for large dictionaries
4. **Performance Optimization**: More advanced batching for similar queries
5. **Language Support**: Unicode and multi-language dictionaries

