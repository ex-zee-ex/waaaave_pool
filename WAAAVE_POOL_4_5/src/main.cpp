/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#include "ofMain.h"
#include "ofApp.h"

int main(){
	//ofGLWindowSettings settings;
	//settings.setGLVersion(3,2);


	ofGLESWindowSettings settings;
	settings.glesVersion=2;
	
	//here is where you change output resolutions
	
	//CEA mode 2
	settings.setSize(720,480);
	
	//CEA mode 4
	//settings.setSize(1280,720);
	
	//CEA mode 16
	//settings.setSize(1920,1080);
	
	
	ofCreateWindow(settings);
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
