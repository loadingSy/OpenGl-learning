#version 430 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 textCoord;
uniform sampler2D textureId;
uniform bool hasImage;

void main()
{
    if(hasImage)
    {
        FragColor = texture(textureId, textCoord) * vec4(vertexColor, 1.0);
    }
    else
    {
        FragColor = vec4(vertexColor, 1.0);
    }
}