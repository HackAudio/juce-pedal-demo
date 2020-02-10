#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PedalComponent.h"

using KnobNamePair = std::pair<StringRef, StringRef>;

class MainComponent : public Component
{
public:
    // We define some static constants to use within our GUI code
    static const StringRef    PedalNames          [];
    static const StringRef    PedalShortNames     [];
    static const KnobNamePair PedalKnobNames      [];
    static const Colour       PedalColours        [];

    static const Rectangle<int> PedalSize;

    MainComponent();
    ~MainComponent() = default;

private:
    void addPedal(int);
    void paint (Graphics&) override;
    void resized() override;

    TextButton                 pedalReset;
    TextButton                 pedalAdd;
    Viewport                   pedalView;
    OwnedArray<PedalComponent> pedals;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
