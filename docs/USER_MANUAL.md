# User Manual: Spell Checker & Corrector

**Version:** 1.0  
**Group:** PROJ-25  
**Course:** CSE 211 - Data Structures

---

## Introduction

The **Spell Checker & Corrector** is a C++ application that helps you identify misspelled words and get suggestions for corrections.

**What it does:**
- Checks if words are spelled correctly
- Suggests corrections for misspelled words
- Completes prefixes using autocomplete
- Shows suggestions ranked by similarity and word frequency
- Works with command-line or graphical interface
- Supports larger dictionary data files

---

## System Requirements

**What you need:**
- Linux, Windows (with WSL), or macOS
- C++ compiler (g++ or clang++)
- Make utility

**For GUI:**
- Qt6 library
- CMake

**For documentation and formatting:**
- Doxygen
- clang-format
- PlantUML, if UML images need to be regenerated

---

## Installation

### Step 1: Extract and Build

```bash
unzip PROJ25_25.zip
cd PROJ25_25
make
```

This builds the command-line fallback and the required Qt6 GUI when dependencies are available.

### Step 2: Build GUI

**Install Qt6:**
- **Ubuntu/Debian:** `sudo apt-get install qt6-base-dev cmake`
- **Windows:** Download from https://www.qt.io/download
- **macOS:** `brew install qt6 cmake`

**Build:**
```bash
make gui
```

### Step 3: Check if it works

```bash
make test
```

You should see: `SUMMARY: 147/147 tests passed | 0 failed | 0 skipped`.

---

## How to Use

### Graphical Interface (GUI)

**Start the GUI:**
```bash
make run
```

**Or start the already-built GUI directly:**
```bash
./frontend/gui/build/spell_gui
```

**How to use:**
1. Type text in the input field
2. The last word you type is checked automatically
3. Status shows "correct" for valid words
4. For misspelled words, correction suggestions appear as buttons
5. For valid prefixes, autocomplete completions appear as buttons
6. Click a suggestion or completion button to replace the last word

### Command-Line Fallback

**Run the fallback program:**
```bash
make run-cli
```

**Or use a specific file:**
```bash
./spellcheck data/input_sample.json
```

**What you'll see:**
```
=== Batch Check ===
hello is correct.
helo -> hello
wrold -> world
thier -> their, there, they

=== Interactive ===
Enter word: computr
Suggestions:
  computer (dist=1, freq=25000)
Enter word: exit
```

**How it works:**
1. The program loads words from the input file
2. It checks each word in the "check_words" list
3. For correct words, it shows "is correct."
4. For misspelled words, it shows suggestions
5. After batch processing, you can type words manually

The command-line mode is kept for testing and debugging. The Qt GUI is the required visual frontend for the term project.

---

## Input File Format

The program reads JSON files. Here's the format:

```json
{
  "dictionary": [
    {"word": "hello", "frequency": 30000},
    {"word": "world", "frequency": 40000}
  ],
  "check_words": ["hello", "helo", "wrold"],
  "max_edit_distance": 2,
  "max_suggestions": 3
}
```

**What each part means:**
- **dictionary:** List of words the program knows
- **check_words:** Words you want to check
- **max_edit_distance:** How different words can be (1-3 is typical)
- **max_suggestions:** Maximum number of suggestions to show

**Ready-to-use files:**
- `data/input_sample.json` - Small example (6 words)
- `data/input_large.json` - Larger dictionary sample

---

## Examples

### Example 1: Check words from a file

```bash
./spellcheck data/input_sample.json
```

### Example 2: Use your own dictionary

1. Create a JSON file with your words (see format above)
2. Run:
```bash
./spellcheck my_dictionary.json
```

### Example 3: Check words one by one

After the program processes the file, you can type words:
```
Enter word: algoritm
Suggestions:
  algorithm (dist=1, freq=20000)
Enter word: exit
```

---

## Common Problems and Solutions

**Problem:** "Command not found: spellcheck"
- **Fix:** Run `make` first to build the program

**Problem:** "Failed to load input file"
- **Fix:** Make sure the file path is correct and the file exists

**Problem:** "No suggestions found"
- **Fix:** Try increasing `max_edit_distance` in your input file, or use a larger dictionary

**Problem:** GUI doesn't open
- **Fix:** Install Qt6 and run `make gui` again

**Problem:** "Loaded 0 words"
- **Fix:** Check that your JSON file follows the correct format

---

## Useful Commands

```bash
make              # Build command-line fallback and required GUI
make run          # Build and run the GUI
make run-cli      # Run command-line fallback
make test         # Run tests
make gui          # Build required Qt6 GUI
make check-deps   # Check installed tools
make deps         # Install tools using brew or apt-get when available
make clean        # Remove build files
make docs         # Generate documentation
```

---

**For more technical information, see `docs/design_document.md`**  
**For project overview, see `README.md`**
