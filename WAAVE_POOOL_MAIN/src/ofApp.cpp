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
bool scaleswitch=1;

//0 is picaputre, 1 is usbinput
bool inputswitch=1;

//0 is no midi controller input, 1 is midi controller
bool midiswitch=1;

//at the moment there is a brightness reduction that comes along with
//having the midi controller involved so if we have one then
//we send a little boost into the shaders.
int midiscaler=1;

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


float c1=0.0;
float c2=0.0;
float c3=0.0;
float c4=0.0;
float c5=0.0;
float c6=0.0;
float c7=0.0;
float c8=0.0;
float c9=0.0;
float c10=0.0;
float c11=0.0;
float c12=0.0;
float c13=0.0;
float c14=0.0;
float c15=.0;
float c16=.0;

int width=0;
int height=0;


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

const int fbob=60;

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


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);

   ofSetFrameRate(30);
    
    ofBackground(0);
   
    ofHideCursor();
	
		
	if(scaleswitch==0){
		width=320;
		height=240;
	}
	
	if(scaleswitch==1){
		width=640;
		height=480;
	}
	  
    settings.sensorWidth = 640;
    settings.sensorHeight = 480;
    settings.framerate = 30;
    settings.enableTexture = true;
    settings.sensorMode=7;
    
    settings.whiteBalance ="Off";
    settings.exposurePreset ="Off";
    settings.whiteBalanceGainR = 1.0;
    settings.whiteBalanceGainB = 1.0;
    
    //pass in the settings and it will start the camera
	if(inputswitch==0){
		videoGrabber.setup(settings);
	}

	
	if(inputswitch==1){
		cam1.initGrabber(width,height);
	}
	
	framebuffer0.allocate(width,height);
	
	framebuffer0.begin();
	ofClear(0,0,0,255);
	framebuffer0.end();
	
	
	/*
	framebuffer1.allocate(width,height);
	
	
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


	if(midiswitch==1){
		midiscaler=2;
	}
	
	shader_mixer.load("shadersES2/shader_mixer");
	
	shader_blur.load("shadersES2/shader_blur");
	
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
	
	if(inputswitch==1){
		cam1.update();
	}
	
	if(inputswitch==0){
		videoGrabber.setSharpness(100);
		videoGrabber.setBrightness(40);
		videoGrabber.setContrast(100);
		videoGrabber.setSaturation(0);
	}
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
		    
                
                /* the nanostudio kontrols
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
                
                
                 if(message.control==39){
					if(message.value==127){
						y_skew_switch=TRUE;
					}
					
					if(message.value==0){
						y_skew_switch=FALSE;
					}
					
                }
                
                if(message.control==55){
					if(message.value==127){
						x_skew_switch=TRUE;
					}
					
					if(message.value==0){
						x_skew_switch=FALSE;
					}
					
                }
                if(y_skew_switch==TRUE){
					y_skew+=.00001;
                }
                
                if(x_skew_switch==TRUE){
					x_skew+=.00001;
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
                
                if(message.control==62){
					if(message.value==127){
						luma_switch=TRUE;
					}
					
					if(message.value==0){
						luma_switch=FALSE;
					}
					
                }
                
                if(message.control==61){
					if(message.value==127){
						x_mirror_switch=TRUE;
					}
					
					if(message.value==0){
						x_mirror_switch=FALSE;
					}
					
                }
                
                 if(message.control==60){
					if(message.value==127){
						y_mirror_switch=TRUE;
					}
					
					if(message.value==0){
						y_mirror_switch=FALSE;
					}
					
                }
                
                
                
                //nanokontrol2 controls
                 //c1 maps to fb0 hue attenuation
                if(message.control==16){
                  //  c1=(message.value-63.0)/63.0;
                     c1=(message.value)/127.00;
                    
                }
                
                //c2 maps to fb0 saturation attenuation
                if(message.control==17){
                    c2=(message.value-63.0)/63.0;
                   //   c2=(message.value)/127.00;
                    
                }
                
                //c3 maps to fb0 brightness attenuation
                if(message.control==18){
                    c3=(message.value-63.0)/63.00;
                    //  c3=(message.value)/127.00;
                }
                
                //c4 maps to fb0_mix amount
                if(message.control==19){
                     c4=(message.value-63.0)/63.00;
                   // c4=(message.value)/127.00;
                   
                }
                
                //c5 maps to fb0 x displace
                if(message.control==20){
                     c5=(message.value-63.0)/63.00;
                  //  c5=(message.value)/127.00;
                  
                }
                
                //c6 maps to fb0 y displace
                if(message.control==21){
                    c6=(message.value-63.0)/63.0;
                     // c6=(message.value)/127.00;
                }
                
                //c7 maps to fb0 z displace
                if(message.control==22){
                    c7=(message.value-63.0)/63.0;
                   //   c7=(message.value)/127.00;
                }
                
                //c8 maps to fb0 luma key value
                if(message.control==23){
                    // c8=(message.value-63.0)/63.00;
                    c8=(message.value)/127.0;
                   
                }
                
                //c9 maps to fb0 x displace
                if(message.control==120){
                     c9=(message.value-63.0)/63.00;
                     
                     if(x_2==TRUE){
						 c9=2.0*(message.value-63.0)/63.00;
						 }
						 
					 if(x_5==TRUE){
						 c9=5.0*(message.value-63.0)/63.00;
						 }
					 if(x_10==TRUE){
						 c9=10.0*(message.value-63.0)/63.00;
						 }	 	 
                    //c9=(message.value)/127.0;
                   
                }
                
                 //c10 maps to fb0 y displace
                if(message.control==121){
                     c10=(message.value-63.0)/63.00;
                     
                     
                     if(y_2==TRUE){
						 c10=2.0*(message.value-63.0)/63.00;
						 }
						 
					 if(y_5==TRUE){
						 c10=5.0*(message.value-63.0)/63.00;
						 }
					 if(y_10==TRUE){
						 c10=10.0*(message.value-63.0)/63.00;
						 }	 	 
                     
                     
                    //c10=(message.value)/127.0;
                   
                }
                
               
                if(message.control==122){
                     c11=(message.value-63.0)/63.00;
                     
                     if(z_2==TRUE){
						 c11=2.0*(message.value-63.0)/63.00;
						 }
						 
					 if(z_5==TRUE){
						 c11=5.0*(message.value-63.0)/63.00;
						 }
					 if(z_10==TRUE){
						 c11=10.0*(message.value-63.0)/63.00;
						 }	 	 
                     
                    //c11=(message.value)/127.0;
                   
                }
              
                if(message.control==123){
                     c12=(message.value-63.0)/63.00;
                     
                     if(theta_0==TRUE){
						 c12=2*(message.value-63.0)/63.00;
						 }
						 
					 if(theta_1==TRUE){
						 c12=4*(message.value-63.0)/63.00;
						 }
					 if(theta_2==TRUE){
						 c12=8*(message.value-63.0)/63.00;
						 }	 	
                     
                   // c12=(message.value)/127.0;
                   
                }
                
             
                if(message.control==124){
                    // c13=(message.value-63.0)/63.00;
                     
                    //instead of switches to go from different multiples
                    //switche to go from 0-.25, 0-.5,0-.75,0-1 
                     
                    c13=(message.value)/32.0;
                    
                    if(huexx_0==TRUE){
						 c13=message.value/64;
						 }
						 
					 if(huexx_1==TRUE){
						 c13=message.value/96.00;
						 }
					 if(huexx_2==TRUE){
						 c13=message.value/127.00;
						 }	 
                   
                }
              
            
                if(message.control==125){
                     c14=(message.value-63.0)/63.00;
                     
                     if(huexy_0==TRUE){
						 c14=2*(message.value-63.0)/63.00;
						 }
						 
					 if(huexy_1==TRUE){
						 c14=4*(message.value-63.0)/63.00;
						 }
					 if(huexy_2==TRUE){
						 c14=8*(message.value-63.0)/63.00;
						 }	 
                     
                     
                     
                    //c14=(message.value)/127.0;
                   
                }
                
               
                if(message.control==126){
                     c15=(message.value-63.0)/63.00;
                     
                     if(huexz_0==TRUE){
						 c15=2*(message.value-63.0)/63.00;
						 }
						 
					 if(huexz_1==TRUE){
						 c15=4*(message.value-63.0)/63.00;
						 }
					 if(huexz_2==TRUE){
						 c15=8*(message.value-63.0)/63.00;
						 }	 
                     
                    //c15=(message.value)/127.0;
                   
                }
                
                
                if(message.control==127){
                   //  c16=(message.value-63.0)/63.00;
                    c16=(message.value)/127.0;
                   
                }
                
                
                
                
                
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
	
	
	//end midi biz
	
	
	//control attenuation section

	
	
	
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
			cam1.draw(0,0);
		}
	}
	
	//videoGrabber.draw(0,0);
    
    shader_mixer.setUniformTexture("fb0", pastFrames[(abs(framedelayoffset-fbob-fb0_delayamount-int(c16*(fbob-1.0)))-1)%fbob].getTexture(),1);
    shader_mixer.setUniformTexture("fb1", pastFrames[(abs(framedelayoffset-fbob)-1)%fbob].getTexture(),2);
    
    
    //mixing variables
    shader_mixer.setUniform1f("fb0_mix",jm+2.0*c2);
    shader_mixer.setUniform1f("fb0_lumakey_value",kk+1.01*c1);
   
    
    
    shader_mixer.setUniform1f("fb1_mix",op+1.1*c6);
    
    //fb0_displacment variables
    shader_mixer.setUniform1f("fb0_xdisplace",sx+.01*c9);
    shader_mixer.setUniform1f("fb0_ydisplace",dc+.01*c10);
    shader_mixer.setUniform1f("fb0_zdisplace",az*(1+.05*c11));
    shader_mixer.setUniform1f("fb0_rotate",qw+.314159265*c12);
    
   
    
    
    ofVec3f dummy3f;
    dummy3f.set(fv*(1.0+.75*c3),gb*(1.0+c4),hn*(1.0+.5*c5));//(1.0+.35*c5));
    shader_mixer.setUniform3f("fb0_hsbx",dummy3f);
    
    
    dummy3f.set(er*(1.0-c13),ty+.25*c14,ui+.25*c15);
    //dummy3f.set(1.0,.5,.01);
    shader_mixer.setUniform3f("fb0_huex",dummy3f);
    
    shader_mixer.setUniform1f("fb1_brightx", fb1_brightx+c7);
    shader_mixer.setUniform1f("cam1_brightx",ll+2.0*c8);
    
    
    shader_mixer.setUniform1i("fb0_b_invert",fb0_b_invert);
    shader_mixer.setUniform1i("fb0_h_invert",fb0_h_invert);
    shader_mixer.setUniform1i("fb0_s_invert",fb0_s_invert);
    
    shader_mixer.setUniform1i("fb0_h_mirror",fb0_h_mirror);
    shader_mixer.setUniform1i("fb0_v_mirror",fb0_v_mirror);
    
    shader_mixer.setUniform1i("toroid_switch",toroid_switch);
    
    shader_mixer.setUniform1f("boost",midiscaler);
    
    
    shader_mixer.setUniform1i("luma_switch",luma_switch);
    
    shader_mixer.setUniform1i("x_mirror_switch",x_mirror_switch);
    
    shader_mixer.setUniform1i("y_mirror_switch",y_mirror_switch);

    shader_mixer.end();
	framebuffer0.end();
	
	//_-_-__---_---___
	
	
	
	//_----___---------_-_-----_--_-_--_--_
	
	
	
	framebuffer0.draw(0,0,720,480);
	

	//_-------------------------------------------
	
	
	pastFrames[abs(fbob-framedelayoffset)-1].begin(); //eeettt
    
    ofPushMatrix();
	ofTranslate(360,240);
    ofRotateYRad(y_skew);
    ofRotateXRad(x_skew);
    framebuffer0.draw(-360,-240);
	
	ofPopMatrix();
	
    pastFrames[abs(fbob-framedelayoffset)-1].end(); //eeettt

	incIndex();
   
	//ofDrawBitmapString("fps =" + ofToString(getFps()), 10, ofGetHeight() - 5 );

//i use this block of code to print out like useful information for debugging various things and/or just to keep the 
//framerate displayed to make sure i'm not losing any frames while testing out new features.  uncomment the ofDrawBitmap etc etc
//to print the stuff out on screen
   ofSetColor(255);
   string msg="fps="+ofToString(ofGetFrameRate(),2)+" clear switch"+ofToString(clear_switch,5);//+" z="+ofToString(az,5);
  // ofDrawBitmapString(msg,10,10);
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
    if(key=='3'){fb0_h_invert=!fb0_h_invert;}
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
