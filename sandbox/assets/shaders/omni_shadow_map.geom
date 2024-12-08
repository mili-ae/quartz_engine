#version 330

layout (triangles) in; // Type of primitive coming in

// Triangle strip: LIke multiple triangles, but always reuse
//the last 2 points of the last triangle to make a new one.
// All stuck together. Define this as what is coming out, with a max of 18 vertices
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightMatrices[6]; // view and projection matrices of the light source

out vec4 FragPos;

void main()
{
    for (int face = 0; face < 6; face++)
    {
        gl_Layer = face; // Specifies which of the 6 textures of the cubemap we want to output.
        for (int i = 0; i < 3; i++)
        {
            FragPos = gl_in[i].gl_Position; // gl_in is the triangle
            gl_Position = lightMatrices[face] * FragPos; // Get the fragment position in relation to the view from the light
            EmitVertex(); // Emits this vertex at this position on the specified layer
        }

        EndPrimitive(); // End, go to next triangle
    }
}