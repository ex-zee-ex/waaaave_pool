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
#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
//#include "ofxOMXVideoGrabber.h"


class ofApp : public ofBaseApp, public ofxMidiListener {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void cameraSetup(int w, int h);
	void cameraUpdate();

	//personal procedures
	void allocateFbo(int w, int h);//declare and allocate
	void clearFbo();//clear all fbo
	
	void p_lockClear();
	void p_lockUpdate();
	/*
	void omx_settings();
	void omx_updates();
	ofxOMXCameraSettings settings;
    ofxOMXVideoGrabber videoGrabber;
	*/
	void midiSetup();
	void midibiz();
	
	void newMidiMessage(ofxMidiMessage& eventArgs);
	
	ofxMidiIn midiIn;
	std::vector<ofxMidiMessage> midiMessages;
	std::size_t maxMessages = 10; //< max number of messages to keep track of
	
	
	
	
	ofShader shader_mixer;
	ofShader shaderSharpen;
    
    
    ofFbo framebuffer0;
    ofFbo dry_framebuffer;
    ofFbo sharpenFramebuffer;
    
    
    ofVideoGrabber cam1;
    
    
    
    
    
};
