#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_graphics/juce_graphics.h>
#include "ClockPluginProcessor.h"

class ClockPluginEditor : public juce::AudioProcessorEditor,
                          private juce::Timer
{
public:
    explicit ClockPluginEditor(ClockPluginProcessor&);
    ~ClockPluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    juce::String getCurrentTimeString() const;

    ClockPluginProcessor& processorRef;
    juce::Label timeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClockPluginEditor)
};

