#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

using KnobNamePair = std::pair<StringRef, StringRef>;

class PedalKnobLookAndFeel : public LookAndFeel_V4
{
public:
    PedalKnobLookAndFeel()  = default;
    ~PedalKnobLookAndFeel() = default;

    void drawRotarySlider(
        Graphics&,
        int x,
        int y,
        int width,
        int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        Slider&
    ) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PedalKnobLookAndFeel)
};

class PedalComponent : public Component
{
public:
    PedalComponent(
        const StringRef    &name,
        const StringRef    &shortName,
        const Colour       &colour,
        const KnobNamePair &knobs
    );
    ~PedalComponent();

private:
    void mouseUp(const MouseEvent&) override;
    void paint (Graphics&) override;
    void resized() override;

    // SharedResourcePointer acts as static storage and lets us use one shared
    // PedalKnobLookAndFeel object no matter how many PedalComponents we create
    SharedResourcePointer<PedalKnobLookAndFeel> lookAndFeel;

    Colour colour;
    Slider knobs[2];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PedalComponent)
};
