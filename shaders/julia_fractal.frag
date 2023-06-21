#version 460 core

uniform vec2 resolution;
uniform vec2 c;

uniform int bailout;
uniform float limit;

uniform float zoom;

out vec4 outColor;

void main()
{
  vec2 z = (gl_FragCoord.xy / resolution) - vec2(0.5, 0.5);

  z = z / zoom;

  int i;
  for (i = 0; i < bailout; i++)
  {
    if (length(z) > limit) break;
    z = vec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y) + c;
  }

  float value = float(i) / bailout;

  outColor = vec4(value, value, value, 1.0);
}