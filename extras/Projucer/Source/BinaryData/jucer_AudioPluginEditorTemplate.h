/*
    FILENAME
    Created: DATE
    Author:  AUTHOR
*/

#pragma once

EDITORHEADERS

//==============================================================================
/**
 * EDITORCLASSNAME
 */
class EDITORCLASSNAME
        : public AudioProcessorEditor
{
public:
    EDITORCLASSNAME (FILTERCLASSNAME&);
    ~EDITORCLASSNAME();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FILTERCLASSNAME& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EDITORCLASSNAME)
};
