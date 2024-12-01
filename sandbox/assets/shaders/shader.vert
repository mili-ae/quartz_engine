#version 330

layout (location = 0) in vec3 pos; // where the vertex is in relation to the origin
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm; // normal -> direction from the position. the direction perpendicular to the surface of an object

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    TexCoord = tex;

    // Remove transform by converting to mat3
    // fix scaling and rotation by inversing it and transpose (non uniform scaling) - normals start pointing upwards if you don't
    Normal = mat3(transpose(inverse(model))) * norm;

    FragPos = (model * vec4(pos, 1.0)).xyz;
}