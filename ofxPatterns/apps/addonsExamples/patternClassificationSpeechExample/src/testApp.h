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

#ifndef _TEST_APP
#define _TEST_APP

//[ speech_testApphpp

#include "ofxSpeechClassification.h"
#include "ofMain.h"

#include <Poco/Mutex.h>


class testApp : public ofSimpleApp{
	
	public:
        testApp() : frame(0), recording(false), testing(false)
        {}
		
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		void mouseReleased();
	
		void audioReceived (float * input, int bufferSize, int nChannels);

    private:

        // the recorded speech gestures used for training or classification
		vector<vector<float> > recordings;
	
		vector<float> data;
	
        // current frame
        int frame;
        // flag indicating whether we are recording a gesture
        bool recording;
        // flag indicating whether we are in testing mode (vs. training mode)
        bool testing;
        
        // the object used for recognition of a set of gestures
        ofSpeechClassification classification;
        
        // for synchronization with audio thread (might not be necessary, not sure)
        Poco::Mutex mutex;
        
};

//]

#endif
	
