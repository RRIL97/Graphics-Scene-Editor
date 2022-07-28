#version 330

in vec2 texCoords0;
in vec3 normal0;
in vec3 position0;
uniform float glowIntensity;
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




void main() { 
    float textureW = 256 ;
	float textureH = 256;

	float radiusSize = 50.0;
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
		Color =  accumulatedColor / totalWeight*glowIntensity;

}