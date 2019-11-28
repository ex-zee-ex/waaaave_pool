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
void ofApp::setup(){
	
	
    ofSetVerticalSync(true);

   
    ofSetFrameRate(30);

   
    ofBackground(0);
   
    ofHideCursor();
	
	cam1.initGrabber(640,480);
	
	//cam1.initGrabber(320,240);
	
	framebuffer0.allocate(ofGetWidth(),ofGetHeight());
	
	framebuffer0.begin();
	ofClear(0,0,0,255);
	framebuffer0.end();
	
	
	 for(int i=0;i<fbob;i++){
        
        pastFrames[i].allocate(ofGetWidth(), ofGetHeight());
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
        
    
    }//endifor

	
	shader_mixer.load("shadersES2/shader_mixer");
	



   
}

//--------------------------------------------------------------
void ofApp::update(){
	
	cam1.update();

}

//--------------------------------------------------------------
void ofApp::draw(){


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
    shader_mixer.setUniform1f("fb0_mix",jm);
    shader_mixer.setUniform1f("fb0_lumakey_value",kk);
    
    shader_mixer.setUniform1f("fb1_mix",op);
    
    //fb0_displacment variables
    shader_mixer.setUniform1f("fb0_xdisplace",sx);
    shader_mixer.setUniform1f("fb0_ydisplace",dc);
    shader_mixer.setUniform1f("fb0_zdisplace",az);
    shader_mixer.setUniform1f("fb0_rotate",qw);
    
   
    
    
    ofVec3f hsbx;
    hsbx.set(fv,gb,hn);
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
   
	

   ofSetColor(255);
   string msg="fps="+ofToString(ofGetFrameRate(),2);//+" x="+ofToString(dc,5)+" z="+ofToString(az,5);
   ofDrawBitmapString(msg,10,10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
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
        
        pastFrames[i].allocate(ofGetWidth(), ofGetHeight());
        pastFrames[i].begin();
        ofClear(0,0,0,255);
        pastFrames[i].end();
        
    
    }//endifor
    }

   
	
 
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
   
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
