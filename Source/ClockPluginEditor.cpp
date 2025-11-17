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
    updateFontSize();

    // Resizing aktivieren
    resizeLimits.setSizeLimits(150, 60, 2000, 2000);
    addAndMakeVisible(&resizer);
    setResizable(true, true);
    setResizeLimits(150, 60, 2000, 2000);   // optional, nice to have

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
    auto bounds = getLocalBounds();
    timeLabel.setBounds(bounds);

    resizer.setBounds(bounds.removeFromBottom(16).removeFromRight(16));

    updateFontSize();
}

void ClockPluginEditor::updateFontSize()
{
    auto area = getLocalBounds();
    int height = area.getHeight();

    // Eine schöne proportional skalierende Höhe:
    float fontHeight = height * 0.6f;   // 60% der Höhe

    juce::Font font (fontHeight);
    font.setBold (true);

    timeLabel.setFont(font);
}

void ClockPluginEditor::timerCallback()
{
    timeLabel.setText(getCurrentTimeString(), juce::dontSendNotification);
}

juce::String ClockPluginEditor::getCurrentTimeString() const
{
    auto now = juce::Time::getCurrentTime();
    return now.formatted("%H:%M:%S");
}

