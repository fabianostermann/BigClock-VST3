#include "ClockPluginEditor.h"

ClockPluginEditor::ClockPluginEditor(ClockPluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setOpaque(true);

    timeLabel.setJustificationType(juce::Justification::centred);
    timeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    
    juce::Font font (48.0f);
    font.setBold (true);
    timeLabel.setFont (font);

    addAndMakeVisible(timeLabel);

    setSize(300, 120);

    timeLabel.setText(getCurrentTimeString(), juce::dontSendNotification);
    startTimerHz(1);
}

ClockPluginEditor::~ClockPluginEditor()
{
    stopTimer();
}

void ClockPluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void ClockPluginEditor::resized()
{
    timeLabel.setBounds(getLocalBounds());
}

void ClockPluginEditor::timerCallback()
{
    timeLabel.setText(getCurrentTimeString(), juce::dontSendNotification);
}

juce::String ClockPluginEditor::getCurrentTimeString() const
{
    auto now = juce::Time::getCurrentTime();
    return now.toString(true, true); // 24h + Sekunden
}

