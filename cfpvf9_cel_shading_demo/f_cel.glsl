#version 410
// fragment shader

// per-fragment interpolated values from the vertex shader
in vec3 fN;
in vec3 fL;
in vec3 fE;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform float Shininess;
uniform vec4 FeatureColor;
uniform float FeatureAngle;

void main() 
{ 
    // Normalize the input lighting vectors
    
    vec3 N = normalize(fN);
    vec3 E = normalize(-fE);
    vec3 L = normalize(fL);
	vec4 diffuse;

    vec3 H = normalize( L + E );   
    vec4 ambient = AmbientProduct;
	float Kd = max(dot(L, N), 0.0);
	if(Kd > 0.3)
	{
		diffuse = DiffuseProduct;
	}
	else if(Kd > 0.01)
	{
		diffuse = 0.6*DiffuseProduct;
	}
	else
	{
	    diffuse = vec4(0,0,0,1);
	}
	

    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) 
	specular = vec4(0.0, 0.0, 0.0, 1.0);

	if(dot(E, N) < FeatureAngle)
	{
		gl_FragColor = FeatureColor;
    }
	else
	{
		gl_FragColor = ambient + diffuse + specular;
	}
	//gl_FragColor = vec4(1.0,0,0,1.0);
    gl_FragColor.a = 1.0;
} 