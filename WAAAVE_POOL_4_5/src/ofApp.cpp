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

#define MIDI_MAGIC 63.50f

#define CONTROL_THRESHOLD .04f

//flip this switch to try different scalings
//0 is 320 1 is 640
//if you reduce scale to 320 you can up the total delay time to
//about 4 seconds/ 120 frames
//so try that out sometime and see how that feels!
bool scaleswitch=1;
const int framebufferLength=60;
//const int framebufferLength=120;


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

float testt=1.0;

bool y_skew_switch=FALSE;
bool x_skew_switch=FALSE;



float y_skew=0;

float x_skew=0;



int fb_delayamount=0;



//dummy variables for midi control



int width=640;
int height=480;


//dummy variables for midi to audio attenuatiors
//0 is direct midi, 1 is low_x, 2 is mid_x, 3 is high_x
int control_switch=0;
//for audio reactive controls
/*
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

float my_normalize=0;

float low_value_smoothed=0.0;
float mid_value_smoothed=0.0;
float high_value_smoothed=0.0;
float smoothing_rate=.8;

*/ 

bool clear_switch=0;

//toggle switch variables
bool hueInvert=FALSE;
bool saturationInvert=FALSE;
bool brightInvert=FALSE;
bool cam1_h_invert=FALSE;
bool cam1_s_invert=FALSE;
bool cam1_b_invert=FALSE;
bool horizontalMirror=FALSE;
bool verticalMirror=FALSE;
bool lumakeyInvertSwitch=FALSE;
bool toroidSwitch=0;

float temporalFilterResonance=0.0;

//rescaling switches
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

bool mirrorSwitch;
//framebuffer management biziness

//framebufferLength is the total number of frames that will be stored.  setting framebufferLength to
//30 while fps is set to 30 means 1 second of video will be stored in memory  
//75 seems to work ok with usb cams but the capture device might need a little more memory

int framedelayoffset=0;  // this is used as an index to the circular framebuffers eeettt
unsigned int framecount=0; // framecounter used to calc offset eeettt

//this is an array of framebuffer objects
ofFbo pastFrames[framebufferLength];

void incIndex()  // call this every frame to calc the offset eeettt
{
    
    framecount++;
    framedelayoffset=framecount % framebufferLength;
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



ofFbo aspect_fix_fbo;


//videoreactive controls
bool videoReactiveSwitch=FALSE;

float vLumakeyValue=0.0;
float vMix=0.0;
float vHue=0.0;
float vSaturation=0.0;
float vBright=0.0;
float vTemporalFilterMix=0.0;
float vTemporalFilterResonance=0.0;
float vSharpenAmount=0.0;
float vX=0.0;
float vY=0.0;
float vZ=0.0;
float vRotate=0.0;
float vHuexMod=0.0;
float vHuexOff=0.0;
float vHuexLfo=0.0;


//lfos
bool globalLfoAmpSwitch=false;
bool globalLfoRateSwitch=false;

//0 means plocks active
//1 means videoreactive active
int savedState=0;

bool xLfoAmpSwitch=false;
bool xLfoRateSwitch=false;
float xLfoAmp=0.0;
float xLfoRate=0.0;
int xLfoShape=0;
	

bool yLfoAmpSwitch=false;
bool yLfoRateSwitch=false;
float yLfoAmp=0.0;
float yLfoRate=0.0;
int yLfoShape=0;

bool zLfoAmpSwitch=false;
bool zLfoRateSwitch=false;
float zLfoAmp=0.0;
float zLfoRate=0.0;
int zLfoShape=0;

bool rotateLfoAmpSwitch=false;
bool rotateLfoRateSwitch=false;
float rotateLfoAmp=0.0;
float rotateLfoRate=0.0;
int rotateLfoShape=0;

//latching
bool midiActiveFloat[17];
bool vmidiActiveFloat[17];
bool lfoAmpActiveFloat[17];
bool lfoRateActiveFloat[17];
//--------------------------------------------------------------
void ofApp::setup() {
	//ofSetVerticalSync(true);
	ofSetFrameRate(30);
    ofBackground(0);
	//ofToggleFullscreen();
	ofHideCursor();
	cameraSetup(width, height);	
	//omx_settings();  
    allocateFbo(width,height);
    clearFbo();
	p_lockClear();
	shader_mixer.load("shadersES2/shader_mixer");
	shaderSharpen.load("shadersES2/shaderSharpen");
	midiSetup();
	
	for(int i=0;i<17;i++){
		vmidiActiveFloat[i]=0;
		midiActiveFloat[i]=0;
		lfoAmpActiveFloat[i]=0;
		lfoRateActiveFloat[i]=0;
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	cameraUpdate();
	midibiz();
	p_lockUpdate();
}

//--------------------------------------------------------------
void ofApp::draw() {

	/**controlZones**/
	
	//default values will be for wet mode
	float c_lumakey_value=1.01;
	float c_mix=2;
	float c_hue=.25;
	float c_sat=.2;
	float c_bright=.35;
	float c_temporalFilterMix=1.1;
	float c_sharpenAmount=.5;
	float c_x=.01;
	float c_y=.01;
	float c_z=.05;
	float c_rotate=.314159265;
	float c_huex_off=.25;
	float c_huex_lfo=.25;
	
	//change a bunch of coefficients when switching to dry mode
	if(wet_dry_switch==0){
		c_hue=1.0;
		c_sat=1.0;
		c_bright=1.0;
		c_x=.1;
		c_y=.1;
		c_z=.5;
	}
	
	//all the dummy variables will begin with d
	int d_delay=(abs(framedelayoffset-framebufferLength-fb_delayamount-
				int((p_lock_smoothed[15])*(framebufferLength-1.0))
				)-1)%framebufferLength;
	float d_lumakey_value=kk+c_lumakey_value*p_lock_smoothed[0];							
	float d_mix=jm+c_mix*p_lock_smoothed[1];
	float d_hue=fv*(1.0f+c_hue*p_lock_smoothed[2]);
	float d_sat=gb*(1.0f+c_sat*p_lock_smoothed[3]);
	float d_bright=hn*(1.0f+c_bright*p_lock_smoothed[4]);
	float d_temporalFilterMix=op+c_temporalFilterMix*p_lock_smoothed[5];
	float d_fb1_x=temporalFilterResonance+p_lock_smoothed[6];
	float d_sharpenAmount=ll+c_sharpenAmount*p_lock_smoothed[7];
	float d_x=sx+c_x*p_lock_smoothed[8];
	float d_y=dc+c_y*p_lock_smoothed[9];
	float d_z=az*(1.0f+c_z*p_lock_smoothed[10]);
	float d_rotate=qw+c_rotate*p_lock_smoothed[11];
	float d_huex_mod=er*(1.0f-p_lock_smoothed[12]);
	float d_huex_off=ty+c_huex_off*p_lock_smoothed[13];
	float d_huex_lfo=ui+c_huex_lfo*p_lock_smoothed[14];
    
    /**lfo zones**/
    
    d_x+=c_x*xLfoAmp*sin(ofGetElapsedTimef()*xLfoRate);
    d_y+=c_y*yLfoAmp*sin(ofGetElapsedTimef()*yLfoRate);
    d_z+=c_z*zLfoAmp*sin(ofGetElapsedTimef()*zLfoRate);
    d_rotate+=c_rotate*rotateLfoAmp*sin(ofGetElapsedTimef()*rotateLfoRate);
    /**shader zones**/
	
	framebuffer0.begin();
    shader_mixer.begin();

	//videoGrabber.getTextureReference().draw(0, 0, 320, 640);
	if(scaleswitch==0){
		if(inputswitch==0){
			//videoGrabber.draw(0,0,320,240);
		}
		if(inputswitch==1){
			cam1.draw(0,0,320,240);
		}
	}
	if(scaleswitch==1){
		if(inputswitch==0){
			//videoGrabber.draw(0,0);
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
	
	//send the textures
	shader_mixer.setUniformTexture("fb", pastFrames[d_delay].getTexture(),1);
	
	if(wet_dry_switch==1){
		shader_mixer.setUniformTexture("temporalFilter", pastFrames[(abs(framedelayoffset-framebufferLength)-1)%framebufferLength].getTexture(),2);
	}
	if(wet_dry_switch==0){
		shader_mixer.setUniformTexture("temporalFilter", dry_framebuffer.getTexture(),2);
	}	
	//send the continuous variables
	shader_mixer.setUniform1f("lumakey",d_lumakey_value);
    shader_mixer.setUniform1f("fbMix",d_mix); 
    shader_mixer.setUniform1f("fbHue",d_hue);
    shader_mixer.setUniform1f("fbSaturation",d_sat);
    shader_mixer.setUniform1f("fbBright",d_bright);
    shader_mixer.setUniform1f("temporalFilterMix",d_temporalFilterMix);
    shader_mixer.setUniform1f("temporalFilterResonance",d_fb1_x );
    shader_mixer.setUniform1f("fbXDisplace",d_x);
    shader_mixer.setUniform1f("fbYDisplace",d_y);
    shader_mixer.setUniform1f("fbZDisplace",d_z);
    shader_mixer.setUniform1f("fbRotate",d_rotate);
    shader_mixer.setUniform3f("fb_huex",ofVec3f(d_huex_mod,d_huex_off,d_huex_lfo));
    shader_mixer.setUniform1f("fbHuexMod",d_huex_mod);
    shader_mixer.setUniform1f("fbHuexOff",d_huex_off);
	shader_mixer.setUniform1f("fbHuexLfo",d_huex_lfo);	
   
	//send the switches
    shader_mixer.setUniform1i("toroidSwitch",toroidSwitch);
    shader_mixer.setUniform1i("mirrorSwitch",mirrorSwitch);
    shader_mixer.setUniform1i("brightInvert",brightInvert);
    shader_mixer.setUniform1i("hueInvert",hueInvert);
    shader_mixer.setUniform1i("saturationInvert",saturationInvert);
    shader_mixer.setUniform1i("horizontalMirror",horizontalMirror);
    shader_mixer.setUniform1i("verticalMirror",verticalMirror);
    shader_mixer.setUniform1i("lumakeyInvertSwitch",lumakeyInvertSwitch);
    
    //send the videoreactive controls
    shader_mixer.setUniform1f("vLumakey",vLumakeyValue*c_lumakey_value);
    shader_mixer.setUniform1f("vMix",vMix*c_mix);
    shader_mixer.setUniform1f("vHue",2.0f*vHue*c_hue);
    shader_mixer.setUniform1f("vSat",2.0f*vSaturation*c_sat);
    shader_mixer.setUniform1f("vBright",2.0f*vBright*c_bright);
    shader_mixer.setUniform1f("vtemporalFilterMix",vTemporalFilterMix*c_temporalFilterMix);
    shader_mixer.setUniform1f("vFb1X",vTemporalFilterResonance);
    shader_mixer.setUniform1f("vX",.025*vX);
    shader_mixer.setUniform1f("vY",.025*vY);
    shader_mixer.setUniform1f("vZ",1.25*vZ*c_z);
    shader_mixer.setUniform1f("vRotate",vRotate*c_rotate);
    shader_mixer.setUniform1f("vHuexMod",1.0f-vHuexMod);
    shader_mixer.setUniform1f("vHuexOff",vHuexOff*c_huex_off);
    shader_mixer.setUniform1f("vHuexLfo",vHuexLfo*c_huex_lfo);
    shader_mixer.end();
	framebuffer0.end();
	
	/**sharpening mode**/	
	sharpenFramebuffer.begin();
	shaderSharpen.begin();
	framebuffer0.draw(0,0);
	shaderSharpen.setUniform1f("sharpenAmount",d_sharpenAmount);
	shaderSharpen.setUniform1f("vSharpenAmount",vSharpenAmount*c_sharpenAmount);
	shaderSharpen.end();
	sharpenFramebuffer.end();

	//instead of doing another framebuffer swap we just work with sharpenFramebuffer as the end of the chain
	sharpenFramebuffer.draw(0,0,ofGetWidth(),ofGetHeight());
	
	/**heres where we manage ping ponging the buffers as well as maintining the circular buffer**/	
	pastFrames[abs(framebufferLength-framedelayoffset)-1].begin(); 
	//if wetdry ==0 then we are in feedforward mode so we feed the buffer from the input source instead
	//of from the output source
    if(wet_dry_switch==0){
		if(inputswitch==0){
			//videoGrabber.draw(0,0,framebuffer0.getWidth(),framebuffer0.getHeight());
		}
		if(inputswitch==1){
			if(hdmi_aspect_ratio_switch==0){
				cam1.draw(0,0,640,480);
			}	
			if(hdmi_aspect_ratio_switch==1){
				aspect_fix_fbo.draw(0,0,640,480);
			}
		}
	//on the other hand we still want to make sure we can smooth out things
	//with the temporal filter so that still comes from the last draw
	dry_framebuffer.begin();
	sharpenFramebuffer.draw(0,0);
	dry_framebuffer.end();
	}
	//wetdry==1 means feedback mode
	if(wet_dry_switch==1){
		sharpenFramebuffer.draw(0,0);
		}
    pastFrames[abs(framebufferLength-framedelayoffset)-1].end();
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
	string msg="fps="+ofToString(ofGetFrameRate(),2);//+" z="+ofToString(az,5);
	//ofDrawBitmapString(msg,15,15);
}

//--------------------------------------------------------------
void ofApp::exit() {
	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//-------------------------------------------------------------
void ofApp::p_lockUpdate(){
	for(int i=0;i<p_lock_number;i++){
        p_lock_smoothed[i]=p_lock[i][p_lock_increment]*(1.0f-p_lock_smooth)+p_lock_smoothed[i]*p_lock_smooth;
        if(abs(p_lock_smoothed[i])<.01){p_lock_smoothed[i]=0;}
    }
}

//------------------------------------------------------
void ofApp::p_lockClear(){
	for(int i=0;i<p_lock_number;i++){
        for(int j=0;j<p_lock_size;j++){
            p_lock[i][j]=0; 
        }//endplocksize
    }//endplocknumber
}

//----------------------------------------------------
void ofApp::cameraSetup(int w, int h){
	//width=640;
	//height=480;	
	if(scaleswitch==0){
		width=320;
		height=240;
	}
	/*
	if(scaleswitch==1){
		width=640;
		height=480;
	}
	* */
	
	//OMX_IFDEF  
	//omx_settings();  
	
	 //pass in the settings and it will start the camera
	if(inputswitch==0){
		//videoGrabber.setup(settings);
	}
	if(inputswitch==1){	
		cam1.setDesiredFrameRate(30);
		cam1.initGrabber(w,h);
	}
}

//------------------------------------------------------------
void ofApp::cameraUpdate(){
	if(inputswitch==1){
		cam1.update();
	}
	if(inputswitch==0){
		//omx_updates();	
	}
	//corner crop and stretch to preserve hd aspect ratio
	if(hdmi_aspect_ratio_switch==1){
			aspect_fix_fbo.begin();
			cam1.draw(0,0,853,480);
			aspect_fix_fbo.end();		
	}
}

//----------------------------------------------------------
//declare and allocate fbo
void ofApp::allocateFbo(int w, int h){
	
	framebuffer0.allocate(w,h);
	framebuffer0.begin();
	ofClear(0,0,0,255);
	framebuffer0.end();
	
	aspect_fix_fbo.allocate(w,h);
	aspect_fix_fbo.begin();
	ofClear(0,0,0,255);
	aspect_fix_fbo.end();
	
	dry_framebuffer.allocate(w,h);
	dry_framebuffer.begin();
	ofClear(0,0,0,255);
	dry_framebuffer.end();
	
	sharpenFramebuffer.allocate(w,h);
	sharpenFramebuffer.begin();
	ofClear(0,0,0,255);
	sharpenFramebuffer.end();
	
	for(int i=0;i<framebufferLength;i++){
        pastFrames[i].allocate(w,h);
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
	}
}

//----------------------------------------------------------
//clear all
void ofApp::clearFbo(){
	
	framebuffer0.begin();
	ofClear(0,0,0,255);
	framebuffer0.end();
	
	aspect_fix_fbo.begin();
	ofClear(0,0,0,255);
	aspect_fix_fbo.end();
	
	dry_framebuffer.begin();
	ofClear(0,0,0,255);
	dry_framebuffer.end();
	
	for(int i=0;i<framebufferLength;i++){
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
	}
}
//---------------------------------------------------------
/*

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
	
*/	
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

	if (key == '[') {fb_delayamount += 1;}
    if (key == ']') {
		fb_delayamount = fb_delayamount-1;
		if(fb_delayamount<0){
			fb_delayamount=framebufferLength-fb_delayamount;
		}
	}    
    //fb1 mix
    if (key == 'o') {op += .01;}
    if (key == 'p') {op -= .01;}    
    //fb z displace
    if (key == 'a') {az += .0001;}
    if (key == 'z') {az -= .0001;}    
    //fb x displace
    if (key == 's') {sx += .0001;}
    if (key == 'x') {sx -= .0001;}    
    //fb y displace
    if (key == 'd') {dc += .0001;}
    if (key == 'c') {dc -= .0001;}    
    //fb hue attenuate
    if (key == 'f') {fv += .001;}
    if (key == 'v') {fv -= .001;}    
    //fb saturation attenuate
    if (key == 'g') {gb += .001;}
    if (key == 'b') {gb -= .001;}    
    //fb brightness attenuate
    if (key == 'h') {hn += .001;}
    if (key == 'n') {hn -= .001;}
    //fb mix
    if (key == 'j') {jm += .01;}
    if (key == 'm') {jm -= .01;}
    //fb lumakey value
    if (key == 'k') {kk = ofClamp(kk+.01,0.0,1.0);}
    if (key == ',') {kk = ofClamp(kk-.01,0.0,1.0);}        
    if (key == 'l') {ll += .01;}
    if (key == '.') {ll -= .01;}    
    if (key == ';') {temporalFilterResonance += .01;}
    if (key == '\'') {temporalFilterResonance -= .01;}    
    //fb rotation
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
	/*
    if (key == '1') {

        //clear the framebuffer if thats whats up
        framebuffer0.begin();
        ofClear(0, 0, 0, 255);
        framebuffer0.end();
        for(int i=0;i<framebufferLength;i++){
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
		}//endifor
    }

    if(key=='2'){brightInvert=!brightInvert;}   
    if(key=='3'){hdmi_aspect_ratio_switch=!hdmi_aspect_ratio_switch;}
    if(key=='4'){saturationInvert=!saturationInvert;}
    if(key=='5'){verticalMirror=!verticalMirror;}
    if(key=='6'){horizontalMirror=!horizontalMirror;}
    if(key=='7'){toroidSwitch=!toroidSwitch;}
    if (key == '-') {y_skew += .01;}
    if (key == '=') {y_skew -= .01;}
    if (key == '9') {x_skew += .01;}
    if (key == '0') {x_skew -= .01;}
    */
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
		fb_delayamount=0;
		
		brightInvert=0;
		hueInvert=0;
		saturationInvert=0;
		
		verticalMirror=0;
		horizontalMirror=0;
		
		x_skew=0;
		y_skew=0;
		
		framebuffer0.begin();
		ofClear(0, 0, 0, 255);
		framebuffer0.end();
		 
		for(int i=0;i<framebufferLength;i++){
			pastFrames[i].begin();
			ofClear(0,0,0,255);
			pastFrames[i].end();
		}
	}
    
  

//lfobiz
	if(key=='1'){
		globalLfoAmpSwitch=1;
		if(p_lock_0_switch==1){
			savedState=0;
		}
		if(videoReactiveSwitch==1){
			savedState=1;
		}
		p_lock_0_switch=0;
		videoReactiveSwitch=0;
		//cout<<"lfo active"<<endl;
	}
	if(key=='2'){
		globalLfoRateSwitch=1;
		if(p_lock_0_switch==1){
			savedState=0;
		}
		if(videoReactiveSwitch==1){
			savedState=1;
		}
		p_lock_0_switch=0;
		videoReactiveSwitch=0;
		//cout<<"lfo active"<<endl;
	}
	/*
	if(xLfoAmpSwitch==1){
		if(key=='+'){
			xLfoRate+=.01;
			cout<<"up"<<endl;
		}		
		if(key=='-'){
			xLfoRate-=.01;
			cout<<"down"<<endl;
		}
	}
	*/	
	if(key=='0'){xLfoRate=xLfoAmp=yLfoRate=yLfoAmp=zLfoAmp=zLfoRate=rotateLfoAmp=rotateLfoRate=0;}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if(key=='1'){
		globalLfoAmpSwitch=0;
		if(savedState==0){
			p_lock_0_switch=1;
		}
		if(savedState==1){
			videoReactiveSwitch=1;
		}
		cout<<"lfo amp inactive"<<endl;
		xLfoAmpSwitch=0;
		yLfoAmpSwitch=0;
		zLfoAmpSwitch=0;
		rotateLfoAmpSwitch=0;
	}
	if(key=='2'){
		globalLfoRateSwitch=1;
		if(savedState==0){
			p_lock_0_switch=1;
		}
		if(savedState==1){
			videoReactiveSwitch=1;
		}
		cout<<"lfo rate inactive"<<endl;
		xLfoRateSwitch=0;
		yLfoRateSwitch=0;
		zLfoRateSwitch=0;
		rotateLfoRateSwitch=0;
	}
}

//----------------------------------------------------------
void ofApp::midiSetup(){
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
//------------------------------------------------------------------
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

                //videoreactive
                if(message.control==39){
                    if(message.value==127){
						videoReactiveSwitch=1;
						p_lock_0_switch=0;
                    }
                    if(message.value==0){
						videoReactiveSwitch=0;
						p_lock_0_switch=1;
                    }
                }
                if(message.control==55){
                    if(message.value==127){
                        p_lock_switch=1;
                        for(int i=0;i<p_lock_number;i++){
							p_lock_smoothed[i]=0;
							for(int j=0;j<p_lock_size;j++){
								p_lock[i][j]=p_lock[i][p_lock_increment];
							}
						}
                    }
                    if(message.value==0){
                        p_lock_switch=0;
                    }
                }
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
						toroidSwitch=TRUE;
					}	
					if(message.value==0){
						toroidSwitch=FALSE;
					}
                }                
                if(message.control==61){
					if(message.value==127){
						mirrorSwitch=TRUE;
					}					
					if(message.value==0){
						mirrorSwitch=FALSE;
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
                if(message.control==60){
					if(message.value==127){
						lumakeyInvertSwitch=TRUE;
					}					
					if(message.value==0){
						lumakeyInvertSwitch=FALSE;
					}
                }
                //CONTINUOUS CONTROLS
                if(message.control==16){
					if(control_switch==0){	
						if(p_lock_0_switch==1){
							vmidiActiveFloat[0]=0;
							if(abs(message.value/127.0f-p_lock[0][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[0]=1;
							}
							if(midiActiveFloat[0]==1){
								p_lock[0][p_lock_increment]=message.value/127.0f;
							}
						}
						if(videoReactiveSwitch==1){
							midiActiveFloat[0]=0;
							if(abs(message.value/127.0f-vLumakeyValue)<CONTROL_THRESHOLD){
								vmidiActiveFloat[0]=1;
							}
							if(vmidiActiveFloat[0]==1){
								vLumakeyValue=message.value/127.0f;
							}		
						}              
                    }
                }
                if(message.control==17){
                   if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[1]=0;
							if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[1][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[1]=1;
							}
							if(midiActiveFloat[1]==1){
								p_lock[1][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}
                    }
					if(videoReactiveSwitch==1){
						midiActiveFloat[1]=0;
						if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-vMix)<CONTROL_THRESHOLD){
							vmidiActiveFloat[1]=1;
						}
						if(vmidiActiveFloat[1]==1){
							vMix=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
						}
					}  
                }
                //c3 maps to fb hue
                if(message.control==18){
                    if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[2]=0;
							if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[2][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[2]=1;
							}
							if(midiActiveFloat[2]==1){
								p_lock[2][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}
                    }
					if(videoReactiveSwitch==1){
						midiActiveFloat[2]=0;
						if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-vHue)<CONTROL_THRESHOLD){
							vmidiActiveFloat[2]=1;
						}
						if(vmidiActiveFloat[2]==1){
							vHue=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
						}
					} 
                }
                //c4 maps to fb satx
                if(message.control==19){
                  if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[3]=0;
							if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[3][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[3]=1;
							}
							if(midiActiveFloat[3]==1){
								p_lock[3][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}
                    }
					if(videoReactiveSwitch==1){
						midiActiveFloat[3]=0;
						if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-vSaturation)<CONTROL_THRESHOLD){
							vmidiActiveFloat[3]=1;
						}
						if(vmidiActiveFloat[3]==1){
							vSaturation=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
						}
					} 
                }
                //c5 maps to fb brightx
                if(message.control==20){
                    if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[4]=0;
							if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[4][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[4]=1;
							}
							if(midiActiveFloat[4]==1){
								p_lock[4][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}
                    }
					if(videoReactiveSwitch==1){
						midiActiveFloat[4]=0;
						if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-vBright)<CONTROL_THRESHOLD){
								vmidiActiveFloat[4]=1;
							}
						if(vmidiActiveFloat[4]==1){
							vBright=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
						}
					} 
                }
                //c6 maps to temporal filter
                if(message.control==21){
                   if(control_switch==0){
					   if(p_lock_0_switch==1){
						vmidiActiveFloat[5]=0;
							if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[5][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[5]=1;
							}
							if(midiActiveFloat[5]==1){
								p_lock[5][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}
                    }
					if(videoReactiveSwitch==1){
						midiActiveFloat[5]=0;
						if(abs((message.value-MIDI_MAGIC)/MIDI_MAGIC-vTemporalFilterMix)<CONTROL_THRESHOLD){
							vmidiActiveFloat[5]=1;
						}
						if(vmidiActiveFloat[5]==1){
							vTemporalFilterMix =(message.value-MIDI_MAGIC)/MIDI_MAGIC;
						}
					} 
                }
                //c7 maps to temporal filter resonance
                if(message.control==22){
                    if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[6]=0;
							if(abs(message.value/127.0f-p_lock[6][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[6]=1;
							}
							if(midiActiveFloat[6]==1){
								p_lock[6][p_lock_increment]=message.value/127.0f;
							}
						}
						if(videoReactiveSwitch==1){
							midiActiveFloat[6]=0;
							if(abs(message.value/127.0f-vTemporalFilterResonance)<CONTROL_THRESHOLD){
								vmidiActiveFloat[6]=1;
							}
							if(vmidiActiveFloat[6]==1){
								vTemporalFilterResonance=message.value/127.0f;
							}		
						}              
                    }
                }
                //c8 maps to brightx
                if(message.control==23){
                     if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[7]=0;
							if(abs(message.value/127.0f-p_lock[7][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[7]=1;
							}
							if(midiActiveFloat[7]==1){
								p_lock[7][p_lock_increment]=message.value/127.0f;
							}
						}
						if(videoReactiveSwitch==1){
							midiActiveFloat[7]=0;
							if(abs(message.value/127.0f-vSharpenAmount)<CONTROL_THRESHOLD){
								vmidiActiveFloat[7]=1;
							}
							if(vmidiActiveFloat[7]==1){
								vSharpenAmount=message.value/127.0f;
							}		
						}              
                    }
                }
                //c9 maps to fb x displace
                if(message.control==120){
					float hackyScalingFactor=1.0;
					if(x_2==TRUE){hackyScalingFactor=2.0;}
					if(x_5==TRUE){hackyScalingFactor=5.0;}
					if(x_10==TRUE){hackyScalingFactor=10.0;}
					if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[8]=0;
							lfoAmpActiveFloat[8]=0;
							lfoRateActiveFloat[8]=0;
							//cout<<"scale*midi = "<<hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC<<endl;
							//cout<<"p_lock[8] = "<<p_lock[8][p_lock_increment]<<endl;
							if(abs( hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC -p_lock[8][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[8]=1;
							}
							if(midiActiveFloat[8]==1){
								p_lock[8][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								if(x_2==TRUE){
									p_lock[8][p_lock_increment]=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(x_5==TRUE){
									p_lock[8][p_lock_increment]=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(x_10==TRUE){
									p_lock[8][p_lock_increment]=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
							}		 	 
						}
                    }
                    if(videoReactiveSwitch==1){
						midiActiveFloat[8]=0;
						lfoAmpActiveFloat[8]=0;
						lfoRateActiveFloat[8]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-vX)<CONTROL_THRESHOLD){
							vmidiActiveFloat[8]=1;
						}
						if(vmidiActiveFloat[8]==1){
							vX=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(x_2==TRUE){
								vX=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							} 
							if(x_5==TRUE){
								vX=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(x_10==TRUE){
								vX=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}	 
						}		 
                    }
                    if(globalLfoAmpSwitch==1){
						xLfoAmpSwitch=1;
						midiActiveFloat[8]=0;
						vmidiActiveFloat[8]=0;
						lfoRateActiveFloat[8]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-xLfoAmp)<CONTROL_THRESHOLD+2.0*x_2+5.0*x_5+10.0*x_10){
							lfoAmpActiveFloat[8]=1;
						}
						if(lfoAmpActiveFloat[8]==1){
							xLfoAmp=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(x_2==TRUE){
								xLfoAmp=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							} 
							if(x_5==TRUE){
								xLfoAmp=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(x_10==TRUE){
								xLfoAmp=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}		
						}	
					}
					if(globalLfoRateSwitch==1){
						xLfoRateSwitch=1;
						midiActiveFloat[8]=0;
						vmidiActiveFloat[8]=0;
						lfoAmpActiveFloat[8]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-xLfoRate)<CONTROL_THRESHOLD+2.0*x_2+4.0*x_5+8.0*x_10){
							lfoRateActiveFloat[8]=1;
						}
						if(lfoRateActiveFloat[8]==1){
							xLfoRate=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(x_2==TRUE){
								xLfoRate=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							} 
							if(x_5==TRUE){
								xLfoRate=4.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(x_10==TRUE){
								xLfoRate=8.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}//cout<<xLfoAmpSwitch<<endl;
					}
                }
                 //c10 maps to fb y displace
                if(message.control==121){
					float hackyScalingFactor=1.0;
					if(y_2==TRUE){hackyScalingFactor=2.0;}
					if(y_5==TRUE){hackyScalingFactor=5.0;}
					if(y_10==TRUE){hackyScalingFactor=10.0;}
					if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[9]=0;
							lfoAmpActiveFloat[9]=0;
							lfoRateActiveFloat[9]=0;
							if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[9][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[9]=1;
							}
							if(midiActiveFloat[9]==1){
								p_lock[9][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								if(y_2==TRUE){
									p_lock[9][p_lock_increment]=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(y_5==TRUE){
									p_lock[9][p_lock_increment]=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(y_10==TRUE){
									p_lock[9][p_lock_increment]=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
							}		 	 
						}
                    }
                    if(videoReactiveSwitch==1){
						midiActiveFloat[9]=0;
						lfoAmpActiveFloat[9]=0;
						lfoRateActiveFloat[9]=0;	
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-vY)<CONTROL_THRESHOLD){
							vmidiActiveFloat[9]=1;
						}
						if(vmidiActiveFloat[9]==1){
							vY=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(y_2==TRUE){
								vY=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							} 
							if(y_5==TRUE){
								vY=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(y_10==TRUE){
								vY=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}	 
						}		 
                    }
                    if(globalLfoAmpSwitch==1){
						yLfoAmpSwitch=1;
						midiActiveFloat[9]=0;
						vmidiActiveFloat[9]=0;
						lfoRateActiveFloat[9]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-yLfoAmp)<CONTROL_THRESHOLD+2.0*y_2+4.0*y_5+8.0*y_10){
							lfoAmpActiveFloat[9]=1;
						}
						if(lfoAmpActiveFloat[9]==1){
							yLfoAmp=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(y_2==TRUE){
								yLfoAmp=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							} 
							if(y_5==TRUE){
								yLfoAmp=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(y_10==TRUE){
								yLfoAmp=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}		
						}	
					}
					if(globalLfoRateSwitch==1){
						yLfoRateSwitch=1;
						midiActiveFloat[9]=0;
						vmidiActiveFloat[9]=0;
						lfoAmpActiveFloat[9]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-yLfoRate)<CONTROL_THRESHOLD+2.0*y_2+4.0*y_5+8.0*y_10){
							lfoRateActiveFloat[9]=1;
						}
						if(lfoRateActiveFloat[9]==1){
							yLfoRate=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(y_2==TRUE){
								yLfoRate=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(y_5==TRUE){
								yLfoRate=4.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(y_10==TRUE){
								yLfoRate=8.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}		
					}	
                }
                //z displace
                if(message.control==122){
					float hackyScalingFactor=1.0;
					if(z_2==TRUE){hackyScalingFactor=2.0;}
					if(z_5==TRUE){hackyScalingFactor=5.0;}
					if(z_10==TRUE){hackyScalingFactor=10.0;}
					if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[10]=0;
							lfoAmpActiveFloat[10]=0;
							lfoRateActiveFloat[10]=0;
							if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[10][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[10]=1;
							}
							if(midiActiveFloat[10]==1){
								p_lock[10][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;						 
								if(z_2==TRUE){
									p_lock[10][p_lock_increment]=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}							 
								if(z_5==TRUE){
									p_lock[10][p_lock_increment]=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(z_10==TRUE){
									p_lock[10][p_lock_increment]=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
							}		 	 
						}
                    }
                    if(videoReactiveSwitch==1){
						midiActiveFloat[10]=0;
						lfoAmpActiveFloat[10]=0;
						lfoRateActiveFloat[10]=0;	
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-vZ)<CONTROL_THRESHOLD){
							vmidiActiveFloat[10]=1;
						}
						if(vmidiActiveFloat[10]==1){
							vZ=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(z_2==TRUE){
								vZ=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}							 
							if(z_5==TRUE){
								vZ=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(z_10==TRUE){
								vZ=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}	 
						}		 
                    }                    
                    if(globalLfoAmpSwitch==1){
						zLfoAmpSwitch=1;						
						midiActiveFloat[10]=0;
						vmidiActiveFloat[10]=0;
						lfoRateActiveFloat[10]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-zLfoAmp)<CONTROL_THRESHOLD+2.0*z_2+4.0*z_5+8.0*z_10){
								lfoAmpActiveFloat[10]=1;
						}						
						if(lfoAmpActiveFloat[10]==1){
							zLfoAmp=(message.value-MIDI_MAGIC)/MIDI_MAGIC;							
							if(z_2==TRUE){
								zLfoAmp=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}							 
							if(z_5==TRUE){
								zLfoAmp=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(z_10==TRUE){
								zLfoAmp=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}		
						}						
					}					
					if(globalLfoRateSwitch==1){
						zLfoRateSwitch=1;						
						midiActiveFloat[10]=0;
						vmidiActiveFloat[10]=0;
						lfoAmpActiveFloat[10]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-zLfoRate)<CONTROL_THRESHOLD+2.0*z_2+4.0*z_5+8.0*z_10){
								lfoRateActiveFloat[10]=1;
						}						
						if(lfoRateActiveFloat[10]==1){
							zLfoRate=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(z_2==TRUE){
								zLfoRate=2.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}							 
							if(z_5==TRUE){
								zLfoRate=4.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(y_10==TRUE){
								zLfoRate=8.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}		
					}	
                }
				//rotations
                if(message.control==123){
					float hackyScalingFactor=1.0;
					if(theta_0==TRUE){hackyScalingFactor=2.0;}
					if(theta_1==TRUE){hackyScalingFactor=5.0;}
					if(theta_2==TRUE){hackyScalingFactor=10.0;}
					if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[11]=0;
							lfoAmpActiveFloat[11]=0;
							lfoRateActiveFloat[11]=0;							
							if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[11][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[11]=1;
							}							
							if(midiActiveFloat[11]==1){
								p_lock[11][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;						 
								if(theta_0==TRUE){
									p_lock[11][p_lock_increment]=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}							 
								if(theta_1==TRUE){
									p_lock[11][p_lock_increment]=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(theta_2==TRUE){
									p_lock[11][p_lock_increment]=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
							}		 	 
						}
                    }                    
                    if(videoReactiveSwitch==1){
						midiActiveFloat[11]=0;
						lfoAmpActiveFloat[11]=0;
						lfoRateActiveFloat[11]=0;						
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-vRotate)<CONTROL_THRESHOLD){
							vmidiActiveFloat[11]=1;
						}						
						if(vmidiActiveFloat[11]==1){
							vRotate=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(theta_0==TRUE){
								vRotate=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}							 
							if(theta_1==TRUE){
								vRotate=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(theta_2==TRUE){
								vRotate=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}	 
						}		 
                    }                    
                    if(globalLfoAmpSwitch==1){
						rotateLfoAmpSwitch=1;	
						midiActiveFloat[11]=0;
						vmidiActiveFloat[11]=0;
						lfoRateActiveFloat[11]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-rotateLfoAmp)<CONTROL_THRESHOLD+2.0*theta_0+4.0*theta_1+8.0*theta_2){
							lfoAmpActiveFloat[11]=1;
						}						
						if(lfoAmpActiveFloat[11]==1){
							rotateLfoAmp=(message.value-MIDI_MAGIC)/MIDI_MAGIC;	
							if(theta_0==TRUE){
								rotateLfoAmp=2.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}							 
							if(theta_1==TRUE){
								rotateLfoAmp=5.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(theta_2==TRUE){
								rotateLfoAmp=10.0*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}		
						}	
					}					
					if(globalLfoRateSwitch==1){
						rotateLfoRateSwitch=1;						
						midiActiveFloat[11]=0;
						vmidiActiveFloat[11]=0;
						lfoAmpActiveFloat[11]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-rotateLfoRate)<CONTROL_THRESHOLD+2.0*theta_0+4.0*theta_1+8.0*theta_2){
							lfoRateActiveFloat[11]=1;
						}						
						if(lfoRateActiveFloat[11]==1){
							rotateLfoRate=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(theta_0==TRUE){
								rotateLfoRate=2.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}							 
							if(theta_1==TRUE){
								rotateLfoRate=4.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(theta_2==TRUE){
								rotateLfoRate=8.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}		
					}	
                }
                //huexMod
                if(message.control==124){
					float hackyScalingFactor=1.0;
					if(huexx_0==TRUE){hackyScalingFactor=2.0;}
					if(huexx_1==TRUE){hackyScalingFactor=5.0;}
					if(huexx_2==TRUE){hackyScalingFactor=10.0;}
                    if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[12]=0;
							if(abs(hackyScalingFactor*message.value/127.0f-p_lock[12][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[12]=1;
							}
							if(midiActiveFloat[12]==1){
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
					}
					if(videoReactiveSwitch==1){
						midiActiveFloat[12]=0;
						if(abs(hackyScalingFactor*message.value/127.0f-vHuexMod)<CONTROL_THRESHOLD){
							vmidiActiveFloat[12]=1;
						}
						if(vmidiActiveFloat[12]==1){
							vHuexMod=(message.value)/32.0f;
							if(huexx_0==TRUE){
								vHuexMod=message.value/64.0f;
							}
							if(huexx_1==TRUE){
								vHuexMod=message.value/96.0f;
							}
							if(huexx_2==TRUE){
								vHuexMod=message.value/127.0f;
							}
						}
					}
                }
                if(message.control==125){
					float hackyScalingFactor=1.0;
					if(huexy_0==TRUE){hackyScalingFactor=2.0;}
					if(huexy_1==TRUE){hackyScalingFactor=5.0;}
					if(huexy_2==TRUE){hackyScalingFactor=10.0;}
					if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[13]=0;
							if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[13][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[13]=1;
							}
							if(midiActiveFloat[13]==1){
								p_lock[13][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								if(huexy_0==TRUE){
									p_lock[13][p_lock_increment]=2*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(huexy_1==TRUE){
									p_lock[13][p_lock_increment]=4*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(huexy_2==TRUE){
									p_lock[13][p_lock_increment]=8*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
							}	 
						}
					}
					if(videoReactiveSwitch==1){
						midiActiveFloat[13]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-vHuexOff)<CONTROL_THRESHOLD){
							vmidiActiveFloat[13]=1;
						}
						if(vmidiActiveFloat[13]==1){
							vHuexOff=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(huexy_0==TRUE){
								vHuexOff=2*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(huexy_1==TRUE){
								vHuexOff=4*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(huexy_2==TRUE){
								vHuexOff=8*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}    
					}
                }
                if(message.control==126){
					float hackyScalingFactor=1.0;
					if(huexz_0==TRUE){hackyScalingFactor=2.0;}
					if(huexz_1==TRUE){hackyScalingFactor=5.0;}
					if(huexz_2==TRUE){hackyScalingFactor=10.0;}
					if(control_switch==0){
						if(p_lock_0_switch==1){
							vmidiActiveFloat[14]=0;
							if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-p_lock[14][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[14]=1;
							}
							if(midiActiveFloat[14]==1){
								p_lock[14][p_lock_increment]=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								if(huexz_0==TRUE){
									p_lock[14][p_lock_increment]=2.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								} 
								if(huexz_1==TRUE){
									p_lock[14][p_lock_increment]=4.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}
								if(huexz_2==TRUE){
									p_lock[14][p_lock_increment]=8.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
								}	 
							}
						}
                     }
                     if(videoReactiveSwitch==1){
						midiActiveFloat[14]=0;
						if(abs(hackyScalingFactor*(message.value-MIDI_MAGIC)/MIDI_MAGIC-vHuexLfo)<CONTROL_THRESHOLD){
							vmidiActiveFloat[14]=1;
						}
						if(vmidiActiveFloat[14]==1){
							vHuexLfo=(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							if(huexz_0==TRUE){
								vHuexLfo=2.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(huexz_1==TRUE){
								vHuexLfo=4.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
							if(huexz_2==TRUE){
								vHuexLfo=8.0f*(message.value-MIDI_MAGIC)/MIDI_MAGIC;
							}
						}    
					} 
				}
                if(message.control==127){
					if(control_switch==0){
						if(p_lock_0_switch==1){
							if(abs(message.value/127.0f-p_lock[15][p_lock_increment])<CONTROL_THRESHOLD){
								midiActiveFloat[15]=1;
							}
							if(midiActiveFloat[15]==1){
								p_lock[15][p_lock_increment]=(message.value)/127.0;
							}
						}
					} 
                }
                if(message.control==43){
					if(message.value==127){
						brightInvert=TRUE;
					}
					if(message.value==0){
						brightInvert=FALSE;
					}
                }
                if(message.control==44){
					if(message.value==127){
						saturationInvert=TRUE;
					}
					if(message.value==0){
						saturationInvert=FALSE;
					}
                }
                if(message.control==62){
					if(message.value==127){
						hdmi_aspect_ratio_switch=TRUE;
					}
					if(message.value==0){
						hdmi_aspect_ratio_switch=FALSE;
					}
                }
                if(message.control==42){
					if(message.value==127){
						hueInvert=TRUE;						
					}					
					if(message.value==0){
						hueInvert=FALSE;
					}
                }
                if(message.control==41){
					if(message.value==127){
						horizontalMirror=TRUE;
					}
					if(message.value==0){
						horizontalMirror=FALSE;
					}
                }
                if(message.control==45){
					if(message.value==127){
						verticalMirror=TRUE;
					}
					if(message.value==0){
						verticalMirror=FALSE;
					}
                 }
                 if(message.control==59){		
					if(message.value==127){
						vLumakeyValue=0.0;
						vMix=0.0;
						vHue=0.0;
						vSaturation=0.0;
						vBright=0.0;
						vTemporalFilterMix=0.0;
						vTemporalFilterResonance=0.0;
						vSharpenAmount=0.0;
						vX=0.0;
						vY=0.0;
						vZ=0.0;
						vRotate=0.0;
						vHuexMod=0.0;
						vHuexOff=0.0;
						vHuexLfo=0.0;
                        for(int i=0;i<p_lock_number;i++){
                            midiActiveFloat[i]=0;
                            vmidiActiveFloat[i]=0;
                            
                            for(int j=0;j<p_lock_size;j++){                               
                                p_lock[i][j]=0;   
                            }                        
                        }
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
							for(int i=0;i<framebufferLength;i++){
								pastFrames[i].begin();
								ofClear(0,0,0,255);
								pastFrames[i].end();
							}
						}
					}
                }
                if(message.control!=58){
					clear_switch=0;
                }
            }		
		}
	}	
}
