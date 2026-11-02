/* Meta information */
#version 330 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

/* Inirect I/O */
layout(location = 0) uniform vec4 center_n_extent;

/* Direct inputs */
layout(location = 0) in vec2 _in_pos;

/* Direct outputs */
layout(location = 0) out vec2 _out_tex;


/* Code */
void main()
{
  vec2 center = center_n_extent.xy;
  vec2 extent = center_n_extent.zw;
  _out_tex = center + extent * _in_pos;

  gl_Position = vec4(_in_pos, 0.0, 1.0);
}
