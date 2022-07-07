#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;
in vec3 lookat; // direction vector representing a 3D texture coordinate

uniform vec4 lightColor;
uniform samplerCube sampler1;
uniform vec4 lightDirection;



float fogEffect(
  const float dist,
  const float density
) {
  const float LOG2 = -1.442695;
  float d = density * dist;
  return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}

out vec4 Color;
void main()
{
   vec4 tmpColor = texture(sampler1, lookat);

  float fogDistance = gl_FragCoord.z / gl_FragCoord.w;;
  float fogAmount =  fogEffect(fogDistance, 0.008);
   vec4 fogColor = vec4(1,0,0,0);
  Color = mix(tmpColor, fogColor, fogAmount);
}  