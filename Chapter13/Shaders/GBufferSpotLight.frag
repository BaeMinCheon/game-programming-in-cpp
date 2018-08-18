
#version 330

in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;
uniform sampler2D uGSpecPower;

struct SpotLight
{
	vec3 mWorldPos;
	vec3 mDiffuseColor;
	float mInnerRadius;
	float mOuterRadius;
	vec3 mSpecularColor;
};

uniform SpotLight uSpotLight;
uniform vec2 uScreenDimensions;

void main()
{
	vec2 gbufferCoord = gl_FragCoord.xy / uScreenDimensions;
	
	vec3 gbufferDiffuse = texture(uGDiffuse, gbufferCoord).xyz;
	vec3 gbufferNorm = texture(uGNormal, gbufferCoord).xyz;
	vec3 gbufferWorldPos = texture(uGWorldPos, gbufferCoord).xyz;
	float gbufferSpecPower = texture(uGSpecPower, fragTexCoord).x;
	
	vec3 N = normalize(gbufferNorm);
	vec3 L = normalize(uSpotLight.mWorldPos - gbufferWorldPos);

	vec3 Phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		float dist = distance(uSpotLight.mWorldPos, gbufferWorldPos);
		float intensity = smoothstep(uSpotLight.mInnerRadius, uSpotLight.mOuterRadius, dist);
		vec3 DiffuseColor = mix(uSpotLight.mDiffuseColor, vec3(0.0, 0.0, 0.0), intensity);
		vec3 SpecularColor = mix(uSpotLight.mSpecularColor, vec3(0.0, 0.0, 0.0), intensity);
		Phong = DiffuseColor * NdotL + SpecularColor;
	}

	outColor = vec4(gbufferDiffuse * Phong, 1.0);
}