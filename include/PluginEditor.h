#pragma once

#include "PluginProcessor.h"
#include <juce_gui_basics/juce_gui_basics.h>

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    PluginProcessor& audioProcessor_;

    juce::Slider ratioSlider_, downwardSlewSlider_, upwardSlewSlider_,
                 minGainSlider_, maxGainSlider_, rmsWindowSlider_;
    juce::Label ratioLabel_, downwardSlewLabel_, upwardSlewLabel_,
                minGainLabel_, maxGainLabel_, rmsWindowLabel_;
    std::unique_ptr<SliderAttachment> ratioAttachment_, downwardSlewAttachment_,
        upwardSlewAttachment_, minGainAttachment_, maxGainAttachment_, rmsWindowAttachment_;

    void setupSlider(juce::Slider&, juce::Label&, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
