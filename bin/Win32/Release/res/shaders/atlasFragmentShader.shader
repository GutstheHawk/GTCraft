#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

//uniform vec2 v_atlasOffsetSize;
//uniform vec2 v_atlasIndexes;

void main()
{

    //float textureRes = 256.0;
    //float halfPixelOffset = 0.5 / textureRes;

    //vec2 offsetTexcoord = (v_TexCoord / v_atlasOffsetSize) + (v_atlasIndexes / v_atlasOffsetSize);
    //offsetTexcoord.x = clamp(offsetTexcoord.x, halfPixelOffset, offsetTexcoord.x - halfPixelOffset);
    //offsetTexcoord.y = clamp(offsetTexcoord.y, halfPixelOffset, offsetTexcoord.y + halfPixelOffset);

    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
}