#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 MVP;
uniform mat4 model;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    gl_Position = MVP * vec4(aPos, 1.0);
}