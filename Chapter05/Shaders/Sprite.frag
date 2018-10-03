// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

// Request GLSL 3.3
#version 330

// Tex coord input from vertex shader
in vec2 fragTexCoord;
// vertex color input
in vec4 vertexColor;

// This corresponds to the output color to the color buffer
out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

void main()
{
	// Sample color from texture
	vec4 textureColor = texture(uTexture, fragTexCoord);
	vec3 textureRGB = textureColor.rgb;
	float textureAlpha = textureColor.a;

	vec3 vertexRGB = vertexColor.rgb;

	vec3 outRGB = (textureRGB + vertexRGB) / 2;
	outColor = vec4(outRGB.r, outRGB.g, outRGB.b, textureAlpha);
}
