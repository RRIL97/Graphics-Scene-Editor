#version 330

//uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform bool isBloomOn;
uniform float glowIntensity;

in vec2 texCoords0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;


uniform float weight[5] = float[] (0.0227027, 0.01945946, 0.01216216, 0.054054, 0.016216);

out vec4 Color;
void main()
{
	Color = texture2D(sampler1, texCoords0) ; //you must have gl_FragColor
	if(isBloomOn){
	 vec2 tex_offset = 1.0 / textureSize(sampler1, 0); // gets size of single texel
     vec3 bloomBlur = texture(sampler1, texCoords0).rgb * weight[0]; // current fragment's contribution

        for(int i = 1; i < 5; ++i)
        {
            bloomBlur += texture(sampler1, texCoords0 + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            bloomBlur += texture(sampler1, texCoords0 - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    vec3 hdrColor = texture(sampler1, texCoords0).rgb;      
    hdrColor += bloomBlur; // additive blending
	Color = vec4(hdrColor, 1.0);
	}
}
