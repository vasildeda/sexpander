cmake_minimum_required(VERSION 3.22)

project(Sexpander VERSION 1.0.0)

# Add JUCE
add_subdirectory(libs/juce)

# Create a JUCE plugin
juce_add_plugin(Sexpander
    COMPANY_NAME "Vasilovo"
    PLUGIN_MANUFACTURER_CODE VSLV
    PLUGIN_CODE Sexp
    FORMATS VST3
    PRODUCT_NAME "Sexpander"
    PLUGIN_NAME "Sexpander"
    VERSION 1.0.0
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER "Vasilovo"
)

# Add resources
juce_add_binary_data(BinaryResources SOURCES
    resources/background.svg
)

# Add source files
target_sources(Sexpander PRIVATE
    source/CircularBuffer.cpp
    source/GainComputer.cpp
    source/PluginProcessor.cpp
    source/PluginEditor.cpp
)

# Add headers
target_include_directories(Sexpander PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

# Add JUCE modules
target_link_libraries(Sexpander PRIVATE
    BinaryResources
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Set C++ standard
target_compile_features(Sexpander PUBLIC cxx_std_17)


# Disable VST2 to avoid parameter automation conflict with VST3
target_compile_definitions(Sexpander PRIVATE
    JUCE_VST3_CAN_REPLACE_VST2=0
)
