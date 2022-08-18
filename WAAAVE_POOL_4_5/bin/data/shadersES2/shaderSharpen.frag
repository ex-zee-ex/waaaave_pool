precision highp float;

uniform sampler2D tex0;

uniform float sharpenAmount;
uniform float vSharpenAmount;
varying vec2 texCoordVarying;

//-------------------------
vec3 rgb2hsb(in vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}
//---------------------------
vec3 hsb2rgb(in vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}
//-------------------------
void main()
{
	vec4 outColor=vec4(1.0,0.0,1.0,1.0);
	//first we try to just sharpen brightness
	
	
	float colorSharpenBright=0.0;
	float X=2.0*.0015625;
	float Y=2.0*.0020833;
	
	//colorSharpenBright=rbg2hsb(texture2D(tex0, texCoordVarying+vec2(X,0.0))).z;
	
	colorSharpenBright = //rgb2hsb(texture2D(tex0, texCoordVarying+vec2(X,0.0)).rgb).z +
			//rgb2hsb(texture2D(tex0, texCoordVarying+vec2(-X,0.0)).rgb).z +
			rgb2hsb(texture2D(tex0, texCoordVarying+vec2(X,Y)).rgb).z +
			rgb2hsb(texture2D(tex0, texCoordVarying+vec2(-X,Y)).rgb).z +
			rgb2hsb(texture2D(tex0, texCoordVarying+vec2(-X,-Y)).rgb).z +
			rgb2hsb(texture2D(tex0, texCoordVarying+vec2(X,-Y)).rgb).z;
			//rgb2hsb(texture2D(tex0, texCoordVarying+vec2(0.0,-Y)).rgb).z +
			//rgb2hsb(texture2D(tex0, texCoordVarying+vec2(.0,Y)).rgb).z;
			
	colorSharpenBright=colorSharpenBright*.125;		
	
	vec4 ogColor=texture2D(tex0,texCoordVarying);
	vec3 ogColorHSB=rgb2hsb(ogColor.rgb);
	
	float VVV=ogColorHSB.z;
	
	ogColorHSB.z-=sharpenAmount*colorSharpenBright+(colorSharpenBright*vSharpenAmount*VVV);
	
	//got to boost that bb
	if(sharpenAmount>0.0){
	ogColorHSB.z*=(1.0+sharpenAmount*.45+.45*(vSharpenAmount*VVV));
	ogColorHSB.x*=(1.0+sharpenAmount*.25+.25*(vSharpenAmount*VVV));	
	}
	
	outColor.rgb=hsb2rgb(ogColorHSB);
	outColor.a=1.0;
	//color.rgb=1.0-color.rgb;
	gl_FragColor = outColor;//texture2D(tex0,texCoordVarying);
}
