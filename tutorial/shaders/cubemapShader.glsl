#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;
in vec3 lookat; // direction vector representing a 3D texture coordinate
uniform float fogDensity;
uniform int showFog;

uniform vec4 lightColor;
uniform samplerCube sampler1;
uniform vec4 lightDirection;

uniform float sigma;
uniform bool performBlurMotion;




float fogEffect( const float dist) 
{
  const float LOG2 = -1.442695;
  float d = fogDensity * dist;
  return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}
const float PI = 3.14159265;

out vec4 Color;
void main()
{
   vec4 tmpColor = texture(sampler1, lookat);
  if(showFog == 1){
   float fogDistance = gl_FragCoord.z / gl_FragCoord.w;;
   float fogAmount =  fogEffect(fogDistance);
   vec4 fogColor = vec4(0.5,0.6,0.7,0);
   tmpColor = mix(tmpColor, fogColor, fogAmount);
  }
  if(performBlurMotion){
  float blurSize = 5;
  int horizontalPass = 0; 
  vec3 texOffset = vec3(1.0, 1.0, 1.0);
 
  vec2 p = texCoord0.xy;
  float numBlurPixelsPerSide = float(blurSize / 2); 
  
  // Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
  vec3 incrementalGaussian;
  incrementalGaussian.x = 1.0 / (sqrt(2.0 * PI) * sigma);
  incrementalGaussian.y = exp(-0.5 / (sigma * sigma));
  incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

  vec4 avgValue = vec4(0.0, 0.0, 0.0, 0.0);
  float coefficientSum = 0.0;

  // Take the central sample first...
  avgValue +=tmpColor  * incrementalGaussian.x;
  coefficientSum += incrementalGaussian.x;
  incrementalGaussian.xy *= incrementalGaussian.yz;

 for (float i = 1.0; i <= numBlurPixelsPerSide; i++) { 
    avgValue +=  texture(sampler1, lookat - i * texOffset) * incrementalGaussian.x;   
    avgValue += texture(sampler1, lookat + i * texOffset) * incrementalGaussian.x * incrementalGaussian.x;         
    coefficientSum += 2.0 * incrementalGaussian.x;
    incrementalGaussian.xy *= incrementalGaussian.yz;
  }

  Color = avgValue;
  } 
  else {
   Color = tmpColor;
  }

 
}  