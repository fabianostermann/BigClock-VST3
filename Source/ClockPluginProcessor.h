#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

// ── Read-only parameter that exposes the wall-clock time as a string ──────────
class TimeDisplayParameter : public juce::AudioProcessorParameter
{
public:
    TimeDisplayParameter() = default;

    // The normalised value encodes seconds-of-day so the host has *something*
    // numeric to track; getText() is what actually matters for display.
    float getValue() const override                    { return currentValue.load(); }
    void  setValue(float) override                     {}   // read-only
    float getDefaultValue() const override             { return 0.0f; }

    juce::String getName(int maxLen) const override
        { return juce::String("Time").substring(0, maxLen); }

    juce::String getLabel() const override             { return {}; }
    juce::String getText(float, int) const override
        { return juce::Time::getCurrentTime().formatted("%H:%M:%S"); }
    float getValueForText(const juce::String&) const override { return 0.0f; }

    bool isAutomatable()  const override               { return true; }
    bool isDiscrete()     const override               { return false; }
    bool isBoolean()      const override               { return false; }

    // Call this once per second (from processBlock or a timer).
    // Returns true when the value actually changed (i.e. a new second ticked).
    bool tick()
    {
        auto now   = juce::Time::getCurrentTime();
        float secs = float(now.getHours()   * 3600
                         + now.getMinutes() * 60
                         + now.getSeconds());
        float normalised = secs / 86400.0f;

        if (normalised == currentValue.load())
            return false;

        currentValue.store(normalised);
        sendValueChangedMessageToListeners(normalised);
        return true;
    }

private:
    std::atomic<float> currentValue { 0.0f };
};

// ─────────────────────────────────────────────────────────────────────────────

class ClockPluginProcessor : public juce::AudioProcessor
{
public:
    ClockPluginProcessor();
    ~ClockPluginProcessor() override = default;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
   #endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    // Owned by the processor via addParameter(); pointer is non-owning.
    TimeDisplayParameter* timeParam { nullptr };

    // Throttle: only call tick() ~once per second, not every block.
    int  samplesSinceLastTick { 0 };
    double currentSampleRate  { 44100.0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClockPluginProcessor)
};

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter();
