#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texSlotIndex;

out vec4 v_color;
out vec2 v_TexCoord;
out float v_TextSlotIdx;

void main()
{
    gl_Position = position;
    v_color = color;
    v_TexCoord = texCoord;
    v_TextSlotIdx = texSlotIndex;
};

#shader fragment
#version 330 core

in vec4 v_color;
in vec2 v_TexCoord;
in float v_TextSlotIdx;

out vec4 color;

uniform sampler2D u_Textures[2];

void main()
{
    int idx = int(v_TextSlotIdx);
    vec4 texColor = texture(u_Textures[idx], v_TexCoord);
    color = texColor;
};