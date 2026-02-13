#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(p),
      audioProcessor_(p)
{
    auto& apvts = audioProcessor_.getApvts();

    setupSlider(rmsMinSlider_, rmsMinLabel_, "RMS Min");
    setupSlider(rmsMaxSlider_, rmsMaxLabel_, "RMS Max");
    setupSlider(curveSlider_, curveLabel_, "Curve");
    setupSlider(downwardSlewSlider_, downwardSlewLabel_, "Down Slew");
    setupSlider(upwardSlewSlider_, upwardSlewLabel_, "Up Slew");
    setupSlider(minGainSlider_, minGainLabel_, "Min Gain");
    setupSlider(maxGainSlider_, maxGainLabel_, "Max Gain");
    setupSlider(rmsWindowSlider_, rmsWindowLabel_, "RMS Window");

    rmsMinAttachment_ = std::make_unique<SliderAttachment>(apvts, "rmsMin", rmsMinSlider_);
    rmsMaxAttachment_ = std::make_unique<SliderAttachment>(apvts, "rmsMax", rmsMaxSlider_);
    curveAttachment_ = std::make_unique<SliderAttachment>(apvts, "curve", curveSlider_);
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

    auto topSliderWidth = topRow.getWidth() / 4;
    rmsMinSlider_.setBounds(topRow.removeFromLeft(topSliderWidth));
    rmsMaxSlider_.setBounds(topRow.removeFromLeft(topSliderWidth));
    minGainSlider_.setBounds(topRow.removeFromLeft(topSliderWidth));
    maxGainSlider_.setBounds(topRow);

    auto bottomSliderWidth = bottomRow.getWidth() / 4;
    curveSlider_.setBounds(bottomRow.removeFromLeft(bottomSliderWidth));
    downwardSlewSlider_.setBounds(bottomRow.removeFromLeft(bottomSliderWidth));
    upwardSlewSlider_.setBounds(bottomRow.removeFromLeft(bottomSliderWidth));
    rmsWindowSlider_.setBounds(bottomRow);
}
