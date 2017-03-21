/*
    FILENAME
    Created: DATE
    Author:  AUTHOR
*/

INCLUDE_JUCE

//==============================================================================
/**
 * This component lives inside our window
 * , and this is where you should put all
 * your controls and content.
*/
class MainContentComponent
    : public OpenGLAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);
    }

    ~MainContentComponent()
    {
        shutdownOpenGL();
    }

    void initialise() override
    {
    }

    void shutdown() override
    {
    }

    void render() override
    {
        OpenGLHelpers::clear (Colours::black);
    }

    void paint (Graphics& g) override
    {

    }

    void resized() override
    {

    }


private:
    // private member variables
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()    { return new MainContentComponent(); }

