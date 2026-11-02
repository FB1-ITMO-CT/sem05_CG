/* Meta information */
#version 330 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable


/* Inirect I/O */
layout(location = 0) uniform sampler2D tex_2d;
layout(location = 1) uniform vec2 aspect_ratio;
layout(location = 2) uniform vec4 color;

/* Direct inputs */
layout(location = 0) in vec2 _in_pos;

/* Direct outputs */
layout(location = 0) out vec4 _out_col;


/* Code */
void main()
{
  vec2 vert_tex = (_in_pos + 1) / 2;
  vec3 vert_col = vec3(0.5, 0.5, 0.5);

  // TODO: Replace with fractal
  vec4 texel = texture(tex_2d, vec2(vert_tex.x, 1.0 - vert_tex.y));
  float greyscale_factor = dot(texel.rgb, vec3(0.21, 0.71, 0.07));
  vec3 semi_greyscale = mix(vec3(greyscale_factor), vert_col.rgb, 0.7f);
  vec3 animated_color = mix(color.rgb, semi_greyscale, 0.7f);

  _out_col = vec4(animated_color, 1.0);
}
