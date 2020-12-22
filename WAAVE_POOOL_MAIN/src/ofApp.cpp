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

#include "iostream"

//flip this switch to try different scalings
//0 is 320 1 is 640
//if you reduce scale to 320 you can up the total delay time to
//about 4 seconds/ 120 frames
//so try that out sometime and see how that feels!
bool scaleswitch=1;
const int fbob=60;
//const int fbob=120;


//might be unnecessary now!!
bool hdmi_input_switch=0;

//0 is sd aspect ratio
//use definitely with all of the VSERPI devices 
//and anything else doing like 480i/p over hdmi
//1 is corner cropping to fill the screen
int hdmi_aspect_ratio_switch=0;

//0 is picaputre, 1 is usbinput
//note that this refers to the picapture SD1 HATs which are no longer
//(as of Nov 2020) in production.  left in for legacy support but
//mostly irrelevant to anyone just getting started with these after nov 2020
bool inputswitch=1;

//0 is wet (framebuffer fed from final output, internal
//feedback mode
//1 is dry (framebuffer fed direct from camera input,
//traditional video delay mode
bool wet_dry_switch=1;

float az = 1.0;
float sx = 0;
float dc = 0;
float fv = 1;
float gb = 1;
float hn = 1;
float jm = 0.0;
float kk = 0.0;
float ll = 0.0;
float qw = 0.0;

float er = 1.0;
float ty = 0.0;
float ui = 0.0;

float op = 0.0;

float fb1_brightx=0.0;

bool toroid_switch=0;

float testt=1.0;

bool y_skew_switch=FALSE;
bool x_skew_switch=FALSE;

bool luma_switch=FALSE;

bool x_mirror_switch=FALSE;

bool y_mirror_switch=FALSE;

float y_skew=0;

float x_skew=0;



int fb0_delayamount=0;



//dummy variables for midi control



int width=0;
int height=0;


//dummy variables for midi to audio attenuatiors
//0 is direct midi, 1 is low_x, 2 is mid_x, 3 is high_x
int control_switch=0;

float low_c1=0.0;
float low_c2=0.0;
float low_c3=0.0;
float low_c4=0.0;
float low_c5=0.0;
float low_c6=0.0;
float low_c7=0.0;
float low_c8=0.0;
float low_c9=0.0;
float low_c10=0.0;
float low_c11=0.0;
float low_c12=0.0;
float low_c13=0.0;
float low_c14=0.0;
float low_c15=0.0;
float low_c16=0.0;

float mid_c1=0.0;
float mid_c2=0.0;
float mid_c3=0.0;
float mid_c4=0.0;
float mid_c5=0.0;
float mid_c6=0.0;
float mid_c7=0.0;
float mid_c8=0.0;
float mid_c9=0.0;
float mid_c10=0.0;
float mid_c11=0.0;
float mid_c12=0.0;
float mid_c13=0.0;
float mid_c14=0.0;
float mid_c15=0.0;
float mid_c16=0.0;

float high_c1=0.0;
float high_c2=0.0;
float high_c3=0.0;
float high_c4=0.0;
float high_c5=0.0;
float high_c6=0.0;
float high_c7=0.0;
float high_c8=0.0;
float high_c9=0.0;
float high_c10=0.0;
float high_c11=0.0;
float high_c12=0.0;
float high_c13=0.0;
float high_c14=0.0;
float high_c15=0.0;
float high_c16=0.0;



bool clear_switch=0;
//toggle switch variables
bool fb0_h_invert=FALSE;
bool fb0_s_invert=FALSE;
bool fb0_b_invert=FALSE;

bool cam1_h_invert=FALSE;
bool cam1_s_invert=FALSE;
bool cam1_b_invert=FALSE;

bool fb0_h_mirror=FALSE;
bool fb0_v_mirror=FALSE;

bool clear_flip=FALSE;

bool x_2=FALSE;
bool x_5=FALSE;
bool x_10=FALSE;

bool y_2=FALSE;
bool y_5=FALSE;
bool y_10=FALSE;

bool z_2=FALSE;
bool z_5=FALSE;
bool z_10=FALSE;

bool theta_0=FALSE;
bool theta_1=FALSE;
bool theta_2=FALSE;

bool huexx_0=FALSE;
bool huexx_1=FALSE;
bool huexx_2=FALSE;

bool huexy_0=FALSE;
bool huexy_1=FALSE;
bool huexy_2=FALSE;

bool huexz_0=FALSE;
bool huexz_1=FALSE;
bool huexz_2=FALSE;

//framebuffer management biziness

//fbob is the total number of frames that will be stored.  setting fbob to
//30 while fps is set to 30 means 1 second of video will be stored in memory  
//75 seems to work ok with usb cams but the capture device might need a little more memory



//const int fbob=2;


int framedelayoffset=0;  // this is used as an index to the circular framebuffers eeettt
unsigned int framecount=0; // framecounter used to calc offset eeettt

//this is an array of framebuffer objects
ofFbo pastFrames[fbob];


void incIndex()  // call this every frame to calc the offset eeettt
{
    
    framecount++;
    framedelayoffset=framecount % fbob;
}


//p_lock biz
//maximum total size of the plock array
const int p_lock_size=240;


//p_lock_switch turns on and off p_locks
bool p_lock_switch=0;

bool p_lock_erase=0;

//maximum number of p_locks available...maybe there can be one for every knob
//for whatever wacky reason the last member of this array of arrays has a glitch
//so i guess just make an extra array and forget about it for now
const int p_lock_number=17;

//so how we will organize the p_locks is in multidimensional arrays
//to access the data at timestep x for p_lock 2 (remember counting from 0) we use p_lock[2][x]
float p_lock[p_lock_number][p_lock_size];


//smoothing parameters(i think for all of these we can array both the arrays and the floats
//for now let us just try 1 smoothing parameter for everything.
float p_lock_smooth=.5;


//and then lets try an array of floats for storing the smoothed values
float p_lock_smoothed[p_lock_number];


//turn on and off writing to the array
bool p_lock_0_switch=1;

//global counter for all the locks
int p_lock_increment=0;


float my_normalize=0;

float low_value_smoothed=0.0;
float mid_value_smoothed=0.0;
float high_value_smoothed=0.0;
float smoothing_rate=.8;

ofFbo aspect_fix_fbo;


//--------------------------------------------------------------
void ofApp::setup() {
	//ofSetVerticalSync(true);

	

	ofSetFrameRate(30);
    
    ofBackground(0);
	
	//ofToggleFullscreen();
	
    ofHideCursor();
	
		
	if(scaleswitch==0){
		width=320;
		height=240;
	}
	
	if(scaleswitch==1){
		width=640;
		height=480;
	}
	  
	omx_settings();  
    
    
   
	framebuffer0.allocate(width,height);
	
	framebuffer0.begin();
	ofClear(0,0,0,255);
	framebuffer0.end();
	
	
	aspect_fix_fbo.allocate(width,height);
	
	aspect_fix_fbo.begin();
	ofClear(0,0,0,255);
	aspect_fix_fbo.end();
	
	
	dry_framebuffer.allocate(width,height);
	
	
	dry_framebuffer.begin();
	ofClear(0,0,0,255);
	dry_framebuffer.end();
	
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
	
	
	
	
	//p_lock biz
	for(int i=0;i<p_lock_number;i++){
        
        for(int j=0;j<p_lock_size;j++){
            
            p_lock[i][j]=0;
            
        }//endplocksize
    
    }//endplocknumber
    
     //pass in the settings and it will start the camera
	if(inputswitch==0){
		videoGrabber.setup(settings);
	}

	
	if(inputswitch==1){
		
		cam1.setDesiredFrameRate(30);
		
		if(hdmi_input_switch==1){
			cam1.initGrabber(1280,720);
		}
		
		if(hdmi_input_switch==0){
			cam1.initGrabber(640,480);
		}
		
	}
	
}

//--------------------------------------------------------------
void ofApp::update() {
	
	
	
	if(inputswitch==1){
		cam1.update();
	}
	
	if(inputswitch==0){
		
		omx_updates();
		
	}
	
	//corner crop and stretch to preserve hd aspect ratio
	if(hdmi_aspect_ratio_switch==1){
			aspect_fix_fbo.begin();
			cam1.draw(0,0,853,480);
			aspect_fix_fbo.end();
			
	}
	
	midibiz();
	
	for(int i=0;i<p_lock_number;i++){
        p_lock_smoothed[i]=p_lock[i][p_lock_increment]*(1.0f-p_lock_smooth)+p_lock_smoothed[i]*p_lock_smooth;
        
        if(abs(p_lock_smoothed[i])<.05){p_lock_smoothed[i]=0;}
        
        
    }
 
 

}

//--------------------------------------------------------------
void ofApp::draw() {
	
	
	
	
	//next-we have three sets of dummy variables fed from the midi controller.
	
	//control attenuation section
	
	//control coeficients
	
	//default values will be for wet mode
	float c_lumakey_value=1.01;
	float c_mix=2;
	float c_hue=.75;
	float c_sat=.5;
	float c_bright=.5;
	float c_fb1_mix=1.1;
	float c_cam1_x=2.0;
	float c_x=.01;
	float c_y=.01;
	float c_z=.05;
	float c_rotate=.314159265;
	float c_huex_off=.25;
	float c_huex_lfo=.25;
	
	if(wet_dry_switch==0){
		c_hue=1.0;
		c_sat=1.0;
		c_bright=1.0;
		c_x=.1;
		c_y=.1;
		c_z=.5;
	}
	
	//all the dummy variables will begin with d
	int d_delay=(abs(framedelayoffset-fbob-fb0_delayamount-
				int((p_lock_smoothed[15])*(fbob-1.0))
				)-1)%fbob;
	float d_lumakey_value=kk+c_lumakey_value*p_lock_smoothed[0];
							
	float d_mix=jm+c_mix*p_lock_smoothed[1];
	float d_hue=fv*(1.0f+c_hue*p_lock_smoothed[2]);
	float d_sat=gb*(1.0f+c_sat*p_lock_smoothed[3]);
	float d_bright=hn*(1.0f+c_bright*p_lock_smoothed[4]);
	float d_fb1_mix=op+c_fb1_mix*p_lock_smoothed[5];
	float d_fb1_x=fb1_brightx+p_lock_smoothed[6];
	float d_cam1_x=ll+c_cam1_x*p_lock_smoothed[7];
	float d_x=sx+c_x*p_lock_smoothed[8];
	float d_y=dc+c_y*p_lock_smoothed[9];
	float d_z=az*(1.0f+c_z*p_lock_smoothed[10]);
	float d_rotate=qw+c_rotate*p_lock_smoothed[11];
	float d_huex_mod=er*(1.0f-p_lock_smoothed[12]);
	float d_huex_off=ty+c_huex_off*p_lock_smoothed[13];
	float d_huex_lfo=ui+c_huex_lfo*p_lock_smoothed[14];
	
	

    
    
    //----------------------------------------------------------
    //
	
	framebuffer0.begin();
    shader_mixer.begin();

	//videoGrabber.getTextureReference().draw(0, 0, 320, 640);
	if(scaleswitch==0){
		
		if(inputswitch==0){
			videoGrabber.draw(0,0,320,240);
		}
		
		if(inputswitch==1){
			cam1.draw(0,0,320,240);
		}
	}
	
	if(scaleswitch==1){
		if(inputswitch==0){
			videoGrabber.draw(0,0);
		}
		
		if(inputswitch==1){
			if(hdmi_aspect_ratio_switch==0){
				cam1.draw(0,0,640,480);
			}
			
			if(hdmi_aspect_ratio_switch==1){
				aspect_fix_fbo.draw(0,0,640,480);
			}
			
		}
	}
	
	//textures
	shader_mixer.setUniformTexture("fb0", pastFrames[d_delay].getTexture(),1);
	
	if(wet_dry_switch==1){
		shader_mixer.setUniformTexture("fb1", pastFrames[(abs(framedelayoffset-fbob)-1)%fbob].getTexture(),2);
	}
	if(wet_dry_switch==0){
		shader_mixer.setUniformTexture("fb1", dry_framebuffer.getTexture(),2);
	}	
	//continuous variables
	shader_mixer.setUniform1f("fb0_lumakey_value",d_lumakey_value);
    shader_mixer.setUniform1f("fb0_mix",d_mix);
    shader_mixer.setUniform3f("fb0_hsbx",ofVec3f(d_hue,d_sat,d_bright));
    shader_mixer.setUniform1f("fb1_mix",d_fb1_mix);
    shader_mixer.setUniform1f("fb1_brightx",d_fb1_x );
    shader_mixer.setUniform1f("cam1_brightx",d_cam1_x);
    shader_mixer.setUniform1f("fb0_xdisplace",d_x);
    shader_mixer.setUniform1f("fb0_ydisplace",d_y);
    shader_mixer.setUniform1f("fb0_zdisplace",d_z);
    shader_mixer.setUniform1f("fb0_rotate",d_rotate);
    shader_mixer.setUniform3f("fb0_huex",ofVec3f(d_huex_mod,d_huex_off,d_huex_lfo));
    
    if(wet_dry_switch==1){
		shader_mixer.setUniform1i("toroid_switch",toroid_switch);
        shader_mixer.setUniform1i("mirror_switch",0);
	}
	
	 if(wet_dry_switch==0){
		shader_mixer.setUniform1i("toroid_switch",0);
        shader_mixer.setUniform1i("mirror_switch",toroid_switch);
	}
    
   
    shader_mixer.setUniform1i("fb0_b_invert",fb0_b_invert);
    shader_mixer.setUniform1i("fb0_h_invert",fb0_h_invert);
    shader_mixer.setUniform1i("fb0_s_invert",fb0_s_invert);
    
    shader_mixer.setUniform1i("fb0_h_mirror",fb0_h_mirror);
    shader_mixer.setUniform1i("fb0_v_mirror",fb0_v_mirror);
    
    //shader_mixer.setUniform1i("toroid_switch",toroid_switch);
    
    
    
    
    shader_mixer.setUniform1i("luma_switch",luma_switch);
    
    //shader_mixer.setUniform1i("x_mirror_switch",x_mirror_switch);
    
    //shader_mixer.setUniform1i("y_mirror_switch",y_mirror_switch);

    shader_mixer.end();
	framebuffer0.end();
	
	//_-_-__---_---___
	
	
	
	//_----___---------_-_-----_--_-_--_--_
	
	
	
	//framebuffer0.draw(0,0,ofGetScreenWidth(),ofGetScreenHeight());
	framebuffer0.draw(0,0,ofGetWidth(),ofGetHeight());
	

	//_-------------------------------------------
	
	
	pastFrames[abs(fbob-framedelayoffset)-1].begin(); //eeettt
    
    //ofPushMatrix();
	//ofTranslate(framebuffer0.getWidth()/2,framebuffer0.getHeight()/2);
    //ofRotateYRad(y_skew);
    //ofRotateXRad(x_skew);
    
    if(wet_dry_switch==0){
		
		
		
		
		if(inputswitch==0){
			videoGrabber.draw(0,0,framebuffer0.getWidth(),framebuffer0.getHeight());
		}
		
		if(inputswitch==1){
			if(hdmi_aspect_ratio_switch==0){
				cam1.draw(0,0,640,480);
			}
			
			if(hdmi_aspect_ratio_switch==1){
				aspect_fix_fbo.draw(0,0,640,480);
			}
		}
	
	dry_framebuffer.begin();
	framebuffer0.draw(0,0);
	dry_framebuffer.end();
	
	}//endifwetdry0
	
		
	if(wet_dry_switch==1){
		
		framebuffer0.draw(0,0);
		
		
		}//endifwetdry1
	//ofPopMatrix();
	
    pastFrames[abs(fbob-framedelayoffset)-1].end(); //eeettt

	incIndex();
   
	//p_lock biz
	 if(p_lock_switch==1){
        p_lock_increment++;
        p_lock_increment=p_lock_increment%p_lock_size;
    }
   
	
	
	
//i use this block of code to print out like useful information for debugging various things and/or just to keep the 
//framerate displayed to make sure i'm not losing any frames while testing out new features.  uncomment the ofDrawBitmap etc etc
//to print the stuff out on screen
   ofSetColor(255);
   string msg="fps="+ofToString(ofGetFrameRate(),2)+" clear switch"+ofToString(clear_switch,5);//+" z="+ofToString(az,5);
   //ofDrawBitmapString(msg,10,10);
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}


void ofApp::omx_settings(){
	
	settings.sensorWidth = 640;
    settings.sensorHeight = 480;
    settings.framerate = 30;
    settings.enableTexture = true;
    settings.sensorMode=7;
    
    settings.whiteBalance ="Off";
    settings.exposurePreset ="Off";
    settings.whiteBalanceGainR = 1.0;
    settings.whiteBalanceGainB = 1.0;
	
	}
//------------------------------------------------------------

void ofApp::omx_updates(){
	
		videoGrabber.setSharpness(50);
		videoGrabber.setBrightness(40);
		videoGrabber.setContrast(100);
		videoGrabber.setSaturation(0);
	
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
    
    if (key == ';') {fb1_brightx += .01;}
    if (key == '\'') {fb1_brightx -= .01;}
    
    //fb0 rotation
    if (key == 'q') {qw += .0001;}
    if (key == 'w') {qw -= .0001;}


	//hue chaos1
    if (key == 'e') {er += .001;}
    if (key == 'r') {er -= .001;}
    
    //hue chaos2
    if (key == 't') {ty += .01;}
    if (key == 'y') {ty -= .01;}
    
    //hue chaos3
    if (key == 'u') {ui += .01;}
    if (key == 'i') {ui -= .01;}

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
    
    
    if(key=='2'){fb0_b_invert=!fb0_b_invert;}
    
    if(key=='3'){hdmi_aspect_ratio_switch=!hdmi_aspect_ratio_switch;}
    
    
    if(key=='4'){fb0_s_invert=!fb0_s_invert;}
    
    if(key=='5'){fb0_v_mirror=!fb0_v_mirror;}
    if(key=='6'){fb0_h_mirror=!fb0_h_mirror;}
    
    if(key=='7'){toroid_switch=!toroid_switch;}
    
    if (key == '-') {y_skew += .01;}
    if (key == '=') {y_skew -= .01;}
    if (key == '9') {x_skew += .01;}
    if (key == '0') {x_skew -= .01;}
    
	//reset button
    if (key == '!') {
		
		
	az = 1.0;
	sx = 0;
    dc = 0;
	fv = 1;
	gb = 1;
	hn = 1;
	jm = 0.0;
	kk = 0.0;
	ll = 0.0;
	qw = 0.0;

	er = 1.0;
	ty = 0.0;
	ui = 0.0;

	op = 0.0;
	fb0_delayamount=0;
	
	fb0_b_invert=0;
	fb0_h_invert=0;
	fb0_s_invert=0;
	
	fb0_v_mirror=0;
	fb0_h_mirror=0;
	
	x_skew=0;
	y_skew=0;
	
	framebuffer0.begin();
    ofClear(0, 0, 0, 255);
    framebuffer0.end();
	 
	for(int i=0;i<fbob;i++){
        
       
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
        
    
		}//endifor
		
	}
    
    //no
   // if(key=='='){inputswitch = !inputswitch;}
}

void ofApp::midibiz(){
	
    //lets figure out the hd switch thing here
	//bool cc_aspect_switch=0;
    //int cc_aspect_int=0;
                
	for(unsigned int i = 0; i < midiMessages.size(); ++i) {

		ofxMidiMessage &message = midiMessages[i];
	
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
                
		
                
                
                
                
                
                
                if(message.control==39){
                    if(message.value==127){
                        p_lock_0_switch=0;
                       // p_lock_increment=0;
                    }
                    
                    if(message.value==0){
                        p_lock_0_switch=1;
                    }

                }
                
                
                if(message.control==55){
                    if(message.value==127){
                        p_lock_switch=1;
                        // p_lock_increment=0;
                        
                        for(int i=0;i<p_lock_number;i++){
								p_lock_smoothed[i]=0;
								for(int j=0;j<p_lock_size;j++){
                                
									p_lock[i][j]=p_lock[i][p_lock_increment];
                                
								}//endplocksize
                            
							}//endplocknumber
                    }
                    
                    if(message.value==0){
                        p_lock_switch=0;
                    }
                    
                }
                
                /*
                if(message.control==58){
                    if(message.value==127){
                        for(int i=0;i<p_lock_number;i++){
                            
                            for(int j=0;j<p_lock_size;j++){
                                
                                p_lock[i][j]=0;
                                
                            }//endplocksize
                            
                        }//endplocknumber
                    }
                  
                    
                }
                */
                
                if(message.control==32){
					if(message.value==127){
						x_2=TRUE;
					}
					
					if(message.value==0){
						x_2=FALSE;
					}
                }
                
                if(message.control==48){
					if(message.value==127){
						x_5=TRUE;
					}
					
					if(message.value==0){
						x_5=FALSE;
					}
                }
                
                 if(message.control==64){
					if(message.value==127){
						x_10=TRUE;
					}
					
					if(message.value==0){
						x_10=FALSE;
					}
                }
                
                
                
                if(message.control==33){
					if(message.value==127){
						y_2=TRUE;
					}
					
					if(message.value==0){
						y_2=FALSE;
					}
                }
                
                if(message.control==49){
					if(message.value==127){
						y_5=TRUE;
					}
					
					if(message.value==0){
						y_5=FALSE;
					}
                }
                
                 if(message.control==65){
					if(message.value==127){
						y_10=TRUE;
					}
					
					if(message.value==0){
						y_10=FALSE;
					}
                }
                
                
                
                if(message.control==34){
					if(message.value==127){
						z_2=TRUE;
					}
					
					if(message.value==0){
						z_2=FALSE;
					}
                }
                
                if(message.control==50){
					if(message.value==127){
						z_5=TRUE;
					}
					
					if(message.value==0){
						z_5=FALSE;
					}
                }
                
                 if(message.control==66){
					if(message.value==127){
						z_10=TRUE;
					}
					
					if(message.value==0){
						z_10=FALSE;
					}
                }
                
                
                
                
                if(message.control==35){
					if(message.value==127){
						theta_0=TRUE;
					}
					
					if(message.value==0){
						theta_0=FALSE;
					}
                }
                
                if(message.control==51){
					if(message.value==127){
						theta_1=TRUE;
					}
					
					if(message.value==0){
						theta_1=FALSE;
					}
                }
                
                 if(message.control==67){
					if(message.value==127){
						theta_2=TRUE;
					}
					
					if(message.value==0){
						theta_2=FALSE;
					}
                }
                
                
                
                
                if(message.control==36){
					if(message.value==127){
						huexx_0=TRUE;
					}
					
					if(message.value==0){
						huexx_0=FALSE;
					}
                }
                
                if(message.control==52){
					if(message.value==127){
						huexx_1=TRUE;
					}
					
					if(message.value==0){
						huexx_1=FALSE;
					}
                }
                
                 if(message.control==68){
					if(message.value==127){
						huexx_2=TRUE;
					}
					
					if(message.value==0){
						huexx_2=FALSE;
					}
                }
                
                if(message.control==46){
					if(message.value==127){
						toroid_switch=TRUE;
					}
					
					if(message.value==0){
						toroid_switch=FALSE;
					}
                }
                
                
                
             
                if(y_skew_switch==TRUE){
					y_skew+=.00001;
                }
                
                if(x_skew_switch==TRUE){
					x_skew+=.00001;
                }
                
                
                if(message.control==71){
					if(message.value==127){
						wet_dry_switch=FALSE;
					}
					
					if(message.value==0){
						wet_dry_switch=TRUE;
					}
					
                }
                
                //---------------------
                
                
                
				if(message.control==37){
					if(message.value==127){
						huexy_0=TRUE;
					}
					
					if(message.value==0){
						huexy_0=FALSE;
					}
                }
                
                if(message.control==53){
					if(message.value==127){
						huexy_1=TRUE;
					}
					
					if(message.value==0){
						huexy_1=FALSE;
					}
                }
                
                 if(message.control==69){
					if(message.value==127){
						huexy_2=TRUE;
					}
					
					if(message.value==0){
						huexy_2=FALSE;
					}
                }
                
                
                //---------------------
                
                
                
                if(message.control==38){
					if(message.value==127){
						huexz_0=TRUE;
					}
					
					if(message.value==0){
						huexz_0=FALSE;
					}
                }
                
                if(message.control==54){
					if(message.value==127){
						huexz_1=TRUE;
					}
					
					if(message.value==0){
						huexz_1=FALSE;
					}
                }
                
                 if(message.control==70){
					if(message.value==127){
						huexz_2=TRUE;
					}
					
					if(message.value==0){
						huexz_2=FALSE;
					}
                }
                
                
                //
                
                
                /*
                if(message.control==62){
					if(message.value==127){
						luma_switch=TRUE;
					}
					
					if(message.value==0){
						luma_switch=FALSE;
					}
					
                }
                */
                
                /*
                if(message.control==61){
					if(message.value==127){
						x_mirror_switch=TRUE;
					}
					
					if(message.value==0){
						x_mirror_switch=FALSE;
					}
					
                }
                */
                
                 if(message.control==60){
					if(message.value==127){
						luma_switch=TRUE;
					}
					
					if(message.value==0){
						luma_switch=FALSE;
					}
					
                }
                
                
                if(message.control==43){
					if(message.value==127){
						control_switch=1;
					}
					
					if(message.value==0){
						control_switch=0;
					}
					
                }
                
                if(message.control==44){
					if(message.value==127){
						control_switch=2;
					}
					
					if(message.value==0){
						control_switch=0;
					}
					
                }
                
                 if(message.control==42){
					if(message.value==127){
						control_switch=3;
					}
					
					if(message.value==0){
						control_switch=0;
					}
					
                }
                /*
                
                if(message.control==62){
					if(message.value==127){
						control_switch=1;
					}
					
					if(message.value==0){
						control_switch=0;
					}
					
                }
                
                if(message.control==61){
					if(message.value==127){
						control_switch=2;
					}
					
					if(message.value==0){
						control_switch=0;
					}
					
                }
                
                 if(message.control==60){
					if(message.value==127){
						control_switch=3;
					}
					
					if(message.value==0){
						control_switch=0;
					}
					
                }
                */
                
                //nanokontrol2 controls
                 //c1 maps to fb0 lumakey
                if(message.control==16){
                   
					if(control_switch==0){	
						if(p_lock_0_switch==1){
							p_lock[0][p_lock_increment]=message.value/127.0f;
						}              
                    }
                    
                    
                }
                
               if(message.control==17){
                   
                   if(control_switch==0){
						if(p_lock_0_switch==1){
							p_lock[1][p_lock_increment]=(message.value-63)/63.0f;
						}
                    }
                    
                  
                }
                
                //c3 maps to fb0 huex
                if(message.control==18){
                    if(control_switch==0){
						if(p_lock_0_switch==1){
							p_lock[2][p_lock_increment]=(message.value-63)/63.0f;
						}
                    }
                
                    
                }
                
                //c4 maps to fb0 satx
                if(message.control==19){
                  if(control_switch==0){
						if(p_lock_0_switch==1){
							p_lock[3][p_lock_increment]=(message.value-63)/63.0f;
						}
                    }
                   
                }
                
                //c5 maps to fb0 brightx
                if(message.control==20){
                    if(control_switch==0){
						if(p_lock_0_switch==1){
							p_lock[4][p_lock_increment]=(message.value-63)/63.0f;
						}
                    }
					
					
                }
                
                //c6 maps to temporal filter
                if(message.control==21){
                   if(control_switch==0){
						if(p_lock_0_switch==1){
							p_lock[5][p_lock_increment]=(message.value-63)/63.0f;
						}
                    }
                 
                }
                
                //c7 maps to temporal filter resonance
                if(message.control==22){
                    if(control_switch==0){
						if(p_lock_0_switch==1){
							p_lock[6][p_lock_increment]=(message.value)/127.0f;
						}
                    }
                 
                }
                
                //c8 maps to brightx
                if(message.control==23){
                     if(control_switch==0){
						if(p_lock_0_switch==1){
							p_lock[7][p_lock_increment]=(message.value)/127.0f;
						}
                    }
                  
                   
                }
                
                //c9 maps to fb0 x displace
                if(message.control==120){
					if(control_switch==0){
						p_lock[8][p_lock_increment]=(message.value-63)/63.0f;
                     
						if(x_2==TRUE){
							p_lock[8][p_lock_increment]=2.0*(message.value-63.0)/63.0f;
							}
						 
						if(x_5==TRUE){
							p_lock[8][p_lock_increment]=5.0*(message.value-63.0)/63.0f;
							}
						if(x_10==TRUE){
							p_lock[8][p_lock_increment]=10.0*(message.value-63.0)/63.0f;
							}	 	 
                    }
                    
                    
                    
                    
                    
                    
                   
                }
                
                 //c10 maps to fb0 y displace
                if(message.control==121){
					if(control_switch==0){
						p_lock[9][p_lock_increment]=(message.value-63)/63.0f;
                     
						if(y_2==TRUE){
							p_lock[9][p_lock_increment]=2.0*(message.value-63.0)/63.0f;
						 }
						 
						if(y_5==TRUE){
							p_lock[9][p_lock_increment]=5.0*(message.value-63.0)/63.0f;
						 }
						if(y_10==TRUE){
							p_lock[9][p_lock_increment]=10.0*(message.value-63.0)/63.0f;
						 }	 	 
                     }
                     
                     
                     
                     
                     
                     
                    
                   
                }
                
               
                if(message.control==122){
					if(control_switch==0){
						p_lock[10][p_lock_increment]=(message.value-63.0)/63.0f;
                     
						if(z_2==TRUE){
							p_lock[10][p_lock_increment]=2.0*(message.value-63.0)/63.0f;
						 }
						 
						if(z_5==TRUE){
							p_lock[10][p_lock_increment]=5.0*(message.value-63.0)/63.0f;
						 }
						if(z_10==TRUE){
							p_lock[10][p_lock_increment]=10.0*(message.value-63.0)/63.0f;
						 }	 	 
                   }
                   
                   
                   
                   
                   
                   
                   
                }
              
                if(message.control==123){
					if(control_switch==0){
						p_lock[11][p_lock_increment]=(message.value-63)/63.0f;
                     
						if(theta_0==TRUE){
							p_lock[11][p_lock_increment]=2*(message.value-63.0)/63.0f;
						 }
						 
						if(theta_1==TRUE){
							p_lock[11][p_lock_increment]=4*(message.value-63.0)/63.0f;
						 }
						if(theta_2==TRUE){
							p_lock[11][p_lock_increment]=8*(message.value-63.0)/63.0f;
						 }	 	
					}   
					
					  
					
					
					
					 
                  
                   
                }
                
             
                if(message.control==124){
                    if(control_switch==0){
						p_lock[12][p_lock_increment]=(message.value)/32.0f;
                    
						if(huexx_0==TRUE){
							p_lock[12][p_lock_increment]=message.value/64.0f;
						 }
						 
						if(huexx_1==TRUE){
							p_lock[12][p_lock_increment]=message.value/96.0f;
						 }
						if(huexx_2==TRUE){
							p_lock[12][p_lock_increment]=message.value/127.0f;
						 }	 
					}
					
					
					
					
					
					
                }
              
            
                if(message.control==125){
					if(control_switch==0){
						p_lock[13][p_lock_increment]=(message.value-63.0)/63.0f;
                     
						if(huexy_0==TRUE){
							p_lock[13][p_lock_increment]=2*(message.value-63.0)/63.0f;
						 }
						 
						if(huexy_1==TRUE){
							p_lock[13][p_lock_increment]=4*(message.value-63.0)/63.0f;
						 }
						if(huexy_2==TRUE){
							p_lock[13][p_lock_increment]=8*(message.value-63.0)/63.0f;
						 }	 
                     }
                     
                     
                     
                     
                     
                     
                     
                    
                   
                }
                
               
                if(message.control==126){
					if(control_switch==0){
						p_lock[14][p_lock_increment]=(message.value-63.0)/63.0f;
                     
						if(huexz_0==TRUE){
							p_lock[14][p_lock_increment]=2*(message.value-63.0)/63.0f;
						}
						 
						if(huexz_1==TRUE){
							p_lock[14][p_lock_increment]=4*(message.value-63.0)/63.0f;
						}
						if(huexz_2==TRUE){
							p_lock[14][p_lock_increment]=8*(message.value-63.0)/63.0f;
						}	 
                     }
                     
                     
                     
                      
                     
                     
                     
                    
                   
                }
                
                
                if(message.control==127){
                   if(control_switch==0){
						p_lock[15][p_lock_increment]=(message.value)/127.0;
                   }
                   
                   
                }
                
                
                
                
                //gots to remap these to 60-61
                 //cc43 maps to fb0_b_invert
                if(message.control==43){
					if(message.value==127){
						fb0_b_invert=TRUE;
					}
					
					if(message.value==0){
						fb0_b_invert=FALSE;
					}
                }
                
                //cc44 maps to fb0_s_invert
                if(message.control==44){
					if(message.value==127){
						fb0_s_invert=TRUE;
					}
					
					if(message.value==0){
						fb0_s_invert=FALSE;
					}
                }
                
                 if(message.control==62){
					if(message.value==127){
						//fb0_h_invert=TRUE;
						hdmi_aspect_ratio_switch=TRUE;
					}
					
					if(message.value==0){
						//fb0_h_invert=FALSE;
						hdmi_aspect_ratio_switch=FALSE;
					}
                }
                
                //cc42 maps to fb0_s_invert
                if(message.control==42){
					if(message.value==127){
						fb0_h_invert=TRUE;
						
					}
					
					if(message.value==0){
						fb0_h_invert=FALSE;
						
					}
                }
                
                 
                //cc41 maps to fb0_h_mirror
                if(message.control==41){
					if(message.value==127){
						fb0_h_mirror=TRUE;
					}
					
					if(message.value==0){
						fb0_h_mirror=FALSE;
					}
                }
                
                 //cc45 maps to fb0_h_mirror
                if(message.control==45){
					if(message.value==127){
						fb0_v_mirror=TRUE;
					}
					
					if(message.value==0){
						fb0_v_mirror=FALSE;
					}
                }
                
                
                
                
                
                
                //this needs to be tested out and reworked
                //cc45 maps to fb0 clear
                //still doesn't work arggg
                
                
                
                 if(message.control==59){
						
					if(message.value==127){
                        for(int i=0;i<p_lock_number;i++){
                            
                            for(int j=0;j<p_lock_size;j++){
                                
                                p_lock[i][j]=0;
                                
                            }//endplocksize
                            
                        }//endplocknumber
                    }
                }
                 
                
               if(message.control==58){
					if(message.value==127){
						if(clear_switch==0){
							clear_switch=1;
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
					
                }
                
                if(message.control!=58){
					clear_switch=0;
                }
                
              
            }
            else if(message.status == MIDI_PITCH_BEND) {
                //text << "\tval: " << message.value;
			
			}
			else {
				//text << "\tpitch: " << message.pitch;
				
                
              
               

				
			}
			
		}//

	
	}
	
	
	//midiMessages.clear();
	
	//end midi biz
	
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
