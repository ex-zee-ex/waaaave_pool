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
float sx = .5;
float dc = .0;
float fv = .0;
float gb = 1;
float hn = 1;
float jm = 1;
float kk = 0.0;
float qw = 0.0;

float er=0.0;

float op = 0.0;

float fb0_lumakey_value=0.0;


float fb0_lumakey_threshold=0.25;


//dummy variables for midi control

float c8=0.0;
float c1=0.0;
float c2=0.0;
float c3=0.0;
float c4=0.0;
float c5=0.0;
float c6=0.0;
float c7=0.0;



int fb0_delayamount=0;


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
                //these are all set to output bipolor controls at this moment (ranging from -1.0 to 1.0)
                //if u uncomment the second line on each of these if statements that will switch thems to unipolor
                //controls (ranging from 0.0to 1.0) if  you prefer
                //then find the variable that youd like to control down in CAVARIABLEZONES or MIXERVARIBLEZONES
                //and substitute c1,c2, ..cn whichever control knob u wish the map
                
                if(message.control==20){
                    c1=(message.value-63.0)/63.0;
                   //  c1=(message.value)/127.00;
                    
                }
                
                if(message.control==21){
                    c2=(message.value-63.0)/63.0;
                   //   c2=(message.value)/127.00;
                    
                }
                
                if(message.control==22){
                    c3=(message.value-63.0)/63.00;
                    //  c3=(message.value)/127.00;
                }
                
                if(message.control==23){
                     c4=(message.value-63.0)/63.00;
                   // c4=(message.value)/127.00;
                   
                }
                
                if(message.control==24){
                     c5=(message.value-63.0)/63.00;
                  //  c5=(message.value)/127.00;
                  
                }
                if(message.control==25){
                    c6=(message.value-63.0)/63.0;
                    //  c4=(message.value)/127.00;
                }
                
                
                if(message.control==26){
                    c7=(message.value-63.0)/63.0;
                    //  c4=(message.value)/127.00;
                }
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
    shader_mixer.setUniform1f("fb0_mix",kk+2.0*c4);
    shader_mixer.setUniform1f("fb0_lumakey_value",fb0_lumakey_value+1.1*c8);
   
    
    
    shader_mixer.setUniform1f("fb1_mix",op);
    
    //fb0_displacment variables
    shader_mixer.setUniform1f("fb0_xdisplace",dc+.01*c5);
    shader_mixer.setUniform1f("fb0_ydisplace",fv+.01*c6);
    shader_mixer.setUniform1f("fb0_zdisplace",az*(1+.1*c7));
    shader_mixer.setUniform1f("fb0_rotate",qw);
    
   
    
    
    ofVec3f hsbx;
    hsbx.set(gb*(1.0+.25*c1),hn*(1.0+.25*c2),jm*(1.0+.25*c3));
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
    
    if (key == 'o') {op += .01;}
    if (key == 'p') {op -= .01;}
	
	if (key == 'a') {az += .0001;}
    if (key == 'z') {az -= .0001;}
	
	if (key == 's') {sx += .01;}
    if (key == 'x') {sx -= .01;}

    if (key == 'd') {dc += .0001;}
    if (key == 'c') {dc -= .0001;}
    
    if (key == 'f') {fv += .0001;}
    if (key == 'v') {fv -= .0001;}
    
    if (key == 'g') {gb += .001;}
    if (key == 'b') {gb -= .001;}
    
    if (key == 'h') {hn += .001;}
    if (key == 'n') {hn -= .001;}
    
    if (key == 'j') {jm += .001;}
    if (key == 'm') {jm -= .001;}
    
    if (key == 'l') {fb0_lumakey_value += .1;}
    if (key == '.') {fb0_lumakey_value -= .1;}
    
    if (key == 'k') {kk += .01;}
    if (key == ',') {kk -= .01;}
    
    if (key == 'q') {qw += .0001;}
    if (key == 'w') {qw -= .0001;}
    
    if (key == 'e') {er += .01;}
    if (key == 'r') {er -= .01;}


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
