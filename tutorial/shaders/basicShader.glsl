#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 color1;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler1;
uniform vec4 lightDirection;
uniform float sigma;
uniform bool performBlurMotion;

out vec4 Color;
float normpdf(float x)
{
  return 0.39894*exp(-0.5*x*x / (sigma*sigma)) / sigma;
}
void main()
{
 if(!performBlurMotion || sigma == 1.0f)
     Color = texture2D(sampler1, texCoord0)* vec4(color0,1.0); //you must have gl_FragColor
else{
  vec3 c = texture(sampler1, texCoord0).rgb;
 
  const int mSize = 50;
  const int kSize = int((float(mSize) - 1.0) / 2.0);
   float kernel[mSize];

  vec3 final_color = vec3(0.0);
  // Create the kernel
  float Z = 0.0;
  for (int j = 0; j <= kSize; ++j)
  {
    kernel[kSize + j] = kernel[kSize - j] = normpdf(float(j));
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
      final_color += kernel[kSize + j] * kernel[kSize + i] * texture(sampler1, (texCoord0.xy + vec2(float(i), float(j))/textureSize(sampler1, 0).xy)).rgb ;
    }
  }

Color= vec4(final_color*color1 / (Z*Z), 1.0);
}

}
