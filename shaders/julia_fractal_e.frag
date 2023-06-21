#version 460 core

uniform vec2 resolution;
uniform vec2 c;

uniform int bailout;
uniform float limit;

uniform float zoom;
uniform vec2 translation;

out vec4 outColor;

void main()
{
  vec2 z = (gl_FragCoord.xy / resolution) - vec2(0.5, 0.5); // z values from -0.5 to 0.5

  z = z * 4 / zoom; // z values from -2.0 to 2.0 multiplied by zoom
  z = z + translation; // z values plus translation applied

  int i;
  float l;
  for (i = 0; i < bailout; i++)
  {
    l = length(z);
    if (l > limit) break;
    float e = exp(z.x);
    z = vec2(z.x * cos(z.y) - z.y * sin(z.y), z.x * sin(z.y) + z.y * cos(z.y)) * e + c;
  }

  float value = 1.0 - (float(i) / bailout);
  
  outColor = vec4(1.0, value, value, 1.0);
}