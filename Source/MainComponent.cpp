/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "step_sequencer.h"




class MainContentComponent
:
public AudioAppComponent,
public Timer
{
public:
    //==============================================================================
    MainContentComponent()
    :
    pos(0),
    step(0),
    numSteps(16),
    samplesPerStep(3000)
    {
        setSize (800, 600);
        setAudioChannels (2, 2);
        addAndMakeVisible(sequencerRow);
        startTimer(50);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void timerCallback()
    {
        sequencerRow.highlightPlayPosition(step);
    }
    //=======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {}

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        auto buf = bufferToFill.buffer;
        auto chans = buf->getNumChannels();

        
        for (int i = bufferToFill.startSample; i < bufferToFill.numSamples; ++i)
        {
            pos++;
            if (pos > samplesPerStep)
            {
                pos -= samplesPerStep;
                step++;
                
                if (step >= numSteps)
                    step -= numSteps;
                
                /* We can't call GUI functions from 
                 here... this is the audio thread. */
                
                stepTriggered = sequencerRow[step];
            }
            
            if (stepTriggered)
            {
                for (int channel = 0; channel < chans; ++channel)
                {
                    buf->getWritePointer(channel)[i] = rand.nextFloat();
                }
            }
            

        }
        
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //=======================================================================
    void paint (Graphics& g) override
    {
        
        g.fillAll (Colours::darkgrey.withMultipliedBrightness(0.4f));
        
    }

    void resized() override
    {
        sequencerRow.setBounds(0, 20, getWidth(), 20);
    }


private:
    int pos;
    int step;
    int numSteps;
    int samplesPerStep;
    bool stepTriggered;
    BasicStepSequencerRow sequencerRow;
    Random rand;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
