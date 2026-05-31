cmake_minimum_required(VERSION 3.16)

project(spell_gui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SPELL_CORE_SOURCES
    ../../src/data_structures/Trie.cpp
    ../../src/core/EditDistance.cpp
    ../../src/core/SpellChecker.cpp
    ../../src/core/SuggestionBuffer.cpp
    ../../src/utils/Parser.cpp
)

add_executable(spell_gui
    main.cpp
    spellchecker.cpp
    SpellCheckerWidget.cpp
    ${SPELL_CORE_SOURCES}
)

# Copy input_large.json next to the build to ensure runtime access
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../data/input_large.json
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

target_include_directories(spell_gui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(spell_gui PRIVATE Qt6::Widgets)

# Older GCC releases need an explicit link to stdc++fs for <filesystem>
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        target_link_libraries(spell_gui PRIVATE stdc++fs)
    endif()
endif()
