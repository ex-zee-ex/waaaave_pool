precision highp float;

//tex0=external input
uniform sampler2D tex0;
//fb = feedback framebuffer
uniform sampler2D fb;
//temporal filter=previous frame
uniform sampler2D temporalFilter;

//continuous controls
uniform float fbMix;
uniform float lumakey;
uniform float temporalFilterMix;
uniform float fbXDisplace;
uniform float fbYDisplace;
uniform float fbZDisplace;
uniform float fbRotate;
uniform float fbHue;
uniform float fbSaturation;
uniform float fbBright;
uniform float fbHuexMod;
uniform float fbHuexOff;
uniform float fbHuexLfo;
uniform float temporalFilterResonance;

//switches
uniform int brightInvert;
uniform int saturationInvert;
uniform int hueInvert;
uniform int horizontalMirror;
uniform int verticalMirror;
uniform int toroidSwitch;
uniform int lumakeyInvertSwitch;
uniform int mirrorSwitch;

//videoreactive controls
uniform float vLumakey;
uniform float vMix;
uniform float vHue;
uniform float vSat;
uniform float vBright;
uniform float vtemporalFilterMix;
uniform float vFb1X;
uniform float vX;
uniform float vY;
uniform float vZ;
uniform float vRotate;
uniform float vHuexMod;
uniform float vHuexOff;
uniform float vHuexLfo;

//location
varying vec2 texCoordVarying;

//---------------------------------------------------------------
vec2 mirrorCoord(in vec2 inCoord,in vec2 inDim){

	
	if(inCoord.x<0.0){inCoord.x=abs(inCoord.x);}
	if(inCoord.y<0.0){inCoord.y=abs(inCoord.y);}
	inCoord.x=mod(inCoord.x,inDim.x*2.0);
	inCoord.y=mod(inCoord.y,inDim.y*2.0);
    
    if(inCoord.x>inDim.x){inCoord.x=inDim.x-mod(inCoord.x,inDim.x);}
    if(inCoord.y>inDim.y){inCoord.y=inDim.y-mod(inCoord.y,inDim.y);}
    

    return inCoord;
}
//-----------------------------------------------------------------
vec2 rotate(in vec2 coord,in float theta){
	//shifting x and y by .5 is equivalent to moving coordinates 0,0 to the center of the screen
    vec2 center_coord=vec2(coord.x-.5,coord.y-.5);
    vec2 rotate_coord=vec2(0,0);
    float spiral=(coord.x+coord.y)/2.0;
    coord.x=spiral+coord.x;
    coord.y=spiral+coord.y;
    rotate_coord.x=center_coord.x*cos(theta)-center_coord.y*sin(theta);
    rotate_coord.y=center_coord.x*sin(theta)+center_coord.y*cos(theta);

    rotate_coord=rotate_coord+vec2(.5,.5);
    return rotate_coord;
    

}
//----------------------------------------------------------------------
vec3 rgb2hsb(in vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
//--------------------------------------------------------------------
vec3 hsb2rgb(in vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
//---------------------------------------------------------------------


void main()
{
	//important to note that texCoordVarying here seems to be automatically scaled between 0 and 1
	
	//define a dummy color that we use at the end of the chain when keying/mixing
	vec4 color=vec4(0,0,0,0);
	
	vec4 input1Color = texture2D(tex0, texCoordVarying);

	vec3 input1ColorHsb= rgb2hsb(vec3(input1Color.r,input1Color.g,input1Color.b));
	
	//videoreactive attenuator
	float VVV=input1ColorHsb.z;

	vec4 temporalFilterColor = texture2D(temporalFilter, texCoordVarying);

	/*********coordinate zones******************/
	//offset coords to make center of the screen (0,0)
	vec2 fbCoord=vec2(texCoordVarying.x-.5,texCoordVarying.y-.5);
	
	//zoom in or out
	fbCoord.x=fbZDisplace*fbCoord.x*(1.0+vZ*VVV);
    fbCoord.y=fbZDisplace*fbCoord.y*(1.0+vZ*VVV);
    
    //apply mirrors if switched on
	if(horizontalMirror==1){
		if(fbCoord.x>0.0){
			fbCoord.x=-1.0*fbCoord.x;
		}
	}
	if(verticalMirror==1){
		if(fbCoord.y>0.0){
			fbCoord.y=-1.0*fbCoord.y;
		}
	}
	//x and y displacement		
    fbCoord=vec2(fbCoord.x+fbXDisplace+(vX*VVV)+.5,fbCoord.y+fbYDisplace+(vY*VVV)+.5);
	
	//rotate
	fbCoord=rotate(fbCoord,fbRotate+(vRotate*VVV));
	
	//wrap coords around y and x direction
	if(toroidSwitch==1){
		if(abs(fbCoord.x)>1.0){fbCoord.x=abs(1.0-fbCoord.x);}
		if(abs(fbCoord.y)>1.0){fbCoord.y=abs(1.0-fbCoord.y);}
		fbCoord=fract(fbCoord);
	}
	
	//mirror coords areound y and x direction
	if(mirrorSwitch==1){
		
		fbCoord=mirrorCoord(fbCoord,vec2(1.0,1.0));
		/*
		
		if(fbCoord.x>4.0){fbCoord.x=fract(fbCoord.x);}
		if(fbCoord.y>4.0){fbCoord.y=fract(fbCoord.y);}
		
		if(fbCoord.x>3.0){fbCoord.x=3.0-fract(fbCoord.x);}
		if(fbCoord.y>3.0){fbCoord.y=3.0-fract(fbCoord.y);}
	
		if(fbCoord.x>2.0){fbCoord.x=fract(fbCoord.x);}
		if(fbCoord.y>2.0){fbCoord.y=fract(fbCoord.y);}
		
		if(fbCoord.x>1.0){fbCoord.x=1.0-fract(fbCoord.x);}
		if(fbCoord.y>1.0){fbCoord.y=1.0-fract(fbCoord.y);}
		
		if(fbCoord.x<-2.0){fbCoord.x=fract(abs(fbCoord.x));}
		if(fbCoord.y<-2.0){fbCoord.y=fract(abs(fbCoord.y));}
		
		if(fbCoord.x<-1.0){fbCoord.x=abs(fract(fbCoord.x));}
		if(fbCoord.y<-1.0){fbCoord.y=abs(fract(fbCoord.y));}
		
		if(fbCoord.x<0.0){fbCoord.x=abs(fbCoord.x);}
		if(fbCoord.y<0.0){fbCoord.y=abs(fbCoord.y);}
	
		fbCoord=fract(fbCoord);
		*/ 
	}

	/*****color zones*****/
	
	vec4 fbColor = texture2D(fb, fbCoord);
	
	//clamp the coords so that it doesnt do that annoying color stretch
	if(toroidSwitch ==0 && mirrorSwitch==0){
		if(fbCoord.x>1.0){fbColor=vec4(0.0);}
		if(fbCoord.y>1.0){fbColor=vec4(0.0);}
		if(fbCoord.x<0.0){fbColor=vec4(0.0);}
		if(fbCoord.y<0.0){fbColor=vec4(0.0);}
	}
	
	//convert to hsb
	vec3 fbColorHsb= rgb2hsb(vec3(fbColor.r,fbColor.g,fbColor.b));

	//attenuate hue & chaotic huezones
	fbColorHsb.x=abs(fbColorHsb.x*fbHue*(1.0+vHue*VVV)+(fbHuexLfo+vHuexLfo*VVV)*sin(fbColorHsb.x/3.14));
	fbColorHsb.x=fract(mod(fbColorHsb.x,fbHuexMod+vHuexMod*VVV)+fbHuexOff+vHuexOff*VVV);
	//attenuate sat and bright
	fbColorHsb.y=clamp(fbColorHsb.y*fbSaturation*(1.0+vSat*VVV),0.0,1.0);
	fbColorHsb.z=clamp(fbColorHsb.z*fbBright*(1.0+vBright*VVV),0.0,1.0);
	
	//inverts
	if(brightInvert==1){fbColorHsb.z=1.0-fbColorHsb.z;}
	if(saturationInvert==1){fbColorHsb.y=1.0-fbColorHsb.y;}
	if(hueInvert==1){fbColorHsb.x=fract(abs(1.0-fbColorHsb.x));}
	
	//convert back to rgba
	fbColor=vec4(vec3(hsb2rgb(vec3(fbColorHsb.x,fbColorHsb.y,fbColorHsb.z))),1.0);
	
	//up the temporal Filter resonance
	vec3 temporalFilterColorHsb= rgb2hsb(vec3(temporalFilterColor.r,temporalFilterColor.g,temporalFilterColor.b));
	temporalFilterColorHsb.z=clamp(temporalFilterColorHsb.z*(1.0+.5*temporalFilterResonance*(1.0+vFb1X*VVV)),0.0,1.0);
	temporalFilterColorHsb.y=clamp(temporalFilterColorHsb.y*(1.0+.25*temporalFilterResonance*(1.0+vFb1X*VVV)),0.0,1.0);
	temporalFilterColor=vec4(vec3(hsb2rgb(vec3(temporalFilterColorHsb.x,temporalFilterColorHsb.y,temporalFilterColorHsb.z))),1.0);
	
	/****MIX AND KEYING**/
	//first 'additive/subtractive' lerping
	color=mix(input1Color, fbColor,fbMix+(vMix*VVV));
	
	//janky keying
	if(lumakeyInvertSwitch==0){
		if(VVV<lumakey+(vLumakey*VVV)){color=fbColor;}
	}
	
	if(lumakeyInvertSwitch==1){
		if(VVV>lumakey+(vLumakey*VVV)){color=fbColor;}
	}
	
	//add temporal filter into the mix
	color=mix(color,temporalFilterColor,temporalFilterMix+(vtemporalFilterMix*VVV));
	
	gl_FragColor = color;
}
