/* Meta information */
#version 330 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_uniform_location : enable


/* Inirect I/O */
layout(location = 0) uniform vec4 center_n_extent;
layout(location = 1) uniform vec3 frac_params;
layout(location = 2) uniform vec2 texel_size;

/* Direct inputs */
layout(location = 0) in vec2 _in_tex;

/* Direct outputs */
layout(location = 0) out vec4 _out_col;


/* Code */
void main()
{
  vec2 center = center_n_extent.xy;
  vec2 extent = center_n_extent.zw;

  int max_iter = 4096;
  float a = frac_params.x, b = frac_params.y, c = frac_params.z;
  float texel = sqrt(max(texel_size.x, texel_size.y));

  vec2 base_pos = _in_tex;
  vec2 coord = vec2(0, 0);

  float hits = 0;
  float far_hits = 0;

  for (int i = 0; i < max_iter; i++)
  {
    coord = vec2(coord.y - sign(coord.x) * sqrt(abs(b * coord.x - c)), a - coord.x);

    float dist = distance(coord, base_pos) / texel / 3;
    vec2 far_dist = abs(coord - center);

    if ((far_dist.x < extent.x && far_dist.y < extent.y) || dist < 8)
      far_hits += 1;

    if (dist < 1)
      hits += 1;
    else if (dist < 3)
    {
      dist = (dist - 1) / 2;
      hits += pow(1 - dist, 2);
    }
  }

  _out_col = vec4(pow(hits / (far_hits + 1), 0.5), pow(hits / max_iter, 0.3), hits * 16 / max_iter, 1.0);
}
