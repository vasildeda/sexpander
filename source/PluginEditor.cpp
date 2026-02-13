#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(p),
      audioProcessor_(p)
{
    auto& apvts = audioProcessor_.getApvts();

    setupSlider(ratioSlider_, ratioLabel_, "Ratio");
    setupSlider(downwardSlewSlider_, downwardSlewLabel_, "Down Slew");
    setupSlider(upwardSlewSlider_, upwardSlewLabel_, "Up Slew");
    setupSlider(minGainSlider_, minGainLabel_, "Min Gain");
    setupSlider(maxGainSlider_, maxGainLabel_, "Max Gain");
    setupSlider(rmsWindowSlider_, rmsWindowLabel_, "RMS Window");

    ratioAttachment_ = std::make_unique<SliderAttachment>(apvts, "ratio", ratioSlider_);
    downwardSlewAttachment_ = std::make_unique<SliderAttachment>(apvts, "downwardSlew", downwardSlewSlider_);
    upwardSlewAttachment_ = std::make_unique<SliderAttachment>(apvts, "upwardSlew", upwardSlewSlider_);
    minGainAttachment_ = std::make_unique<SliderAttachment>(apvts, "minGain", minGainSlider_);
    maxGainAttachment_ = std::make_unique<SliderAttachment>(apvts, "maxGain", maxGainSlider_);
    rmsWindowAttachment_ = std::make_unique<SliderAttachment>(apvts, "rmsWindow", rmsWindowSlider_);

    setSize(600, 400);
}

void PluginEditor::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& text)
{
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(slider);

    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void PluginEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto labelHeight = 20;
    auto topRow = area.removeFromTop(area.getHeight() / 2);
    topRow.removeFromTop(labelHeight);
    auto bottomRow = area;
    bottomRow.removeFromTop(labelHeight);

    auto sliderWidth = topRow.getWidth() / 3;

    ratioSlider_.setBounds(topRow.removeFromLeft(sliderWidth));
    downwardSlewSlider_.setBounds(topRow.removeFromLeft(sliderWidth));
    upwardSlewSlider_.setBounds(topRow);

    minGainSlider_.setBounds(bottomRow.removeFromLeft(sliderWidth));
    maxGainSlider_.setBounds(bottomRow.removeFromLeft(sliderWidth));
    rmsWindowSlider_.setBounds(bottomRow);
}
