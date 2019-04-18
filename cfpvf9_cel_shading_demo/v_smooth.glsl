#version 410
// vertex shader 
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

// output values that will be interpolatated per-fragment
out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
    vec4 pos4 = (ModelView * vec4(vPosition, 1.0));
	vec3 pos = {pos4[0],pos4[1],pos4[2]};

	vec3 norm = normalize( ModelView*vec4(vNormal, 0.0) ).xyz;

    fN = norm;
    fE = pos;
    //fL = LightPosition.xyz;
    fL = normalize( LightPosition.xyz - vPosition.xyz );

    //if( LightPosition.w != 0.0 ) {
	//fL = LightPosition.xyz - vPosition.xyz;
    //}

    gl_Position = Projection*ModelView*vec4(vPosition,1.0);
}
