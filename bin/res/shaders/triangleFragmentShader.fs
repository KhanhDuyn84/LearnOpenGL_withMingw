#version 330 core

in vec2 ourTexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

out vec4 FragColor;  
  
void main()
{
    vec2 invertY = ourTexCoord;
    invertY.y = -invertY.y;
    FragColor = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, invertY), 0.2);
}