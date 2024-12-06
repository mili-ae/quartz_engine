#version 330

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 directionalLightSpaceTransform; // Point of view of the world from the light. projection * view

void main()
{
    gl_Position = directionalLightSpaceTransform * model * vec4(pos, 1.0);
}