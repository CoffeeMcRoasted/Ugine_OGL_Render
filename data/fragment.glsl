//FRAGMENT SHADER

//uniform sampler2D texsampler;
//uniform bool usestex;
varying vec2 ftex;
varying vec3 fnormal;
//varying vec4 fcolor;
//varying vec3 combinedSpecular;

//Advanced Lighting
uniform int baseTexMode;
uniform bool useReflectTex;
uniform bool useRefractTex;
uniform bool useNormalTex;
uniform sampler2D baseTexSampler;
uniform samplerCube baseCubeSampler;
uniform samplerCube reflectCubeSampler;
uniform samplerCube refractCubeSampler;
uniform sampler2D normalTexSampler;
varying vec3 cubeCoords;
varying vec3 reflectCoords;
varying vec3 refractCoords;
varying mat3 tbnMatrix;

varying vec4 vpos4;

//ILLUMINATION
#define MAX_LIGHTS 8
uniform mat4 modelView;
uniform mat4 normalMatrix;
uniform bool lightingEnabled;
uniform bool lightEnabled[MAX_LIGHTS];
uniform vec4 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform float lightAtt[MAX_LIGHTS];
uniform vec4 diffuse;
uniform vec3 ambient;
uniform int shininess;

void main()
{
	// We combine the obtained color in the frag shader with the texture.
	vec3 combinedSpecular = vec3(0,0,0);
	vec4 fcolor;
	if(lightingEnabled)
	{
		vec3 combinedDiffuse = ambient;

		int i = 0;
		for(i = 0; i < MAX_LIGHTS; ++i)
		{
			if(lightEnabled[i])
			{
				vec3 N;
				if(useNormalTex)
				{
					N = tbnMatrix * (vec3(texture2D(normalTexSampler, ftex))*2.0 - 1.0);
				}
				else
				{
					N = normalize(vec3(normalMatrix * vec4(fnormal, 0)));
				}
				vec3 L = vec3(lightPos[i]);
				float att = 1.0;
				//We check if the light is a point light.
				if(lightPos[i].w == 1.0)
				{
					//If L is pointlight, we calculate the vector from light to vertex
					//in viewer space and the lineal attenuation
					L = L - vec3(modelView * vpos4);
					att = 1.0 / (lightAtt[i] * length(L));
				}
				L = normalize(L);
				//We calculate the diffuse color component of the light i
				float NdotL = max(dot(N, L), 0.0);
				combinedDiffuse += NdotL * lightColor[i] * att;
				//Specular component calc
				if(shininess > 0 && NdotL > 0.0)
				{
					vec3 H = normalize(L - normalize(vec3(modelView * vpos4)));
					float NdotH = max(dot(N,H), 0.0);
					combinedSpecular += pow(NdotH, float(shininess)) * att;
				}
			}
		}
		fcolor = vec4(clamp(vec3(diffuse) * combinedDiffuse, 0.0, 1.0), diffuse[3]); /*vec4(vnormal,1.0f)*/;
	}
	else
	{
		fcolor = diffuse;
	}

	gl_FragColor = fcolor;
	//if(usestex)
	//{
	//	gl_FragColor *= texture2D(texsampler, ftex);
	//}
	if(baseTexMode == 1)
	{
		gl_FragColor *= texture2D(baseTexSampler, ftex);
	}
	else if(baseTexMode == 2)
	{
		gl_FragColor = textureCube(baseCubeSampler, cubeCoords);
	}

	if(useReflectTex)
	{
		gl_FragColor *= textureCube(reflectCubeSampler, reflectCoords);
	}

	if(useRefractTex)
	{
		gl_FragColor *= textureCube(refractCubeSampler, refractCoords);
	}
	// To the final color we add the combined specular calculated before
	gl_FragColor = clamp(gl_FragColor + vec4(combinedSpecular, 1.0), 0.0, 1.0);
}
