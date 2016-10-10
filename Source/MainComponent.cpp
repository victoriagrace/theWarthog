/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "vicSynth.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);
        
        nChans = 1; // number of output audio channels
        
        setAudioChannels (0, nChans);
        
        audioBuffer = new float*[nChans];
    }

    ~MainContentComponent()
    {
        shutdownAudio();
        delete [] audioBuffer;

    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.
        currentSampleRate = sampleRate;
        blockSize = samplesPerBlockExpected;
        
        synth.init(sampleRate); // initializing the Faust module
        synth.buildUserInterface(&synthControl); // linking the Faust module to the controler
        
        // Print the list of parameters address of "saw"
        // To get the current (default) value of these parameters, sawControl.getParamValue("paramPath") can be used
        for(int i=0; i<synthControl.getParamsCount(); i++){
            std::cout << synthControl.getParamAdress(i) << "\n";
        }
            synthControl.setParamValue("/saw/freq",2000);
            synthControl.setParamValue("/saw/gain",0.5);
            synthControl.setParamValue("/saw/gate",1);
            synthControl.setParamValue("/saw/cutoff",1000);
            
        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        audioBuffer[0] = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        
        // any processing before going to "saw" could be done here in a dedicated buffer loop
        
        synth.compute(blockSize, NULL, audioBuffer); // computing one block with Faust
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);


        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================

    // Your private member variables go here...
    MapUI synthControl;
            
            
    vicSynth synth;
    int blockSize, nChans;
    double currentSampleRate;
    
    float** audioBuffer; // multichannel audio buffer used both for input and output

    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
