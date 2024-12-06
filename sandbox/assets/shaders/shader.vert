#version 330

layout (location = 0) in vec3 pos; // where the vertex is in relation to the origin
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm; // normal -> direction from the position. the direction perpendicular to the surface of an object

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionalLightSpacePos; // Position of where the fragment is relative to the light

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightSpaceTransform;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
    DirectionalLightSpacePos = directionalLightSpaceTransform * model * vec4(pos, 1.0);
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

    TexCoord = tex;

    // Model to multiply because if the object moves around, the normal has to move to.
    // Normal is in relation to where the model is, but the normal doesn't really change
    // direction unles we rotate or scale an object in one direction. Then, the normal
    // will change and have to be recalculated. Because rotation and scale happens in
    // the first 3x3 matrix inside the mat4, we convert the mat4 to a mat3 to only keep
    // rotation and sclae values. The rotation changes the norm in a way we want to, but
    // the scaling changes the normal in a way we don't want to so we have to avoid scaling
    // while taking in rotation. The trick to do that is to transpose and iunverse the matrix,
    // this reverses the scaling
    Normal = mat3(transpose(inverse(model))) * norm;

    // Gives the value where it is in the world without taking into care the "view" or camera
    FragPos = (model * vec4(pos, 1.0)).xyz;
}