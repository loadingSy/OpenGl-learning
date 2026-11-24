#version 430 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 textCoord;
uniform sampler2D textureId;

void main()
{
    FragColor = texture(textureId, textCoord) * vec4(vertexColor, 1.0);
}