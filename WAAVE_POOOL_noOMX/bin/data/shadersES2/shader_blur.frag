precision highp float;


uniform sampler2D tex0;

varying vec2 texCoordVarying;

void main()
{
	vec4 color_blur=vec4(0.0);
	
	color_blur=texture2D(tex0,texCoordVarying);
	
	gl_FragColor = color_blur;
}
