CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Iinclude -Ifrontend
LDFLAGS ?= -pthread

APP := spellcheck
TEST_BIN := test_runner
GUI_BUILD_DIR := frontend/gui/build
GUI_BIN := $(GUI_BUILD_DIR)/spell_gui

CORE_SRC := \
	src/data_structures/Trie.cpp \
	src/core/EditDistance.cpp \
	src/core/SuggestionBuffer.cpp \
	src/core/SpellChecker.cpp \
	src/utils/Parser.cpp

CLI_SRC := $(CORE_SRC) frontend/tui.cpp src/main.cpp
CLI_OBJ := $(CLI_SRC:.cpp=.o)

TEST_SRC := \
	tests/test_main.cpp \
	tests/unit/test_linkedlist.cpp \
	tests/unit/test_trie.cpp \
	tests/unit/test_editdistance.cpp \
	tests/integration/test_workflow.cpp \
	tests/integration/test_io.cpp \
	tests/edge_cases/test_empty.cpp \
	tests/edge_cases/test_single.cpp \
	tests/edge_cases/test_limits.cpp \
	tests/performance/test_stress.cpp \
	tests/performance/test_memory.cpp \
	tests/fixtures/test_helpers.cpp \
	$(CORE_SRC) \
	frontend/tui.cpp

TEST_OBJ := $(TEST_SRC:.cpp=.o)
FORMAT_FILES := $(shell find include src frontend tests \( -name '*.hpp' -o -name '*.h' -o -name '*.cpp' \) 2>/dev/null)
UML_SRC := $(wildcard docs/uml/*.puml)

.PHONY: all run run-cli test gui docs uml deps check-deps format memcheck clean help

all: $(APP) gui

help:
	@echo "Available targets:"
	@echo "  make            Build CLI fallback and required Qt GUI"
	@echo "  make run        Build and run the Qt GUI"
	@echo "  make run-cli    Build and run the command-line fallback"
	@echo "  make test       Build and run all tests"
	@echo "  make docs       Generate Doxygen HTML and UML PNGs"
	@echo "  make format     Format C++ source files"
	@echo "  make deps       Install dependencies with brew or apt-get"
	@echo "  make clean      Remove generated build artifacts"

$(APP): $(CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(CLI_OBJ)

gui:
	@echo "Building required Qt6 GUI..."
	@if ! command -v cmake >/dev/null 2>&1; then \
		echo "Error: cmake is not installed. Run 'make deps' or install CMake manually."; \
		exit 1; \
	fi
	@if ! pkg-config --exists Qt6Widgets 2>/dev/null && ! command -v qmake6 >/dev/null 2>&1; then \
		echo "Error: Qt6 Widgets is not installed. Run 'make deps' or install Qt6 manually."; \
		exit 1; \
	fi
	@cmake -S frontend/gui -B $(GUI_BUILD_DIR)
	@cmake --build $(GUI_BUILD_DIR)
	@echo "GUI built: ./$(GUI_BIN)"

run: gui
	./$(GUI_BIN)

run-cli: $(APP)
	printf "exit\n" | ./$(APP) data/input_sample.json

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(TEST_OBJ)

docs: uml
	@echo "Generating Doxygen documentation..."
	@if ! command -v doxygen >/dev/null 2>&1; then \
		echo "Error: doxygen is not installed. Run 'make deps' or install Doxygen manually."; \
		exit 1; \
	fi
	doxygen docs/Doxyfile
	@echo "Documentation generated in docs/html/"

uml:
	@echo "Generating UML diagram images..."
	@if ! command -v plantuml >/dev/null 2>&1; then \
		echo "Error: plantuml is not installed. Run 'make deps' or install PlantUML manually."; \
		exit 1; \
	fi
	plantuml $(UML_SRC)

deps:
	@echo "Installing required dependencies when a supported package manager is available..."
	@if command -v brew >/dev/null 2>&1; then \
		brew install cmake qt doxygen clang-format plantuml pkg-config; \
	elif command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get update && sudo apt-get install -y build-essential cmake qt6-base-dev doxygen clang-format plantuml pkg-config; \
	else \
		echo "No supported package manager found. Install a C++17 compiler, CMake, Qt6 Widgets, Doxygen, clang-format, PlantUML, and pkg-config manually."; \
		exit 1; \
	fi

check-deps:
	@echo "Checking required dependencies..."
	@status=0; \
	if command -v "$(CXX)" >/dev/null 2>&1; then \
		echo "  OK: C++ compiler ($(CXX))"; \
	else \
		echo "  Missing: C++ compiler ($(CXX))"; \
		status=1; \
	fi; \
	if command -v cmake >/dev/null 2>&1; then \
		echo "  OK: cmake"; \
	else \
		echo "  Missing: cmake"; \
		status=1; \
	fi; \
	if command -v doxygen >/dev/null 2>&1; then \
		echo "  OK: doxygen"; \
	else \
		echo "  Missing: doxygen"; \
		status=1; \
	fi; \
	if command -v clang-format >/dev/null 2>&1; then \
		echo "  OK: clang-format"; \
	else \
		echo "  Missing: clang-format"; \
		status=1; \
	fi; \
	if command -v plantuml >/dev/null 2>&1; then \
		echo "  OK: plantuml"; \
	else \
		echo "  Missing: plantuml"; \
		status=1; \
	fi; \
	if pkg-config --exists Qt6Widgets 2>/dev/null; then \
		echo "  OK: Qt6 Widgets (pkg-config)"; \
	elif command -v qmake6 >/dev/null 2>&1; then \
		echo "  OK: Qt6 Widgets (qmake6)"; \
	else \
		echo "  Missing: Qt6 Widgets"; \
		status=1; \
	fi; \
	if [ $$status -eq 0 ]; then \
		echo "All required dependencies are available."; \
	else \
		echo "Install missing dependencies with 'make deps' or your package manager."; \
	fi; \
	exit $$status

format:
	@echo "Formatting code with clang-format..."
	@if ! command -v clang-format >/dev/null 2>&1; then \
		echo "Error: clang-format is not installed. Run 'make deps' or install clang-format manually."; \
		exit 1; \
	fi
	clang-format -i $(FORMAT_FILES)

memcheck: test
	@echo "Running memory check with Valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_BIN)

clean:
	rm -f $(CLI_OBJ) $(TEST_OBJ) $(APP) $(TEST_BIN)
	rm -rf $(GUI_BUILD_DIR)
	rm -rf docs/html
