# CSE211-PROJ25: Spell Checker & Corrector

## Project Description

A C++ spell checker application that uses a **Trie** (prefix tree) for efficient dictionary storage, **Levenshtein distance** for correction suggestions, and prefix traversal for **autocomplete**. The project implements custom pointer-based data structures without STL containers as primary storage.

## Group Information

- **Group ID**: PROJ-25
- **Course**: CSE 211 - Data Structures
- **Semester**: 2025-2026 Spring

## Build Instructions

### Prerequisites
- C++17 compatible compiler (g++ or clang++)
- Make utility
- CMake and Qt6 Widgets (for the required GUI)
- Doxygen (for documentation generation)
- clang-format (for formatting)
- PlantUML (for regenerating UML image exports)
- Valgrind (for memory checking, optional)

### Building the Project

1. **Install dependencies** (if any):
   ```bash
   make deps
   ```

2. **Build the project**:
   ```bash
   make
   ```
   This creates the command-line fallback and builds the required Qt6 GUI.

3. **Run the GUI**:
   ```bash
   make run
   ```

4. **Run the command-line fallback**:
   ```bash
   make run-cli
   ```
   Or specify a custom input file with `./spellcheck data/input_sample.json`.

5. **Run tests**:
   ```bash
   make test
   ```

6. **Build GUI only** (required visual frontend, requires Qt6):
   ```bash
   make gui
   ```
   Then run:
   ```bash
   ./frontend/gui/build/spell_gui
   ```
   
   **Installing Qt6:**
   - **Ubuntu/Debian**: `sudo apt-get install qt6-base-dev cmake`
   - **Windows**: Download Qt6 from https://www.qt.io/download
   - **macOS**: `brew install qt6 cmake`
   
   **Note**: The command-line version is kept for testing and debugging, but the Spring 2026 project requirement expects a working visual GUI.

7. **Generate Doxygen documentation**:
   ```bash
   make docs
   ```
   Documentation will be generated in `docs/html/`

8. **Check for memory leaks**:
   ```bash
   make memcheck
   ```

9. **Format code**:
   ```bash
   make format
   ```

10. **Clean build artifacts**:
   ```bash
   make clean
   ```

## Usage Examples

### GUI Mode
The required visual frontend checks the last typed word and shows correction or autocomplete buttons:

```bash
make run
```

### Command-Line Batch Mode
The command-line fallback processes words from the input file and displays suggestions:

```bash
./spellcheck data/input_sample.json
```

Example output:
```
=== Batch Check ===
hello is correct.
helo -> hello
wrold -> world
thier -> their, there, they
teh -> (no suggestions)

=== Interactive ===
Spell Checker Interactive Mode (type 'exit' to quit)
Enter word: helo
Suggestions:
  hello (dist=1, freq=30000)
Enter word: exit
```

### Interactive CLI Mode
After batch processing, the fallback executable enters interactive mode where you can type words to check. This mode is kept for testing and debugging; the Qt GUI is the submission frontend.

## Project Structure

```
CSE211-PROJ25/
├── include/                    # Header files
│   ├── data_structures/       # Custom data structures
│   │   ├── LinkedList.hpp     # Singly linked list
│   │   └── Trie.hpp            # Trie (prefix tree)
│   ├── core/                  # Core algorithms and logic
│   │   ├── SpellChecker.hpp   # Main spell checker class
│   │   ├── EditDistance.hpp   # Levenshtein distance algorithm
│   │   ├── SuggestionBuffer.hpp # Suggestion ranking buffer
│   │   └── Types.hpp           # Common types and structs
│   └── utils/                 # Utility functions
│       └── Parser.hpp          # Input file parser
├── src/                       # Source files
│   ├── data_structures/       # Data structure implementations
│   │   └── Trie.cpp
│   ├── core/                  # Core algorithm implementations
│   │   ├── SpellChecker.cpp
│   │   ├── EditDistance.cpp
│   │   └── SuggestionBuffer.cpp
│   ├── utils/                 # Utility implementations
│   │   └── Parser.cpp
│   └── main.cpp               # Entry point
├── frontend/                  # User interface
│   ├── Tui.hpp                # Text UI header
│   ├── tui.cpp                # Text UI implementation
│   └── gui/                   # Qt6 GUI
│       ├── SpellCheckerWidget.hpp
│       ├── main.cpp
│       ├── SpellCheckerWidget.cpp
│       └── spellchecker.cpp
├── tests/                     # Test files
│   ├── unit/                  # Unit tests
│   ├── integration/           # Integration tests
│   ├── edge_cases/            # Edge case tests
│   ├── performance/           # Performance tests
│   ├── fixtures/              # Test helpers
│   └── test_main.cpp          # Test runner
├── data/                      # Input data files
│   ├── input_default.json
│   ├── input_sample.json
│   ├── input_large.json       # Larger dictionary sample
│   └── edge_case_*.json       # Edge case test files
├── docs/                      # Documentation
│   ├── design_document.md     # Design documentation
│   ├── uml/                   # UML diagrams
│   │   ├── class_diagram.puml
│   │   ├── class_diagram.png
│   │   └── sequence_diagram.puml
│   ├── presentation.pptx      # Project presentation
│   └── Doxyfile               # Doxygen configuration
├── Makefile                   # Build configuration
├── .gitignore                 # Git ignore rules
├── .clang-format              # Code formatting rules
└── README.md                  # This file
```

## Data Structures Used and Why

### 1. **Trie (Prefix Tree)**
- **Purpose**: Efficient dictionary storage and prefix-based operations
- **Why**: 
  - O(k) lookup time where k is word length
  - Enables efficient fuzzy search with edit distance
  - Supports prefix matching for autocomplete-like features
- **Implementation**: Pointer-based nodes with LinkedList children (no STL containers)

### 2. **LinkedList<T>**
- **Purpose**: Stores children of each Trie node
- **Why**:
  - Custom implementation required (no STL)
  - Dynamic size without pre-allocation
  - Pointer-based for memory efficiency
- **Implementation**: Singly linked list with pointer-based nodes

### 3. **SuggestionBuffer**
- **Purpose**: Maintains sorted list of candidate suggestions
- **Why**:
  - Fixed-size buffer for top-k suggestions
  - Efficient insertion maintaining sorted order
  - Primary sort: edit distance, secondary: frequency

### Feature: Autocomplete
- **Purpose**: Complete user prefixes with dictionary words
- **Implementation**: Finds the trie node for the prefix, traverses its subtree, and ranks completions by frequency through `SuggestionBuffer`
- **Complexity**: O(p + s) traversal where p is prefix length and s is the visited prefix subtree size

### Optimization: Repeated-Query Cache
- **Purpose**: Avoid recomputing suggestions and autocomplete results for repeated queries
- **Implementation**: Small bounded pointer-based cache inside `SpellChecker`
- **Scope**: Cache is cleared when the dictionary changes

### Algorithm: Levenshtein Distance
- **Purpose**: Calculate minimum edit distance between words
- **Optimization**: Early termination when distance exceeds threshold
- **Complexity**: O(m*n) worst case, optimized with row-min pruning

## Known Limitations

1. **No Transposition Operation**: The edit distance algorithm does not support character transposition (e.g., "teh" → "the" requires 2 operations instead of 1).

2. **Focused Parser**: The parser supports the documented project schema, including multiline and inline dictionary/check_words arrays, but it is not a full general-purpose JSON parser.

3. **No Concurrency**: The implementation is single-threaded and suitable for moderate dictionary sizes (k ≤ 2 edit distance).

4. **Fixed Buffer Size**: SuggestionBuffer has a fixed capacity; suggestions beyond this limit are discarded.

5. **Memory Management**: Manual memory management is used; smart pointers could improve safety but raw pointers are used for educational purposes.

## Testing

The project includes:
- **Unit tests** for data structures (Trie, LinkedList)
- **Algorithm tests** for edit distance computation
- **Integration tests** for end-to-end workflow

Run tests with `make test`.

## Documentation

Code documentation follows Doxygen standards with `@brief`, `@param`, `@return`, `@pre`, and `@post` comments on public project interfaces. Generate HTML documentation with `make docs`.

## License

This project is created for educational purposes as part of CSE211 course requirements.
