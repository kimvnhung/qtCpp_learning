#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform mat4 gTranslation;

out vec4 Color;

void main()
{
    gl_Position = gTranslation * vec4(clamp(pos,-1.0,1.0),1.0);
    Color = vec4(clamp(color,0.0,1.0),1.0);
}
