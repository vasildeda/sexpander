#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
                         .withInput("Sidechain", juce::AudioChannelSet::stereo(), true))
{
}

void PluginProcessor::prepareToPlay(double /*sampleRate*/, int /*samplesPerBlock*/)
{
    circularBuffer_.clear();
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    auto mainIn = layouts.getMainInputChannelSet();
    auto mainOut = layouts.getMainOutputChannelSet();

    if (mainIn != mainOut)
        return false;

    if (mainIn != juce::AudioChannelSet::mono()
        && mainIn != juce::AudioChannelSet::stereo())
        return false;

    auto sidechain = layouts.getChannelSet(true, 1);
    if (sidechain != juce::AudioChannelSet::mono()
        && sidechain != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto sidechainBuffer = getBusBuffer(buffer, true, 1);
    auto numSidechainChannels = sidechainBuffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    if (numSidechainChannels > 0)
    {
        for (int i = 0; i < numSamples; ++i)
        {
            auto sumOfSquares = 0.0f;

            for (int ch = 0; ch < numSidechainChannels; ++ch)
            {
                auto sample = sidechainBuffer.getSample(ch, i);
                sumOfSquares += sample * sample;
            }

            circularBuffer_.push(std::sqrt(sumOfSquares / static_cast<float>(numSidechainChannels)));
        }
    }
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& /*destData*/)
{
}

void PluginProcessor::setStateInformation(const void* /*data*/, int /*sizeInBytes*/)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
