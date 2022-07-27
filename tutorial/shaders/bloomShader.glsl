#version 330

in vec2 texCoords0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;
uniform sampler2D sampler1;  
out vec4 Color;
  



float gaussianFunction(float x)
{
	float variance = 0.15; //x should be 0-1.0 with this variance

	float alpha = -(x*x / (2.0*variance));
	return exp(alpha);
}

float gaussianFunction2D(float x, float y)
{
	float variance = 0.25; //x and y should be 0-1.0 with this variance

	float alpha = -( (x*x+y*y) / (2.0*variance));
	return exp(alpha);
}

mat4 brightnessMatrix( float brightness )
{
    return mat4( 1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 brightness, brightness, brightness, 1 );
}

mat4 contrastMatrix( float contrast )
{
	float t = ( 1.0 - contrast ) / 2.0;
    
    return mat4( contrast, 0, 0, 0,
                 0, contrast, 0, 0,
                 0, 0, contrast, 0,
                 t, t, t, 1 );

}

mat4 saturationMatrix( float saturation )
{
    vec3 luminance = vec3( 0.3086, 0.6094, 0.0820 );
    
    float oneMinusSat = 1.0 - saturation;
    
    vec3 red = vec3( luminance.x * oneMinusSat );
    red+= vec3( saturation, 0, 0 );
    
    vec3 green = vec3( luminance.y * oneMinusSat );
    green += vec3( 0, saturation, 0 );
    
    vec3 blue = vec3( luminance.z * oneMinusSat );
    blue += vec3( 0, 0, saturation );
    
    return mat4( red,     0,
                 green,   0,
                 blue,    0,
                 0, 0, 0, 1 );
}


void main() { 
         vec4 colorTemp =  texture2D(sampler1, texCoords0)* vec4(color0,1.0);
 
             float textureW = 256 ;
	float textureH = 256;

	float radiusSize = 100.0;
	float rSkip = 4.0;
	float totalWeight = 0.0; 

	vec4 accumulatedColor;
	float u = texCoords0.x;
              float y;

		
		for(y=-radiusSize; y<=radiusSize; y+=rSkip)
		{
			float v = texCoords0.y + y/textureH;
			
			if(v>=0.0 && v<=1.0)
			{
				float weight = gaussianFunction(y/radiusSize);
				accumulatedColor += texture2D(sampler1, vec2(u,v)) * weight;
				totalWeight += weight;
			}
		}


             float v = texCoords0.y;
		float x;
		
		for(x=-radiusSize; x<=radiusSize; x+=rSkip)
		{
			float u = texCoords0.x + x/textureW;
			
			if(u>=0.0 && u<=1.0)
			{
				float weight = gaussianFunction(x/radiusSize);
				accumulatedColor += texture2D(sampler1, vec2(u,v)) * weight;
				totalWeight += weight;
			}
		}
const float brightness = 0.8;
const float contrast = 1.5;
const float saturation = 1.2;
		
vec4 tmpColor =  accumulatedColor / totalWeight;
 
Color=  brightnessMatrix( brightness ) *
        		contrastMatrix( contrast ) * 
        		saturationMatrix( saturation ) *
        		tmpColor;
}