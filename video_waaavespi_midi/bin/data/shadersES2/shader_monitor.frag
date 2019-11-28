precision highp float;

uniform sampler2D tex0;


varying vec2 texCoordVarying;

void main()
{
	
	vec4 cam1_color = texture2D(tex0, texCoordVarying);
	cam1_color.w=(0.299*cam1_color.r + 0.587*cam1_color.g + 0.114*cam1_color.b);

	
	gl_FragColor = cam1_color;
}
