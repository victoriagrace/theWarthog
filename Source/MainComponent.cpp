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
class MainContentComponent   : public AudioAppComponent, private Slider::Listener, private ToggleButton::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);
        
        nChans = 2; // number of output audio channels
        
        setAudioChannels (0, nChans);
        
        audioBuffer = new float*[nChans]; // allocate memory
        
    // Dial 1
        freqSlider.setSliderStyle (Slider::LinearBarVertical);
        freqSlider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible (freqSlider);
       // freqSlider.setBounds (600, 100, 20, 230);
        freqSlider.setRange (50.0, 2000.0);
        freqSlider.setSkewFactorFromMidPoint (500.0); // [4]
        freqSlider.addListener (this);
   
        // Dial 2
        gainSlider.setSliderStyle (Slider::LinearHorizontal);
        gainSlider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
        addAndMakeVisible (gainSlider);
        //gainSlider.setBounds (300, 300, 230, 20);
        gainSlider.setRange (0.0, 1.0);
       // gainSlider.setSkewFactorFromMidPoint(0.5);
        gainSlider.addListener (this);
        
        
        // Dial 3
        cutoffSlider.setSliderStyle (Slider::LinearBarVertical);
        //cutoffSlider.setTextBoxStyle (Slider::NoTextBox, false, 0, 0);
       // cutoffSlider.setBounds (100, 100, 20, 230);
        addAndMakeVisible (cutoffSlider);
        cutoffSlider.setRange (50.0, 3000.0);
        //cutoffSlider.setSkewFactorFromMidPoint(0.5);
        cutoffSlider.addListener (this);
        
        // On and Off button
        File snout = (String)"/Users/victoriagrace/Desktop/Music 256A/theWarthog/snout.png";
        
        if(snout.exists()) {
            std::cout << "Success!\n";
        } else {
            std::cout << "Not a succes. womp womp :(\n";
        }
        
        juceImage = ImageFileFormat::loadFrom(snout);

        onOffB.setImages (true, true, true,
                       juceImage, 0.7f, Colours::pink,
                       juceImage, 1.0f, Colours::peachpuff,
                       juceImage, 1.0f, Colours::red, //
                       0.5f);
        addAndMakeVisible(onOffB);

        onOffB.addListener(this);

        tog = 1;
        
        resized();
    }

    ~MainContentComponent()
    {
        shutdownAudio();
        delete [] audioBuffer; // when app is terminated deallocate memory

    }


    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.
        currentSampleRate = sampleRate;
        blockSize = samplesPerBlockExpected;
        
        synth.init(sampleRate); // initializing the Faust module
        synth.buildUserInterface(&synthControl); // linking the Faust module to the controler/ MAPUI object
        
        // Print the list of parameters address of "saw"
        // To get the current (default) value of these parameters, sawControl.getParamValue("paramPath") can be used
        for(int i=0; i<synthControl.getParamsCount(); i++){
            std::cout << synthControl.getParamAdress(i) << "\n";
        }
            synthControl.setParamValue("/saw/freq",200);
            synthControl.setParamValue("/saw/gain",0.5);
            synthControl.setParamValue("/saw/gate",1);
            synthControl.setParamValue("/saw/cutoff",1000);
            
        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
//        const float freq= freqSlider.getValue();
        
        for(int i=0; i<nChans; i++){
            audioBuffer[i] = bufferToFill.buffer->getWritePointer (i, bufferToFill.startSample);

        }
        
        synth.compute(blockSize, NULL, audioBuffer); // computing one block with Faust        // any processing before going to "saw" could be done here in a dedicated buffer loop


        for (int sample=0; sample < bufferToFill.numSamples; ++ sample){
            audioBuffer[1][sample]=audioBuffer[0][sample];
        }
        
        
        
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
       // g.fillAll (Colours::black);
        
        File f = (String)"/Users/victoriagrace/Desktop/backgrounds/hay.jpg";
        Image hay = ImageFileFormat::loadFrom(f);
        g.drawImageAt(hay,0,0,false);

        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        
        
        const int border = 4;
        
        
        
        Rectangle<int> area = getLocalBounds();
        int w = area.getWidth();
        int h = area.getHeight();
        int gridW = w/8;
        int gridH = h/8;
        int sliderWidth = 20; // for the offset
        
        freqSlider.setBounds(2*gridW-sliderWidth,3*gridH,sliderWidth,3*gridH);
        cutoffSlider.setBounds(6*gridW,3*gridH,sliderWidth,3*gridH);
        
        gainSlider.setBounds(3*gridW,6*gridH,2*gridW,sliderWidth);
        onOffB.setBounds(3*gridW,3*gridH,2*gridW,3*gridH);

        //onOffB.setBounds (3*gridW, 3*gridH, 500, 500);

//        {
//            Rectangle<int> dialArea = area.removeFromTop (area.getHeight() / 2);
//            freqSlider.setBounds (dialArea.removeFromLeft (dialArea.getWidth() / 2).reduced (border));
//
//            
//            gainSlider.setBounds (dialArea.removeFromLeft (dialArea.getWidth() / 2).reduced (border));
//            cutoffSlider.setBounds (dialArea.removeFromLeft (dialArea.getWidth() / 2).reduced (border));
//
//
//        }
        
       

        
    }
            void sliderValueChanged (Slider* slider) override
            {
                if (currentSampleRate > 0.0){
                    if (slider == &freqSlider)
                    {
                        synthControl.setParamValue("/saw/freq", freqSlider.getValue());
                    }
                    else if (slider == &gainSlider)
                    {
                        synthControl.setParamValue("/saw/gain", gainSlider.getValue());
                    }
                    else if (slider == &cutoffSlider)
                    {
                        synthControl.setParamValue("/saw/cutoff", cutoffSlider.getValue());
                    }
                }
            }
    
    /** Called when the button is clicked. */
    void buttonClicked (Button* button) override
    {
        if(button == &onOffB){
            if(tog == 0) {
                tog = 1;
            } else {
                tog = 0;
            }
            
            synthControl.setParamValue("/saw/gate", tog);
        }
        
    }
    
    /** Called when the button's state changes. */
    void buttonStateChanged (Button* button) override  {
       
    }

private:
    //==============================================================================

    // Your private member variables go here...
    MapUI synthControl;
            
            
    vicSynth synth;
    int blockSize, nChans;
    double currentSampleRate;
    
    float** audioBuffer; // multichannel audio buffer used both for input and output

    Slider freqSlider;
    Slider gainSlider;
    Slider cutoffSlider;
    
    ImageButton onOffB;
    Image juceImage;
    int tog;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
