#version 410
// vertex shader 
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vNormalAvg;

// output values that will be interpolatated per-fragment
out vec3 fN;
out vec3 fE;
out vec3 fL;
//flat out int celshaded;


uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;
uniform float TransitionPosition;

void main()
{
    vec4 pos4 = (ModelView * vec4(vPosition, 1.0));
	vec3 pos = {pos4[0],pos4[1],pos4[2]};

	vec3 norm = normalize( ModelView*vec4(vNormalAvg, 0.0) ).xyz;

    fN = norm;
    fE = pos;
    //fL = LightPosition.xyz;
    fL = normalize( LightPosition.xyz - vPosition.xyz );

    //if( LightPosition.w != 0.0 ) {
	//fL = LightPosition.xyz - vPosition.xyz;
    //}
	/*
	if(vPosition.x < TransitionPosition)
	{
		celshaded = 0;
	}
	else
	{
		celshaded = 1;
	}*/

    gl_Position = Projection*ModelView*vec4(vPosition,1.0);
}
