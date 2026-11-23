#version 430 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 textCoord;
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, textCoord) * vec4(vertexColor, 1.0);
}