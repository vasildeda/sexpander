#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
                         .withInput("Sidechain", juce::AudioChannelSet::stereo(), true))
{
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"ratio", 1}, "Ratio",
        juce::NormalisableRange<float>(1.0f, 8.0f, 0.0f, 1.0f),
        2.0f, juce::AudioParameterFloatAttributes().withLabel(":1")));

    auto slewRange = juce::NormalisableRange<float>(100.0f, 50000.0f, 0.0f, 0.3f);

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"downwardSlew", 1}, "Down Slew",
        slewRange, 10000.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB/s")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"upwardSlew", 1}, "Up Slew",
        slewRange, 10000.0f,
        juce::AudioParameterFloatAttributes().withLabel("dB/s")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"minGain", 1}, "Min Gain",
        juce::NormalisableRange<float>(-80.0f, 0.0f, 0.0f, 1.0f),
        -80.0f, juce::AudioParameterFloatAttributes().withLabel("dB")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"maxGain", 1}, "Max Gain",
        juce::NormalisableRange<float>(-40.0f, 12.0f, 0.0f, 1.0f),
        0.0f, juce::AudioParameterFloatAttributes().withLabel("dB")));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"rmsWindow", 1}, "RMS Window",
        juce::NormalisableRange<float>(1.0f, 1000.0f, 0.0f, 0.3f),
        100.0f, juce::AudioParameterFloatAttributes().withLabel("ms")));

    return layout;
}

void PluginProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    circularBuffer_.clear();

    auto rmsWindowMs = apvts_.getRawParameterValue("rmsWindow")->load();
    auto rmsWindowSamples = std::clamp(static_cast<int>(rmsWindowMs * 0.001f * static_cast<float>(sampleRate)),
                                       1, maxRmsWindowSize_);
    circularBuffer_.setSize(rmsWindowSamples);
    gainComputer_.prepare(sampleRate);
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

    gainComputer_.setRatio(apvts_.getRawParameterValue("ratio")->load());
    gainComputer_.setDownwardSlewRate(apvts_.getRawParameterValue("downwardSlew")->load());
    gainComputer_.setUpwardSlewRate(apvts_.getRawParameterValue("upwardSlew")->load());
    gainComputer_.setMinGain(apvts_.getRawParameterValue("minGain")->load());
    gainComputer_.setMaxGain(apvts_.getRawParameterValue("maxGain")->load());

    auto rmsWindowMs = apvts_.getRawParameterValue("rmsWindow")->load();
    auto rmsWindowSamples = std::clamp(static_cast<int>(rmsWindowMs * 0.001f * static_cast<float>(getSampleRate())),
                                       1, maxRmsWindowSize_);
    circularBuffer_.setSize(rmsWindowSamples);

    auto sidechainBuffer = getBusBuffer(buffer, true, 1);
    auto numSidechainChannels = sidechainBuffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    auto mainBuffer = getBusBuffer(buffer, false, 0);
    auto numMainChannels = mainBuffer.getNumChannels();

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

            auto gain = gainComputer_.process(circularBuffer_.getRms());

            for (int ch = 0; ch < numMainChannels; ++ch)
                mainBuffer.setSample(ch, i, mainBuffer.getSample(ch, i) * gain);
        }
    }
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts_.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(apvts_.state.getType()))
        apvts_.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}
