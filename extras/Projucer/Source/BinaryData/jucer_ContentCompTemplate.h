/*
    FILENAME
    Created: DATE
    Author:  AUTHOR
*/

#pragma once

INCLUDE_JUCE

//==============================================================================
/**
 * CONTENTCOMPCLASS
 */
class CONTENTCOMPCLASS
        : public Component
{
public:
    //==============================================================================
    CONTENTCOMPCLASS();
    ~CONTENTCOMPCLASS();

    void paint (Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CONTENTCOMPCLASS)
};

