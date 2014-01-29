/*
 *  Authors:
 *      Dhi Aurrahman
 *
 *      Stjepan Rajko
 *      Arts, Media and Engineering Program
 *      Arizona State University
 *
 *  Copyright 2009 Dhi Aurrahman, Arizona Board of Regents.
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
 
 //[ speech_testAppcpp

#include "testApp.h"
#include <fstream>

//--------------------------------------------------------------
void testApp::setup(){
	
	ofBackground(255,255,255);
	ofSetWindowTitle("speech classification example");
	ofSoundStreamSetup(0,1,this, 44100, 256, 4);// audio stream setup
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofFill();

	ofSetColor(0x000000);
	
    ofDrawBitmapString(
    "Instructions:\n\nYou will start in training mode.  Hold down the mouse button while speaking a training sample (e.g., \"One\") to record it.\n"
    "Repeat a few times for the same pattern (e.g., record \"One\" multiple times), and then hit \"t\" to train a model.\n"
    "Once you've trained a few models (e.g., repeat training for \"Two\" and \"Three\"), hit space to switch to testing mode.\n"
    "Again hold down the mouse button to record samples.  This time, each recording will be classified automatically.\n\n"
    "Please see the documentation for copyright and license information.\n"
    "Example and underlying ofxPatterns / AME Patterns code by:\n\n    Dhi Aurrahman, Arif Setiawan, and Stjepan Rajko"
    , 20, 450);
    
	if(recording){
		ofDrawBitmapString("recording (release the mouse button to stop recording)", 20, 30);
	}else{
		ofDrawBitmapString("not recording (hold down the mouse button to record a sample)", 20, 30);
	}
	
	ofDrawBitmapString("size=" + ofToString((int)data.size()), 20, 50);
    
    if(testing)
        ofDrawBitmapString("last recognized gesture: " + ofToString(classification.lastRecognition()+1), 20, 90);
    else
    	ofDrawBitmapString("number of recorded samples (hit 'c' to clear, or 't' to train): " + ofToString((int)recordings.size()), 20, 90);

    ofDrawBitmapString("trained gestures (hit the space bar to switch between training and testing): " + ofToString(classification.numPatterns()), 20, 110);

    ofDrawBitmapString("hit 's' or 'l' to save or load the gesture models.", 20, 130);
	
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
            // train the gesture using the recorded examples
            // (let the library figure out the number of states)
            if(!recordings.empty())
				// do training
				classification.addPatternWithExamples(recordings);			
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
   recording = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased()
{
    //add sample
    Poco::Mutex::ScopedLock lock(mutex);
    {
        if(!data.empty() && recording)
        {
            recordings.push_back(data);
            recording = false;
            data.clear();
        }
    }
    
    if(testing && !recordings.empty())
    {
        classification.classify(recordings.back());
        recordings.clear();
    }
}

void testApp::audioReceived (float * input, int bufferSize, int nChannels)
{
    if(recording)
    {
        Poco::Mutex::ScopedLock lock(mutex);
        data.insert(data.end(), input , input + bufferSize);
    }
}
//]