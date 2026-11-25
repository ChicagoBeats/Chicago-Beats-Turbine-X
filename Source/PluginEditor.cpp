#include "PluginEditor.h"

namespace
{
    class DarkPage : public juce::Component
    {
    public:
        DarkPage (const juce::String& heading, const juce::String& detail)
            : headingText(heading), detailText(detail) {}
        void paint (juce::Graphics& g) override
        {
            g.fillAll (juce::Colour(0xff111317));
            g.setColour (juce::Colour(0xffd51f2a));
            g.fillRect (0, 0, getWidth(), 3);
            g.setColour (juce::Colours::white);
            g.setFont (juce::Font(22.0f, juce::Font::bold));
            g.drawText (headingText, 22, 22, getWidth() - 44, 32, juce::Justification::left);
            g.setColour (juce::Colour(0xffaeb3bb));
            g.setFont (14.0f);
            g.drawFittedText (detailText, 22, 64, getWidth() - 44, 70,
                              juce::Justification::topLeft, 3);
        }
    private:
        juce::String headingText, detailText;
    };

    void styleSlider (juce::Slider& s, const juce::String& name)
    {
        s.setName(name);
        s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 20);
        s.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffe32636));
        s.setColour(juce::Slider::thumbColourId, juce::Colours::white);
        s.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        s.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xff454950));
        s.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff171a20));
    }
}

ChicagoBeatsTurbineXAudioProcessorEditor::ChicagoBeatsTurbineXAudioProcessorEditor
    (ChicagoBeatsTurbineXAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor(p)
{
    setResizable (true, true);
    resizeLimits.setMinimumSize (720, 480);
    resizeLimits.setMaximumSize (1800, 1200);
    setConstrainer (&resizeLimits);
    setSize (1040, 700);

    title.setText ("CHICAGO BEATS  |  TURBINE X", juce::dontSendNotification);
    title.setFont (juce::Font(24.0f, juce::Font::bold));
    title.setColour (juce::Label::textColourId, juce::Colours::white);
    title.setJustificationType (juce::Justification::centredLeft);
    addAndMakeVisible (title);

    styleSlider (rpm, "RPM");
    styleSlider (throttle, "THROTTLE");
    styleSlider (master, "MASTER");
    styleSlider (drive, "DRIVE");
    styleSlider (width, "WIDTH");
    addAndMakeVisible (rpm);
    addAndMakeVisible (throttle);
    addAndMakeVisible (master);
    addAndMakeVisible (drive);
    addAndMakeVisible (width);

    auto attach = [this] (juce::Slider& s, const juce::String& id)
    {
        // Attachments are stored as child components in later UI expansion.
        // Phase 1 uses parameter listeners below through onValueChange.
        s.setRange(0.0, 1.0, 0.001);
        if (id == "RPM") s.setRange(0.0, 12000.0, 1.0);
        s.setValue(audioProcessor.parameters.getRawParameterValue(id)->load());
        s.onValueChange = [this, &s, id]
        {
            if (auto* param = audioProcessor.parameters.getParameter(id))
                param->setValueNotifyingHost(param->convertTo0to1((float)s.getValue()));
        };
    };
    attach (rpm, "RPM");
    attach (throttle, "THROTTLE");
    attach (master, "MASTER");
    attach (drive, "DRIVE");
    attach (width, "WIDTH");

    tabs.setTabBarDepth (34);
    tabs.addTab ("ENGINE", juce::Colour(0xff171a20), new DarkPage("ENGINE CORE", "RPM dynamics and throttle control. Phase 1 includes a procedural six-layer audio prototype."), true);
    tabs.addTab ("LAYER MATRIX", juce::Colour(0xff171a20), new DarkPage("LAYER MATRIX", "Turbine • Combustion • Pressure • Propulsion • Mechanical • Sample. Dedicated layer editors and sample import are scheduled for the next phase."), true);
    tabs.addTab ("MOTION DESIGNER", juce::Colour(0xff171a20), new DarkPage("MOTION DESIGNER", "Flight paths, Doppler movement, distance, stereo trajectories, and automation curves."), true);
    tabs.addTab ("FX PROCESSING", juce::Colour(0xff171a20), new DarkPage("FX PROCESSING", "Drive is active in the Phase 1 signal path. Filters, compression, modulation, delay, and reverb will be added in later phases."), true);
    tabs.addTab ("RANDOM LAB", juce::Colour(0xff171a20), new DarkPage("RANDOM LAB", "Controlled randomization, parameter locks, morphing, and variation generation."), true);
    tabs.addTab ("PRESET VAULT", juce::Colour(0xff171a20), new DarkPage("PRESET VAULT", "Host state save/restore is implemented. Named presets, favorites, and Sound DNA library tools are planned."), true);
    addAndMakeVisible (tabs);

    resizeGrip = std::make_unique<juce::ResizableCornerComponent>(this, &gripConstrainer);
    addAndMakeVisible (*resizeGrip);
}

void ChicagoBeatsTurbineXAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(0xff08090c));
    g.setColour (juce::Colour(0xffbfc4cc));
    g.drawRect (getLocalBounds(), 1);
    g.setColour (juce::Colour(0xffd51f2a));
    g.fillRect (0, 0, getWidth(), 5);
    g.setColour (juce::Colour(0xff242830));
    g.fillRect (0, 76, getWidth(), 1);
}

void ChicagoBeatsTurbineXAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(14);
    title.setBounds (area.removeFromTop(52));
    area.removeFromTop(10);
    auto controls = area.removeFromTop(150);
    const int slot = controls.getWidth() / 5;
    rpm.setBounds (controls.removeFromLeft(slot).reduced(8));
    throttle.setBounds (controls.removeFromLeft(slot).reduced(8));
    master.setBounds (controls.removeFromLeft(slot).reduced(8));
    drive.setBounds (controls.removeFromLeft(slot).reduced(8));
    width.setBounds (controls.reduced(8));
    area.removeFromTop(10);
    tabs.setBounds (area);
    if (resizeGrip != nullptr)
        resizeGrip->setBounds (getWidth() - 18, getHeight() - 18, 16, 16);
}
