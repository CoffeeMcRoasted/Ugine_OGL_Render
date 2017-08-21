//VERTEX SHADER
uniform mat4 MVP;
attribute vec3 vpos;
attribute vec2 vtex;
attribute vec3 vnormal;
attribute vec3 vtangent;
varying vec2 ftex;
varying vec3 fnormal;
// Color Variable to send to Fragment
//varying vec4 fcolor;
// Shininess variable to send to FRAGMENT
//varying vec3 combinedSpecular;

//Animation
#define MAX_BONES 75
uniform bool skinned;
uniform mat4 animMatrices[MAX_BONES];

attribute vec4 vboneIndices;
attribute vec4 vboneWeights;

//Advanced ILLUMINATION
uniform mat4 normalMatrix;
uniform mat4 model;
uniform vec3 eyePos;
uniform float refractCoef;

varying vec3 cubeCoords;
varying vec3 reflectCoords;
varying vec3 refractCoords;
varying mat3 tbnMatrix;
varying vec4 vpos4;



void main() {
	vpos4 = vec4(vpos, 1);
	// Bone based animation
	if(skinned)
	{
		mat4 boneTransform = mat4(1);
		for ( int i = 0; i < 4; ++i )
		{
			int index = int(vboneIndices[i]);
			if ( index > -1 ) boneTransform += animMatrices[index] * vboneWeights[i];
		}
		vpos4 = boneTransform * vpos4;
	}

	vec3 eye = normalize(vec3(model * vec4(vpos, 1)) - eyePos);
	vec3 normal = normalize(vec3(model * vec4(vnormal, 0)));
	vec3 refl = normalize(reflect(eye, normal));
	vec3 refr = normalize(refract(eye, normal, refractCoef));
	// Transformed vertex position
	gl_Position = MVP * vpos4;
	//gl_Position = MVP * vec4(vpos, 1.0);
	ftex = vtex;
	fnormal = vnormal;
	cubeCoords= normalize(vpos);
	reflectCoords = refl;
	refractCoords = refr;
	vec3 n = vec3(normalMatrix * vec4(vnormal, 1));
	vec3 t = vec3(normalMatrix * vec4(vtangent, 1));
	vec3 bt = cross(n, t);
	tbnMatrix[0] = t;
	tbnMatrix[1] = bt;
	tbnMatrix[2] = n;
}
