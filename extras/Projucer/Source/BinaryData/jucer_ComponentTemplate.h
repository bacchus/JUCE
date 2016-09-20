/*
    auto-generated GUI class
    Created with Projucer version: %%version%%

    FILENAME
    Created: DATE
    Author:  AUTHOR

    only the code within the "//[xyz]" and "//[/xyz]"
    sections will stay when the file is re-saved.
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
%%includeJUCEHeader%%
//[/Headers]

%%includeFilesH%%

//==============================================================================
/**                                                                 //[Comments]
 * %%className%%
 *                                                                  //[/Comments]
 */
%%classDeclaration%%
{
public:
    //==============================================================================
    %%className%% (%%constructorParams%%);
    ~%%className%%();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    %%publicMemberDeclarations%%

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    %%privateMemberDeclarations%%

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%className%%)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
