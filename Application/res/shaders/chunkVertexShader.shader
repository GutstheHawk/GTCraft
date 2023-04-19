#version 460 core

layout(location = 0) in vec4 coord;

uniform mat4 pvm;

out vec4 v_TexCoord;

void main() 
{

    /*float textureRes = 512.0;
    float halfPixelOffset = (0.5 / textureRes);*/

    //texIndex = coord.w;
    float texIndex = coord.w;

    vec2 texIndexes;

    texIndexes.x = floor(texIndex / 16);
    texIndexes.y = texIndex % 16;


    vec2 altasOffset = vec2(16.0, 16.0);


    v_TexCoord = coord;

    /*v_TexCoord = (texCoord / v_atlasOffsetSize) + (v_atlasIndexes / v_atlasOffsetSize);*/

    gl_Position = pvm * vec4(coord.xyz, 1);
}