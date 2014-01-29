/*
 *  Author:
 *      Stjepan Rajko
 *      Arts, Media and Engineering Program
 *      Arizona State University
 *
 *  Copyright 2008 Arizona Board of Regents.
 *
 *  This file is part of the AME Patterns openFrameworks addon.
 *
 *  AMELiA is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the License,
 *  or (at your option) any later version.
 *
 *  AMELiA is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AMELiA.
 *  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include "testApp.h"
#include <fstream>


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);
	ofSetWindowTitle("vector gesture classification example");

	ofSetFrameRate(15); // mouse measurements are not precise enough for high framerates.
}

//--------------------------------------------------------------
void testApp::update(){

    if(recording)
    {
        // if recording, just record the current movement observation
        vector<double> observation(2);
        observation[0] = mouseX;
        observation[1] = mouseY;
        recordings.back().push_back(observation);
    }
    frame++;
}

//--------------------------------------------------------------
void testApp::draw(){

	ofFill();

	ofSetColor(0x000000);
    
    ofDrawBitmapString(
    "Instructions:\n\nYou will start in training mode.  Hold down the mouse button while executing a training sample with the mouse\n"
    "(e.g., an S-like shape) to record it. Repeat a few times for the same pattern (e.g., record an S-like shape multiple times),\n"
    "and then hit \"t\" to train a model.\n"
    "Once you've trained a few models (e.g., repeat training for a few more types of shapes), hit space to switch to testing mode.\n"
    "Again hold down the mouse button while executing testing samples. Each recording will be classified automatically.\n"
    "Note: for this example, the position and scale of the gesture will impact the recognition.\n\n"
    "Please see the documentation for copyright and license information.\n"
    "Example and underlying ofxPatterns / AME Patterns code by:\n\n    Stjepan Rajko\n"
    , 20, 450);

    if(testing)
        ofDrawBitmapString("last recognized gesture: " + ofToString(classification.lastRecognition()+1), 20, 30);
    else
    	ofDrawBitmapString("number of recorded gestures (hit 'c' to clear, or 't' to train): " + ofToString((int)recordings.size()), 20, 30);

    ofDrawBitmapString("trained gestures (hit the space bar to switch between training and testing): " + ofToString(classification.numPatterns()), 20, 50);

    ofDrawBitmapString("hit 's' or 'l' to save or load the gesture models.", 20, 70);

    // display the last recorded gesture
    if(!recordings.empty())
        for(int i=0; i!=recordings.back().size(); i++)
        {
            double color_ratio = double(i) / recordings.back().size();
            ofSetColor(255*color_ratio,0,255*(1-color_ratio),255);
            
            ofRect(recordings.back()[i][0],recordings.back()[i][1],5,5);
        }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    switch(key)
    {
        case 's' :
            {
                // save the gestures
                std::ofstream out("gesture.models", std::ios::binary);
                classification.save(out);
            }
            break;
        case 'l' :
            {
                // save the gestures
                std::ifstream in("gesture.models", std::ios::binary);
                if(in.is_open())
                    classification.load(in);
            }
            break;    
        case 't' :
            // train the gesture using the recorded examples (use 10 states per
            // gesture)
            if(!recordings.empty())
                classification.addPatternWithExamples(recordings, 10);
            // fall through
        case 'c' :
            // clear the recordings
            recordings.clear();
            break;
        case ' ' :
            // toggle training / testing mode
            testing = !testing;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    // if we are in testing mode, we only need to record one gesture at a time
    if(testing)
        recordings.clear();
    // start recording a gesture example
    recordings.push_back(vector<vector<double> >());
    recording = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
    // stop recording
    recording = false;
    // if the recording is empty, discard it
    if(recordings.back().empty())
        recordings.pop_back();
    
    // if we're testing and the recording succeeded, classify
    if(testing && !recordings.empty())
        // classification.classify returns the classification, but
        // it is also accessible through the classification.lastRecognition()
        // so we don't need to record it.
        classification.classify(recordings.back());
}
