cmake_minimum_required(VERSION 3.15)
project(ClockJuce VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# Option: Release als Default
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()


# JUCE via FetchContent holen (keine manuelle Installation nötig)
include(FetchContent)


FetchContent_Declare(
JUCE
GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
# Stabiler Tag; bei Bedarf anpassen
GIT_TAG 8.0.10
)


FetchContent_MakeAvailable(JUCE)

set(CMAKE_CXX_STANDARD 17)

# Plugin anlegen
juce_add_plugin(ClockJuce
COMPANY_NAME "FabianOstermann"
PLUGIN_MANUFACTURER_CODE FAb1
PLUGIN_CODE Clk1
FORMATS VST3 Standalone
PRODUCT_NAME "ClockJuce"
IS_SYNTH FALSE
NEEDS_MIDI_INPUT FALSE
NEEDS_MIDI_OUTPUT FALSE
IS_MIDI_EFFECT FALSE
EDITOR_WANTS_KEYBOARD_FOCUS FALSE
COPY_PLUGIN_AFTER_BUILD TRUE
)


# Quellen
set(SOURCE_FILES
Source/ClockPluginProcessor.cpp
Source/ClockPluginProcessor.h
Source/ClockPluginEditor.cpp
Source/ClockPluginEditor.h
)


target_sources(ClockJuce PRIVATE ${SOURCE_FILES})

set(JUCE_VST2_DISABLED TRUE)

# JUCE Modul-Set


target_link_libraries(ClockJuce
PRIVATE
juce::juce_audio_utils
juce::juce_audio_processors
juce::juce_audio_devices
juce::juce_audio_formats
juce::juce_gui_extra
juce::juce_dsp
)

# Optional: Dinge deaktivieren, die du nicht brauchst (macht Build kleiner)
target_compile_definitions(ClockJuce
PRIVATE
JUCE_WEB_BROWSER=0
JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# never copy builds
set(JUCE_COPY_PLUGIN_AFTER_BUILD OFF CACHE BOOL "" FORCE)
