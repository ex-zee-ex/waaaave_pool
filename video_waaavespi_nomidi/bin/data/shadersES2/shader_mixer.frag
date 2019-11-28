precision highp float;


uniform sampler2D tex0;

uniform sampler2D fb0;

uniform sampler2D fb1;


uniform float fb0_mix;
uniform float fb0_lumakey_value;
uniform float fb0_lumakey_threshold;

uniform float fb1_mix;

uniform float fb0_xdisplace;
uniform float fb0_ydisplace;
uniform float fb0_zdisplace;
uniform float fb0_rotate;

uniform vec3 fb0_hsbx;

uniform vec3 fb0_rescale;

uniform float test;

varying vec2 texCoordVarying;



vec2 rotate(in vec2 coord,in float theta){
	//shifting x and y by .5 is equivalent to moving coordinates 0,0 to the center of the screen
    vec2 center_coord=vec2(coord.x-.5,coord.y-.5);
    vec2 rotate_coord=vec2(0,0);
    float spiral=(coord.x+coord.y)/2.0;
    coord.x=spiral+coord.x;
    coord.y=spiral+coord.y;
    rotate_coord.x=center_coord.x*cos(theta)-center_coord.y*sin(theta);
    rotate_coord.y=center_coord.x*sin(theta)+center_coord.y*cos(theta);
    
    
 

   // rotate_coord.x=center_coord.x*cos(theta)-center_coord.y*sin(theta);
   // rotate_coord.y=center_coord.x*sin(theta)+center_coord.y*cos(theta);
    
    rotate_coord=rotate_coord+vec2(.5,.5);
    //rotate_coord=mod(rotate_coord,vec2(width,height));
    
    
    if(abs(rotate_coord.x)>1.0){rotate_coord.x=abs(1.0-rotate_coord.x);}
    if(abs(rotate_coord.y)>1.0){rotate_coord.y=abs(1.0-rotate_coord.y);}
    
    return rotate_coord;
    

}//endrotate

vec3 rgb2hsb(in vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsb2rgb(in vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


/*

vec3 rgb2hsb( in vec3 c ){
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz),
                 vec4(c.gb, K.xy),
                 step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r),
                 vec4(c.r, p.yzx),
                 step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                d / (q.x + e),
                q.x);
}

vec3 hsb2rgb( in vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                             6.0)-3.0)-1.0,
                     0.0,
                     1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

*/

void main()
{
	//important to note that texCoordVarying here seems to be automatically scaled between 0 and 1
	
	vec4 color=vec4(0,0,0,0);
	
	vec4 cam1_color = texture2D(tex0, texCoordVarying);
	
	vec4 fb1_color = texture2D(fb1, texCoordVarying);
	
	//coordinate zones
	
	
	vec2 fb0_coord=vec2(texCoordVarying.x-.5,texCoordVarying.y-.5);
	fb0_coord.x=fb0_zdisplace*fb0_coord.x;
    fb0_coord.y=fb0_zdisplace*fb0_coord.y;
    fb0_coord=vec2(fb0_coord.x+fb0_xdisplace+.5,fb0_coord.y+fb0_ydisplace+.5);
	
	fb0_coord=rotate(fb0_coord,fb0_rotate);
	
	fb0_coord=fract(fb0_coord);
	
	
	
	vec4 fb0_color = texture2D(fb0, fb0_coord);
	
	//cam1_color=pow(cam1_color,vec4(2));

	//convert to hsb
	
	
	vec3 fb0_color_hsb= rgb2hsb(vec3(fb0_color.r,fb0_color.g,fb0_color.b));
	
	//attenuate hsb
	
	fb0_color_hsb.x=fb0_color_hsb.x*fb0_hsbx.x;
	fb0_color_hsb.y=clamp(fb0_color_hsb.y*fb0_hsbx.y,0.0,1.0);
	fb0_color_hsb.z=clamp(fb0_color_hsb.z*fb0_hsbx.z,0.0,1.0);
	
	
	fb0_color=vec4(vec3(hsb2rgb(vec3(fb0_color_hsb.x,fb0_color_hsb.y,fb0_color_hsb.z))),1.0);
	
	color=mix(cam1_color, fb0_color,fb0_mix);
	
	
	//janky keying
	float cam1_bright=.33*cam1_color.r+.5*cam1_color.g+.16*cam1_color.b;
	if(cam1_bright<fb0_lumakey_value)
	{
		color=fb0_color;
	}
	
	
	color=mix(color,fb1_color,fb1_mix);
	
	gl_FragColor = color;
}
