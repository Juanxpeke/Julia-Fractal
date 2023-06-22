#version 460 core

uniform vec2 resolution;
uniform vec2 c;

uniform int bailout;
uniform float limit;

uniform float color;
uniform float zoom;
uniform vec2 translation;

out vec4 outColor;

float mod(float a, int b)
{
  return a - int(a) + int(a) % b;
}

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
  
  // Color logic
  float H = mod(value * 5.5 + color, 6);
  float S = value;
  float L = value;
  float C = (1.0 - abs(2 * L - 1)) * S;
  float X = C * (1.0 - abs(mod(H, 2) - 1.0));
  float m = L - C / 2.0;

  vec3 rgb;
  if (H < 1) { rgb = vec3(C, X, 0.0); }
  else if (H < 2) { rgb = vec3(X, C, 0.0); }
  else if (H < 3) { rgb = vec3(0.0, C, X); }
  else if (H < 4) { rgb = vec3(0.0, X, C); }
  else if (H < 5) { rgb = vec3(X, 0.0, C); }
  else { rgb = vec3(C, 0.0, X); }

  rgb = rgb + vec3(m, m, m);

  outColor = vec4(rgb, 1.0);
}