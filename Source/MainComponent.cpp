#include "MainComponent.h"

const StringRef MainComponent::PedalNames[] = {
    "Phaser",
    "Over Drive",
    "Spectrum",
    "Distortion",
};

const StringRef MainComponent::PedalShortNames[] = {
    "PH-1",
    "OD-1",
    "SP-1",
    "DS-1",
};

const KnobNamePair MainComponent::PedalKnobNames[] = {
    {"RATE", "DEPTH"},
    {"LEVEL", "OVER DRIVE"},
    {"BALANCE", "SPECTRUM"},
    {"TONE", "DIST"},
};

const Colour MainComponent::PedalColours[] = {
    Colour(0xFF14BB67),
    Colour(0xFFFFD85D),
    Colour(0xFFF90C14),
    Colour(0xFFE96C2F),
};

const Rectangle<int> MainComponent::PedalSize = {265, 475};

// =============================================================================

MainComponent::MainComponent()
{
    pedalView.setViewedComponent(new Component());
    pedalView.setScrollBarsShown(false, true);
    addAndMakeVisible(pedalView);

    pedalReset.setButtonText("reset");
    addAndMakeVisible(pedalReset);

    pedalAdd.setButtonText("+");
    pedalAdd.setTriggeredOnMouseDown(true);
    addAndMakeVisible(pedalAdd);

    setSize(800, 600);

    // Many JUCE widgets offer std::function members to use as callbacks for
    // actions. Here we can show a popup menu of pedal types whenever the
    // user clicks the "+" button
    pedalAdd.onClick = [this]() -> void {
        PopupMenu menu;

        for (const auto& name : PedalNames)
            menu.addItem(menu.getNumItems() + 1, name);

        // Popup menus can also take a std::function object as a callback to use
        // when the user selects an item. Here we simply pass the choice ID to
        // our addPedal() function.
        menu.showMenuAsync(
            PopupMenu::Options()
                .withParentComponent(this)
                .withTargetComponent(pedalAdd),
            [this](int choice) -> void { addPedal(choice); }
        );
    };

    // When we click the reset button, we need to remove all pedals from the
    // component that holds them and delete them. We also reset the bounds of
    // the holder component so that the viewport scrollbars no longer show
    pedalReset.onClick = [this]() -> void {
        auto* const pedalArea = pedalView.getViewedComponent();
        pedalArea->removeAllChildren();
        pedalArea->setBounds({});
        pedals.clear();
    };
}

void MainComponent::addPedal(int choice)
{
    // A choice of zero indicates that the user clicked away from the menu
    // without making a selection
    if (!choice)
        return;

    static const int TotalWidth = PedalSize.getWidth() + 10;

    auto* const pedal = new PedalComponent(
        PedalNames      [choice - 1],
        PedalShortNames [choice - 1],
        PedalColours    [choice - 1],
        PedalKnobNames  [choice - 1]
    );

    pedal->setBounds(PedalSize.translated(
        10 + TotalWidth * pedals.size(), 25
    ));

    pedals.add(pedal);

    // When we add a new pedal, we need to expand the component that holds them
    auto* const pedalArea = pedalView.getViewedComponent();
    pedalArea->setSize(TotalWidth * pedals.size(), pedalView.getHeight());
    pedalArea->addAndMakeVisible(pedal);
}

void MainComponent::paint(Graphics& g)
{
    // Colours can be registered for components using unique IDs and later
    // retrieved with findColour(). JUCE's default LookAndFeel provides
    // components with a standard set of colours for all the IDs in the library
    g.fillAll(findColour(ResizableWindow::backgroundColourId));

    {
        auto viewBounds = pedalView.getBounds();

        ColourGradient gradient;
        gradient.point1 = viewBounds.getBottomLeft().toFloat();
        gradient.point2 = viewBounds.getTopLeft().toFloat();
        gradient.addColour(0.0, Colours::black);
        gradient.addColour(1.0, Colours::black.brighter(0.1f));
        g.setGradientFill(gradient);
        g.fillRect(pedalView.getBounds());
    }
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();

    // The "removedFrom___()" rectangle methods let you quickly slice off
    // sections from a larger area
    {
        auto toolbar = bounds.removeFromTop(35).reduced(5);
        pedalReset   .setBounds(toolbar.removeFromLeft(50));
        pedalAdd     .setBounds(toolbar.removeFromRight(50));
    }

    pedalView.setBounds(bounds);
}
