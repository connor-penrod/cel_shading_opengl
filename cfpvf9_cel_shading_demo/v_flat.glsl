#version 410

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

// vertex shader
out vec4 color;  //vertex shade

// light and material properties
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;


void main()
{
	
	/// Transform vertex  position into eye coordinates
    vec4 pos4 = (ModelView * vec4(vPosition, 1.0));
	vec3 pos = {pos4[0],pos4[1],pos4[2]};
    vec3 L = normalize( LightPosition.xyz - pos );
    vec3 E = normalize( -pos );
    vec3 H = normalize( L + E );

    // Transform vertex normal into eye coordinates
    vec3 N = normalize( ModelView*vec4(vNormal, 0.0) ).xyz;
	// Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;

    float Kd = max( dot(L, N), 0.0 );
    vec4  diffuse = Kd*DiffuseProduct;
    float Ks = pow( max(dot(N, H), 0.0), Shininess );
    vec4  specular = Ks * SpecularProduct;
    if( dot(L, N) < 0.0 )  specular = vec4(0.0, 0.0, 0.0, 1.0); 
	
    gl_Position = Projection * ModelView * vec4(vPosition, 1.0);

    color = ambient + diffuse + specular;
	//color = {0,1.0,0,0};
    color.a = 1.0;
}
