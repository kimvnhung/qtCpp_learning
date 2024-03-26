#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 0) in vec2 TextCoord;

uniform mat4 gWVP;

out vec2 TextCoord0;

void main()
{
    gl_Position = gWVP * vec4(Position, 1.0);
    TextCoord0 = TextCoord;
}
