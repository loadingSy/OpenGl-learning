#version 430 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 textCoord;
uniform sampler2D textureId;
uniform bool hasImage;

uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.4;
    vec3 resultColor = ambientStrength * lightColor * vertexColor;
    if(hasImage)
    {
        FragColor = texture(textureId, textCoord) * vec4(resultColor, 1);
    }
    else
    {
        FragColor = vec4(resultColor, 1);
    }
}