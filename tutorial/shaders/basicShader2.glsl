#version 330

//uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform sampler2D sampler2;


in vec2 texCoords0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;



out vec4 Color;
void main()
{
	Color = texture2D(sampler1, texCoords0) ; //you must have gl_FragColor
}
