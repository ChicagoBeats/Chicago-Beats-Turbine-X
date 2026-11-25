#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ChicagoBeatsTurbineXAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ChicagoBeatsTurbineXAudioProcessorEditor (ChicagoBeatsTurbineXAudioProcessor&);
    ~ChicagoBeatsTurbineXAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ChicagoBeatsTurbineXAudioProcessor& audioProcessor;
    juce::TabbedComponent tabs { juce::TabbedButtonBar::TabsAtTop };
    juce::Slider rpm, throttle, master, drive, width;
    juce::Label title;
    juce::ComponentBoundsConstrainer resizeLimits;
    std::unique_ptr<juce::ResizableCornerComponent> resizeGrip;
    juce::ComponentBoundsConstrainer gripConstrainer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChicagoBeatsTurbineXAudioProcessorEditor)
};
