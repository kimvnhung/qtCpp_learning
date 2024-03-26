#version 330 core
in vec2 TextCoord0;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
    FragColor = texture2D(gSampler, TextCoord0);
}
