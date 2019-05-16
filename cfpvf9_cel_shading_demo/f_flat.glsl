#version 410
// fragment shader

in vec4 color;
uniform int Time;

void main() 
{ 
    gl_FragColor = color;
	gl_FragColor.a = noise1(gl_FragCoord.x + gl_FragCoord.y) + 1;
} 
