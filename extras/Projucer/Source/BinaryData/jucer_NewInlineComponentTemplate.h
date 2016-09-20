/*
    FILENAME
    Created: DATE
    Author:  AUTHOR
*/

#pragma once

INCLUDE_JUCE

//==============================================================================
/**
 * COMPONENTCLASS
*/
class COMPONENTCLASS
        : public Component
{
public:
    COMPONENTCLASS()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

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
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (COMPONENTCLASS)
};

