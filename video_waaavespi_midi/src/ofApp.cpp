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
#include "ofApp.h"




float az = 1.0;
float sx = 0;
float dc = 0;
float fv = 1;
float gb = 1;
float hn = 1;
float jm = 0.0;
float kk = 0.25;
float ll = 0.0;
float qw = 0.0;

float op = 0.0;






int fb0_delayamount=0;



//dummy variables for midi control

float c8=0.0;
float c1=0.0;
float c2=0.0;
float c3=0.0;
float c4=0.0;
float c5=0.0;
float c6=0.0;
float c7=0.0;




//framebuffer management biziness

//fbob is the total number of frames that will be stored.  setting fbob to
//30 while fps is set to 30 means 1 second of video will be stored in memory  
const int fbob=11;


int framedelayoffset=0;  // this is used as an index to the circular framebuffers eeettt
unsigned int framecount=0; // framecounter used to calc offset eeettt

//this is an array of framebuffer objects
ofFbo pastFrames[fbob];


void incIndex()  // call this every frame to calc the offset eeettt
{
    
    framecount++;
    framedelayoffset=framecount % fbob;
}


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);

   
    ofSetFrameRate(30);

   
    ofBackground(0);
   
    ofHideCursor();
	
	int width=640;
	int height=480;
	cam1.initGrabber(width,height);
	
	//cam1.initGrabber(320,240);
	
	framebuffer0.allocate(width,height);
	
	framebuffer0.begin();
	ofClear(0,0,0,255);
	framebuffer0.end();
	
	//framebuffer1.allocate(width,height);
	
	/*
	framebuffer1.begin();
	ofClear(0,0,0,255);
	framebuffer1.end();
	*/
	
	 for(int i=0;i<fbob;i++){
        
        pastFrames[i].allocate(width, height);
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
        
    
    }//endifor

	
	shader_mixer.load("shadersES2/shader_mixer");
	
	// print input ports to console
	midiIn.listInPorts();
	
	// open port by number (you may need to change this)
	midiIn.openPort(1);
	//midiIn.openPort("IAC Pure Data In");	// by name
	//midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
	
	// don't ignore sysex, timing, & active sense messages,
	// these are ignored by default
	midiIn.ignoreTypes(false, false, false);
	
	// add ofApp as a listener
	midiIn.addListener(this);
	
	// print received messages to the console
	midiIn.setVerbose(true);
}

//--------------------------------------------------------------
void ofApp::update() {
	
	cam1.update();
}

//--------------------------------------------------------------
void ofApp::draw() {


	//begin midi biz
	
	
	for(unsigned int i = 0; i < midiMessages.size(); ++i) {

		ofxMidiMessage &message = midiMessages[i];
	
		// draw the last recieved message contents to the screen,
		// this doesn't print all the data from every status type
		// but you should get the general idea
		//stringstream text;
		//text << ofxMidiMessage::getStatusString(message.status);
		//while(text.str().length() < 16) { // pad status width
		//	text << " ";
		//}

		ofSetColor(127);
		if(message.status < MIDI_SYSEX) {
			//text << "chan: " << message.channel;
            if(message.status == MIDI_CONTROL_CHANGE) {
                
                //How to Midi Map
                //uncomment the line that says cout<<message control etc
                //run the code and look down in the console
                //when u move a knob on yr controller keep track of the number that shows up
                //that is the cc value of the knob
                //then go down to the part labled 'MIDIMAPZONE'
                //and change the numbers for each if message.control== statement to the values
                //on yr controller
                
                 // cout << "message.control"<< message.control<< endl;
                 // cout << "message.value"<< message.value<< endl;
                
                
                
                //MIDIMAPZONE
                //these are mostly all set to output bipolor controls at this moment (ranging from -1.0 to 1.0)
                //if u uncomment the second line on each of these if statements that will switch thems to unipolor
                //controls (ranging from 0.0to 1.0) if  you prefer
                
		//these controls are currently set to the default cc values of the korg nanostudio so if you got one of those yr in luck!
		//otherwise you will need to figure out the cc values for the knobs and sliders on your particular controller
		//and for each line where it says " if(message.control==XX)" replace XX with the cc value for the knob that you want to 
		//map for each control.  
		    
                
                
                //c1 maps to fb0 hue attenuation
                if(message.control==20){
                    c1=(message.value-63.0)/63.0;
                   //  c1=(message.value)/127.00;
                    
                }
                
                //c2 maps to fb0 saturation attenuation
                if(message.control==21){
                    c2=(message.value-63.0)/63.0;
                   //   c2=(message.value)/127.00;
                    
                }
                
                //c3 maps to fb0 brightness attenuation
                if(message.control==22){
                    c3=(message.value-63.0)/63.00;
                    //  c3=(message.value)/127.00;
                }
                
                //c4 maps to fb0_mix amount
                if(message.control==23){
                     c4=(message.value-63.0)/63.00;
                   // c4=(message.value)/127.00;
                   
                }
                
                //c5 maps to fb0 x displace
                if(message.control==24){
                     c5=(message.value-63.0)/63.00;
                  //  c5=(message.value)/127.00;
                  
                }
                
                //c6 maps to fb0 y displace
                if(message.control==25){
                    c6=(message.value-63.0)/63.0;
                    //  c4=(message.value)/127.00;
                }
                
                //c7 maps to fb0 z displace
                if(message.control==26){
                    c7=(message.value-63.0)/63.0;
                    //  c4=(message.value)/127.00;
                }
                
                //c8 maps to fb0 luma key value
                if(message.control==27){
                   //  c8=(message.value-63.0)/63.00;
                    c8=(message.value)/127.0;
                   
                }
                
              
            }
            else if(message.status == MIDI_PITCH_BEND) {
                //text << "\tval: " << message.value;
			
			}
			else {
				//text << "\tpitch: " << message.pitch;
				
                
               // int N= message.pitch;
               // int FN=N-69;
                // frequency=pow(2.0,FN/12.0)*440;
               

				
			}
			
		}//

	
	}
	
	
	//end midi biz
	
	
	
	//ok so here is the plan.  try out 1 framebuffer and test out how far back
//in time we can go.  the mix will have the following controls
//hsb for framebuffer 0 and 1
//positions on framebuffer0 ionly 
//some kind of chaotic hue mixing for fb0
//lumakeying on fb0 only
    

    
    
    //----------------------------------------------------------
    //
	
	framebuffer0.begin();
    shader_mixer.begin();

    cam1.draw(0, 0);
    
    shader_mixer.setUniformTexture("fb0", pastFrames[(abs(framedelayoffset-fbob-fb0_delayamount)-1)%fbob].getTexture(),1);
    shader_mixer.setUniformTexture("fb1", pastFrames[(abs(framedelayoffset-fbob)-1)%fbob].getTexture(),2);
    
    
    //mixing variables
    shader_mixer.setUniform1f("fb0_mix",jm+2.0*c4);
    shader_mixer.setUniform1f("fb0_lumakey_value",kk+1.1*c8);
   
    
    
    shader_mixer.setUniform1f("fb1_mix",op);
    
    //fb0_displacment variables
    shader_mixer.setUniform1f("fb0_xdisplace",sx+.01*c5);
    shader_mixer.setUniform1f("fb0_ydisplace",dc+.01*c6);
    shader_mixer.setUniform1f("fb0_zdisplace",az*(1+.1*c7));
    shader_mixer.setUniform1f("fb0_rotate",qw);
    
   
    
    
    ofVec3f hsbx;
    hsbx.set(fv*(1.0+.25*c1),gb*(1.0+.25*c2),hn*(1.0+.25*c3));
    shader_mixer.setUniform3f("fb0_hsbx",hsbx);

    shader_mixer.end();
	framebuffer0.end();
	
	//_----___---------_-_-----_--_-_--_--_
	
	framebuffer0.draw(0,0);
	
	/*
	for(int i=0;i<fbob;i++){
	pastFrames[(abs(framedelayoffset-fbob-fb0_delayamount)-1)%fbob].draw(i*100,0,100,100);
	}
	*/
	//_-------------------------------------------
	
	/*
	framebuffer1.begin();
		
	framebuffer0.draw(0,0);
	
	framebuffer1.end();
    */
	
	
	
	pastFrames[abs(fbob-framedelayoffset)-1].begin(); //eeettt
    
    
    ofPushMatrix();
  
  /*
    //recenter the coordinates so 0,0 is at the center of the screen
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2,0);
   
   
    ofTranslate(ff,gg,hh);
    ofRotateYRad(ss);
    ofRotateXRad(aa);
    ofRotateZRad(dd);
     ofRotateZRad(oo*TWO_PI/ii);
    */
    
    framebuffer0.draw(0,0);
    //cam1.draw(0,0);
    ofPopMatrix();
    
    
    pastFrames[abs(fbob-framedelayoffset)-1].end(); //eeettt
	
	incIndex();
   
	//ofDrawBitmapString("fps =" + ofToString(getFps()), 10, ofGetHeight() - 5 );

   ofSetColor(255);
   string msg="fps="+ofToString(ofGetFrameRate(),2)+" fb1mix="+ofToString(op,2);//+" z="+ofToString(az,5);
   ofDrawBitmapString(msg,10,10);
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {

	// add the latest message to the message queue
	midiMessages.push_back(msg);

	// remove any old messages if we have too many
	while(midiMessages.size() > maxMessages) {
		midiMessages.erase(midiMessages.begin());
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//error-if i decrement fb0_delayamount it always crashes...
	if (key == '[') {fb0_delayamount += 1;}
    if (key == ']') {
		fb0_delayamount = fb0_delayamount-1;
		if(fb0_delayamount<0){
			fb0_delayamount=fbob-fb0_delayamount;
		}//endiffb0
	}//endifkey
    
    //fb1 mix
    if (key == 'o') {op += .01;}
    if (key == 'p') {op -= .01;}
    
    //fb0 z displace
    if (key == 'a') {az += .0001;}
    if (key == 'z') {az -= .0001;}
    
    //fb0 x displace
    if (key == 's') {sx += .0001;}
    if (key == 'x') {sx -= .0001;}
    
    //fb0 y displace
    if (key == 'd') {dc += .0001;}
    if (key == 'c') {dc -= .0001;}
    
    //fb0 hue attenuate
    if (key == 'f') {fv += .001;}
    if (key == 'v') {fv -= .001;}
    
    //fb0 saturation attenuate
    if (key == 'g') {gb += .001;}
    if (key == 'b') {gb -= .001;}
    
    //fb0 brightness attenuate
    if (key == 'h') {hn += .001;}
    if (key == 'n') {hn -= .001;}
    
    //fb0 mix
    if (key == 'j') {jm += .01;}
    if (key == 'm') {jm -= .01;}
    
    //fb0 lumakey value
    if (key == 'k') {kk = ofClamp(kk+.01,0.0,1.0);}
    if (key == ',') {kk = ofClamp(kk-.01,0.0,1.0);}
    
    if (key == 'l') {ll += .01;}
    if (key == '.') {ll -= .01;}
    
    //fb0 rotation
    if (key == 'q') {qw += .0001;}
    if (key == 'w') {qw -= .0001;}


    if (key == '1') {

        //clear the framebuffer if thats whats up
        framebuffer0.begin();
        ofClear(0, 0, 0, 255);
        framebuffer0.end();

      for(int i=0;i<fbob;i++){
        
       
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
        
    
    }//endifor
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased() {
}
