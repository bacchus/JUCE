//==============================================================================
class COMPONENTCLASS
        : public Component
{
public:
    COMPONENTCLASS()
    {

    }

    ~COMPONENTCLASS()
    {

    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colour (0xffaa7711));
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("COMPONENTCLASS", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (COMPONENTCLASS)
};

