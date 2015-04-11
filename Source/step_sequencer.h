/*
  ==============================================================================

    step_sequencer.h
    Created: 11 Apr 2015 4:58:10pm
    Author:  Jim Credland

  ==============================================================================
*/

#ifndef STEP_SEQUENCER_H_INCLUDED
#define STEP_SEQUENCER_H_INCLUDED


#include "../JuceLibraryCode/JuceHeader.h"

class Step
:
public Component
{
public:
    Step(bool highlightAsMajorStep)
    :
    enabled(false),
    majorStep(highlightAsMajorStep),
    showPlayCursor(false)
    {}
    
    void mouseUp(const MouseEvent &) override
    {
        enabled = ! enabled;
        repaint();
    }
    
    void paint(Graphics & g)
    {
        if (enabled)
            g.fillAll(Colours::grey);
        else
            g.fillAll(Colours::black);
        
        g.setColour(showPlayCursor ? Colours::white : Colours::lightgrey);
        g.drawRect(getLocalBounds(), (majorStep || showPlayCursor) ? 3 : 1);
    }
    
    void setShowPlayCursor(bool isPlayCursorShowing)
    {
        if (isPlayCursorShowing != showPlayCursor)
        {
            showPlayCursor = isPlayCursorShowing;
            repaint();
        }
    }
    
    bool isEnabled() const { return enabled; }
private:
    bool enabled;
    bool majorStep;
    bool showPlayCursor;
};

class BasicStepSequencerRow
:
public Component
{
public:
    BasicStepSequencerRow()
    :
    numSteps(16)
    {
        for (int i = 0; i < numSteps; ++i)
        {
            auto newStep = new Step((i % 4) == 0);
            addAndMakeVisible(newStep);
            rowOfSteps.add(newStep);
        }
    }
    
    void resized()
    {
        int x = 0;
        int padding = 2;
        int stepWidth = (getWidth() / numSteps) - (2 * padding); /* padding on both sides. */
        int h = getHeight();
        
        for (auto & r: rowOfSteps)
        {
            r->setBounds(x + padding, 0, stepWidth, h);
            x += stepWidth + padding * 2;
        }
    }
    
    /** Find out whether we should play on a particular step. */
    bool operator[] (int index)
    {
        jassert(index >= 0 & index < numSteps);
        return rowOfSteps[index]->isEnabled();
    }
    
    void highlightPlayPosition(int indexOfPlayPosition)
    {
        for (int i = 0; i < numSteps; ++i)
            rowOfSteps[i]->setShowPlayCursor(indexOfPlayPosition == i);
    }
    
private:
    OwnedArray<Step> rowOfSteps;
    const int numSteps;
};



#endif  // STEP_SEQUENCER_H_INCLUDED
