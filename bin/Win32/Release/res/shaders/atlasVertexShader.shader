#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec2 v_atlasOffsetSize;
uniform vec2 v_atlasIndexes;

out vec2 v_TexCoord;

void main() {

    //vec2 unitVec2;
    //unitVec2.x = 1.0;
    //unitVec2.y = 1.0;

    //vec2 doubleAtlasSize;
    //doubleAtlasSize.x = 512.0;
    //doubleAtlasSize.y = 512.0;

    float textureRes = 512.0;
    float halfPixelOffset = (0.5 / textureRes);

    v_TexCoord = (texCoord / v_atlasOffsetSize) + (v_atlasIndexes / v_atlasOffsetSize);
    //v_TexCoord.x = clamp(v_TexCoord.x, halfPixelOffset, v_TexCoord.x + halfPixelOffset);
    //v_TexCoord.y = clamp(v_TexCoord.y, halfPixelOffset, v_TexCoord.y + halfPixelOffset);

    //v_TexCoord.x -= halfPixelOffset;
    //v_TexCoord.y += halfPixelOffset;

    gl_Position = projection * view * model * position;
}