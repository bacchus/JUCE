/*
    FILENAME
    Created: DATE
    Author:  AUTHOR
*/

INCLUDE_CORRESPONDING_HEADER

INCLUDE_JUCE

//==============================================================================
COMPONENTCLASS::COMPONENTCLASS()
{

}

COMPONENTCLASS::~COMPONENTCLASS()
{

}

void COMPONENTCLASS::paint (Graphics& g)
{
    g.fillAll (Colour (0xffaa7711));
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("COMPONENTCLASS", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void COMPONENTCLASS::resized()
{

}

