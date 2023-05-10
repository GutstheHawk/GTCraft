#version 460 core

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 texIndex_type;

out vec2 v_TexCoord;

void main()
{
    vec3 position = vec3(pos.x, pos.y, 0.0);

    uint texIndex = uint(texIndex_type.x);
    float type = float(texIndex_type.y);

    float texIDX = mod(type, 16.0) - 1.0;
    float texIDY = floor(type / 16.0);

    vec2 atlasIndexes = vec2(texIDX, texIDY);

    vec2 texCoords[4] =
    {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    vec2 texCoord = texCoords[texIndex];
    vec2 atlasOffsetSize = vec2(16.0, 16.0);

    v_TexCoord = (texCoord / atlasOffsetSize) + (atlasIndexes / atlasOffsetSize);
    //v_TexCoord = texCoord;

    gl_Position = vec4(position, 1.0);
}