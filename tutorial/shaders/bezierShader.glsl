#version 130

#define POINT_COLOR vec4(0.75, 0.0, 0.0, 1.0)
#define CURVE_COLOR vec4(0.0, 0.75, 0.0,1.0)

uniform float POINT_RADIUS;

uniform float p1_x;
uniform float p1_y;
uniform float p2_x;
uniform float p2_y;
uniform float p3_x;
uniform float p3_y;
uniform float p4_x;
uniform float p4_y;

void main()
{
    vec2 p1 = vec2(p1_x,p1_y);
    vec2 p2 = vec2(p2_x,p2_y);
    vec2 p3 = vec2(p3_x,p3_y);
    vec2 p4 = vec2(p4_x,p4_y);
   
    if (length(gl_FragCoord.xy - p1) < POINT_RADIUS ||
        length(gl_FragCoord.xy - p2) < POINT_RADIUS ||
        length(gl_FragCoord.xy - p3) < POINT_RADIUS ||
        length(gl_FragCoord.xy - p4) < POINT_RADIUS)
    {
        gl_FragColor = POINT_COLOR;
    }
    else
    {  
    vec2 curvePts[50];
    float t = 50.0f;
    int counter = 0;
    for(float i = 0.; i < 1.0; i += 1./t)
    {
        vec2 q0 = mix(p1, p2, i);
        vec2 q1 = mix(p2, p3, i);
        vec2 q2 = mix(p3, p4, i);

        vec2 r0 = mix(q0, q1, i);
        vec2 r1 = mix(q1, q2, i);

        vec2 p_int = mix(r0, r1, i);
        curvePts[counter] = p_int;
        counter = counter + 1;
     }

     int colored = 0;
     for(int i = 0 ; i < 50 ; i++){
         if(length(gl_FragCoord.xy - curvePts[i]) < POINT_RADIUS)
         {
            gl_FragColor = CURVE_COLOR;
            colored = 1;
            break;
         }
     }
      if(colored == 0)
          gl_FragColor = vec4(0.0);
    }
}
