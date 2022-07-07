#version 330

in vec2 texCoords0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;
uniform sampler2D sampler1; 
out vec4 Color;


float normpdf(float x, float sigma)
{
  return 0.39894*exp(-0.5*x*x / (sigma*sigma)) / sigma;
}

void main() { 
vec3 c = texture(sampler1, texCoords0).rgb;
 
const int mSize = 11;
const int kSize = int((float(mSize) - 1.0) / 2.0);
float kernel[mSize];

vec3 final_color = vec3(0.0);
 
// Create the kernel
float sigma = 7.0;
float Z = 0.0;
for (int j = 0; j <= kSize; ++j)
{
    kernel[kSize + j] = kernel[kSize - j] = normpdf(float(j), sigma);
}

//get the normalization factor (as the gaussian has been clamped)
for (int j = 0; j < mSize; ++j)
  {
    Z += kernel[j];
  }
  
  //read out the texels
  for (int i = -kSize; i <= kSize; ++i)
  {
    for (int j = -kSize; j <= kSize; ++j)
    {
      final_color += kernel[kSize + j] * kernel[kSize + i] * texture(sampler1, (texCoords0.xy + vec2(float(i), float(j))/textureSize(sampler1, 0).xy)).rgb;
    }
  }

gl_FragColor= vec4(final_color / (Z*Z), 1.0);
}
