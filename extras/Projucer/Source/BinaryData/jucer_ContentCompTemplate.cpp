/*
    FILENAME
    Created: DATE
    Author:  AUTHOR
*/

INCLUDE_CORRESPONDING_HEADER

//==============================================================================
CONTENTCOMPCLASS::CONTENTCOMPCLASS()
{
    setSize (600, 400);
}

CONTENTCOMPCLASS::~CONTENTCOMPCLASS()
{

}

void CONTENTCOMPCLASS::paint (Graphics& g)
{
    g.fillAll (Colour (0xffaa7711));
    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("CONTENTCOMPCLASS", getLocalBounds(), Justification::centred, true);
}

void CONTENTCOMPCLASS::resized()
{

}

