/* Meta information */
#version 330 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable

/* Direct inputs */
layout(location = 0) in vec2 _in_pos;

/* Direct outputs */
layout(location = 0) out vec2 _out_pos;


/* Code */
void main()
{
  _out_pos = _in_pos;
  gl_Position = vec4(_in_pos, 0.0, 1.0);
}
