#include "ClockPluginProcessor.h"
#include "ClockPluginEditor.h"

ClockPluginProcessor::ClockPluginProcessor()
    : juce::AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                               .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                               .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
    // addParameter() transfers ownership; we keep a bare pointer for access.
    auto* p = new TimeDisplayParameter();
    addParameter(p);
    timeParam = p;

    // Prime the value immediately so hosts see something on load.
    timeParam->tick();
}

void ClockPluginProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    currentSampleRate   = sampleRate;
    samplesSinceLastTick = 0;
}

void ClockPluginProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ClockPluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif
    return true;
#endif
}
#endif

void ClockPluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                        juce::MidiBuffer& midi)
{
    juce::ignoreUnused(midi);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        buffer.clear(ch, 0, buffer.getNumSamples());

    // Tick the time parameter roughly once per second.
    samplesSinceLastTick += buffer.getNumSamples();
    int samplesPerSecond = static_cast<int>(currentSampleRate);

    if (samplesSinceLastTick >= samplesPerSecond)
    {
        samplesSinceLastTick -= samplesPerSecond;
        timeParam->tick();
    }
}

juce::AudioProcessorEditor* ClockPluginProcessor::createEditor()
{
    return new ClockPluginEditor(*this);
}

void ClockPluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ignoreUnused(destData);
}

void ClockPluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ClockPluginProcessor();
}
