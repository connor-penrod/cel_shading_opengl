#version 410
// fragment shader

// per-fragment interpolated values from the vertex shader
in vec3 fN;
in vec3 fL;
in vec3 fE;
//flat in int celshaded;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform float Shininess;
uniform vec4 FeatureColor;
uniform float FeatureAngle;
uniform float ContourThreshold;
uniform int ShowDerivatives;
uniform float TransitionPosition;
uniform vec4 viewport;
uniform mat4 Projection;
uniform int Stage;

void main() 
{ 
	int change = 0;
	vec4 ndcPos;
	mat4 invPersMatrix = inverse(Projection);
	ndcPos.xy = ((2.0 * gl_FragCoord.xy) - (2.0 * viewport.xy)) / (viewport.zw) - 1;
	ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
		(gl_DepthRange.far - gl_DepthRange.near);
	ndcPos.w = 1.0;

	vec4 clipPos = ndcPos / gl_FragCoord.w;
	vec4 eyePos = invPersMatrix * clipPos;
	if(eyePos.x > TransitionPosition)
	{
		if(abs(eyePos.x - TransitionPosition) < 0.01)
		{
			gl_FragColor = vec4(0,0,0,1);
			return;
		}
		change = 1;
	}
    // Normalize the input lighting vectors
    vec3 N = normalize(fN);
	//vec3 pos = gl_FragCoord.xyz;
	//vec3 N = normalize(cross(dFdx(fE), dFdy(fE)));
    vec3 E = normalize(-fE);
    vec3 L = normalize(fL);
	vec4 diffuse;
    vec3 H = normalize( L + E );
    vec4 ambient = AmbientProduct;
	float Kd = max(dot(L, N), 0.0);

	float gradient = fwidth(N.x + N.y + N.z);

	float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

	/*
	void Stage0();
	{
		diffuse = DiffuseProduct*Kd; 
		gl_FragColor = ambient + diffuse + specular;
	}
	void Stage1()
	{
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
			diffuse = 0*DiffuseProduct;
		}
		gl_FragColor = ambient + specular + diffuse;
	}
	void Stage2()
	{
		if(dot(E, N) < FeatureAngle)
		{
			gl_FragColor = FeatureColor;
		}
	}
	void Stage3()
	{
		if(gradient > ContourThreshold)
		{
			gl_FragColor = vec4(0,0,0,1);
		}
	}
	*/

	if(Stage >= 3)
	{
		if(change==1)
		{
			if(gradient > ContourThreshold)
			{
				gl_FragColor = vec4(0,0,0,1);
			}
			else
			{
				if(dot(E, N) < FeatureAngle)
				{
					gl_FragColor = FeatureColor;
				}
				else
				{
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
						diffuse = 0*DiffuseProduct;
					}
					gl_FragColor = ambient + specular + diffuse;
				}	
			}
		}
		else
		{
			if(dot(E, N) < FeatureAngle)
			{
				gl_FragColor = FeatureColor;
			}
			else
			{
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
					diffuse = 0*DiffuseProduct;
				}
				gl_FragColor = ambient + specular + diffuse;
			}
		}
	}
	else if(Stage >= 2)
	{
		if(change==1)
		{
			if(dot(E, N) < FeatureAngle)
			{
				gl_FragColor = FeatureColor;
			}
			else
			{
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
					diffuse = 0*DiffuseProduct;
				}
				gl_FragColor = ambient + specular + diffuse;
			}
		}
		else
		{
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
				diffuse = 0*DiffuseProduct;
			}
			gl_FragColor = ambient + specular + diffuse;
		}
	}
	else if(Stage >= 1)
	{
		if(change==1)
		{
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
				diffuse = 0*DiffuseProduct;
			}
			gl_FragColor = ambient + specular + diffuse;
		}
		else
		{
			diffuse = DiffuseProduct*Kd; 
			gl_FragColor = ambient + diffuse + specular;
		}
	}
	else
	{
		diffuse = DiffuseProduct*Kd; 
		gl_FragColor = ambient + diffuse + specular;
	}
	/*
	else
	{
		diffuse = DiffuseProduct*Kd; 
		gl_FragColor = ambient + diffuse + specular;
	}*/

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) 
	specular = vec4(0.0, 0.0, 0.0, 1.0);

	//gl_FragColor = ambient + diffuse + specular;
	

	if(ShowDerivatives != 0)
	{
		gl_FragColor = vec4(0,0 + gradient*5,0 + gradient*5,1.0);
	}
    gl_FragColor.a = 1.0;
} 