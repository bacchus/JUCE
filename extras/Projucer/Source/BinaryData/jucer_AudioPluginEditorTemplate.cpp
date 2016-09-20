/*
    FILENAME
    Created: DATE
    Author:  AUTHOR
*/

EDITORCPPHEADERS

//==============================================================================
EDITORCLASSNAME::EDITORCLASSNAME (FILTERCLASSNAME& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

EDITORCLASSNAME::~EDITORCLASSNAME()
{
}

//==============================================================================
void EDITORCLASSNAME::paint (Graphics& g)
{
    g.fillAll (Colour (0xeeaa7711));
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawText ("EDITORCLASSNAME", getLocalBounds(), Justification::centred, 1);
}

void EDITORCLASSNAME::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

