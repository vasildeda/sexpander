#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(p),
      audioProcessor_(p)
{
    auto& apvts = audioProcessor_.getApvts();

    setupSlider(rmsMinSlider_, rmsMinLabel_, "RMS Min");
    setupSlider(rmsMaxSlider_, rmsMaxLabel_, "RMS Max");
    setupSlider(curveSlider_, curveLabel_, "Curve");
    setupSlider(downwardSlewSlider_, downwardSlewLabel_, "Release");
    setupSlider(upwardSlewSlider_, upwardSlewLabel_, "Attack");
    setupSlider(minGainSlider_, minGainLabel_, "Min Gain");
    setupSlider(maxGainSlider_, maxGainLabel_, "Max Gain");
    setupSlider(rmsWindowSlider_, rmsWindowLabel_, "RMS Window");

    rmsMinAttachment_ = std::make_unique<SliderAttachment>(apvts, "rmsMin", rmsMinSlider_);
    rmsMaxAttachment_ = std::make_unique<SliderAttachment>(apvts, "rmsMax", rmsMaxSlider_);
    curveAttachment_ = std::make_unique<SliderAttachment>(apvts, "curve", curveSlider_);
    downwardSlewAttachment_ = std::make_unique<SliderAttachment>(apvts, "release", downwardSlewSlider_);
    upwardSlewAttachment_ = std::make_unique<SliderAttachment>(apvts, "attack", upwardSlewSlider_);
    minGainAttachment_ = std::make_unique<SliderAttachment>(apvts, "minGain", minGainSlider_);
    maxGainAttachment_ = std::make_unique<SliderAttachment>(apvts, "maxGain", maxGainSlider_);
    rmsWindowAttachment_ = std::make_unique<SliderAttachment>(apvts, "rmsWindow", rmsWindowSlider_);

    setSize(600, 500);
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
    auto gap = 40;
    auto logoHeight = 160;
    auto labelHeight = 20;
    auto rowHeight = (area.getHeight() - logoHeight - 2 * gap) / 2;

    area.removeFromTop(logoHeight);
    auto topRow = area.removeFromTop(rowHeight);
    topRow.removeFromTop(labelHeight);

    area.removeFromTop(gap);
    auto bottomRow = area.removeFromTop(rowHeight);
    bottomRow.removeFromTop(labelHeight);

    auto sliderWidth = topRow.getWidth() / 4;

    rmsMinSlider_.setBounds(topRow.removeFromLeft(sliderWidth));
    rmsMaxSlider_.setBounds(topRow.removeFromLeft(sliderWidth));
    curveSlider_.setBounds(topRow.removeFromLeft(sliderWidth));
    rmsWindowSlider_.setBounds(topRow);

    minGainSlider_.setBounds(bottomRow.removeFromLeft(sliderWidth));
    maxGainSlider_.setBounds(bottomRow.removeFromLeft(sliderWidth));
    upwardSlewSlider_.setBounds(bottomRow.removeFromLeft(sliderWidth));
    downwardSlewSlider_.setBounds(bottomRow);
}
