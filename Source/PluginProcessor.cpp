#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
    juce::String layerId (int index, const juce::String& suffix)
    {
        return "L" + juce::String(index + 1) + "_" + suffix;
    }
}

ChicagoBeatsTurbineXAudioProcessor::ChicagoBeatsTurbineXAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, "TURBINE_X_STATE", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
ChicagoBeatsTurbineXAudioProcessor::createParameterLayout()
{
    using APF = juce::AudioParameterFloat;
    using APB = juce::AudioParameterBool;
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<APF>("MASTER", "Master", juce::NormalisableRange<float>(0.0f, 1.0f), 0.70f));
    layout.add (std::make_unique<APF>("RPM", "RPM", juce::NormalisableRange<float>(0.0f, 12000.0f, 1.0f), 2400.0f));
    layout.add (std::make_unique<APF>("THROTTLE", "Throttle", juce::NormalisableRange<float>(0.0f, 1.0f), 0.35f));

    const char* names[6] = { "Turbine", "Combustion", "Pressure", "Propulsion", "Mechanical", "Sample" };
    for (int i = 0; i < 6; ++i)
    {
        layout.add (std::make_unique<APB>(layerId(i, "ON"), names[i] + juce::String(" On"), i < 3));
        layout.add (std::make_unique<APF>(layerId(i, "LEVEL"), names[i] + juce::String(" Level"),
            juce::NormalisableRange<float>(0.0f, 1.0f), i < 3 ? 0.20f : 0.0f));
        layout.add (std::make_unique<APF>(layerId(i, "PITCH"), names[i] + juce::String(" Pitch"),
            juce::NormalisableRange<float>(20.0f, 2000.0f, 0.1f, 0.35f), 110.0f + i * 70.0f));
    }

    layout.add (std::make_unique<APF>("DRIVE", "Drive", juce::NormalisableRange<float>(0.0f, 1.0f), 0.15f));
    layout.add (std::make_unique<APF>("WIDTH", "Stereo Width", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    return layout;
}

void ChicagoBeatsTurbineXAudioProcessor::prepareToPlay (double sampleRate, int)
{
    currentSampleRate = sampleRate;
    for (auto& p : phase) p = 0.0;
}

void ChicagoBeatsTurbineXAudioProcessor::releaseResources() {}

void ChicagoBeatsTurbineXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int channels = buffer.getNumChannels();
    const int samples = buffer.getNumSamples();

    const float master = parameters.getRawParameterValue("MASTER")->load();
    const float drive = parameters.getRawParameterValue("DRIVE")->load();
    const float rpm = parameters.getRawParameterValue("RPM")->load();
    const float throttle = parameters.getRawParameterValue("THROTTLE")->load();

    // Phase 1 audio engine: real-time procedural oscillator/noise layers.
    // No file access, allocation, or blocking calls occur in the audio callback.
    for (int ch = 0; ch < channels; ++ch)
    {
        auto* out = buffer.getWritePointer(ch);
        for (int n = 0; n < samples; ++n)
        {
            float mixed = 0.0f;
            for (int layer = 0; layer < 6; ++layer)
            {
                const auto on = parameters.getRawParameterValue(layerId(layer, "ON"))->load() > 0.5f;
                const float level = parameters.getRawParameterValue(layerId(layer, "LEVEL"))->load();
                const float pitch = parameters.getRawParameterValue(layerId(layer, "PITCH"))->load();
                if (! on || level <= 0.0001f) continue;

                const double rpmRatio = juce::jlimit(0.1, 8.0, static_cast<double>(rpm / 2400.0f));
                const double hz = juce::jlimit(10.0, currentSampleRate * 0.45,
                                               static_cast<double>(pitch) * (0.35 + rpmRatio * 0.25));
                phase[layer] += hz / currentSampleRate;
                if (phase[layer] >= 1.0) phase[layer] -= 1.0;

                float sample = 0.0f;
                if (layer == 0) // turbine: bright harmonic rotor
                    sample = static_cast<float>(std::sin(phase[layer] * juce::MathConstants<double>::twoPi)
                              + 0.35 * std::sin(phase[layer] * juce::MathConstants<double>::twoPi * 2.7));
                else if (layer == 1) // combustion: low pulsing tone
                    sample = static_cast<float>(std::sin(phase[layer] * juce::MathConstants<double>::twoPi)
                              * (0.65 + 0.35 * std::sin(phase[layer] * juce::MathConstants<double>::twoPi * 0.5)));
                else if (layer == 2 || layer == 4) // pressure/mechanical: restrained noise texture
                    sample = (random.nextFloat() * 2.0f - 1.0f) * 0.22f;
                else if (layer == 3) // propulsion: rising harmonic
                    sample = static_cast<float>(std::sin(phase[layer] * juce::MathConstants<double>::twoPi * 1.5));
                else // sample layer placeholder tone; sample import comes in a later phase
                    sample = static_cast<float>(std::sin(phase[layer] * juce::MathConstants<double>::twoPi * 0.5));

                mixed += sample * level;
            }
            const float shaped = std::tanh(mixed * (1.0f + drive * 8.0f));
            out[n] = shaped * master * (0.25f + throttle * 0.75f);
        }
    }
}

juce::AudioProcessorEditor* ChicagoBeatsTurbineXAudioProcessor::createEditor()
{
    return new ChicagoBeatsTurbineXAudioProcessorEditor (*this);
}

void ChicagoBeatsTurbineXAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ChicagoBeatsTurbineXAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName (parameters.state.getType()))
        parameters.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChicagoBeatsTurbineXAudioProcessor();
}
