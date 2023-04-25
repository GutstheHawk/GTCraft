#version 460 core
#extension GL_NV_gpu_shader5 : enable

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 texIndex_lightingVal_type;


uniform mat4 pvm;

out vec2 v_TexCoord;
out float passLightingVal;

void main() 
{
    vec3 position = vec3(pos.x, pos.y, pos.z);

    uint texIndex = uint(texIndex_lightingVal_type.x);

    float lightingVal = float(texIndex_lightingVal_type.y);
    passLightingVal = (lightingVal + 1) / 255.0;

    float type = float(texIndex_lightingVal_type.z);

    /*float textureRes = 512.0;
    float halfPixelOffset = (0.5 / textureRes);*/

    //float floatTexID = float(type);

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

    //uint uintTexIndex = uint(texIndex);

    vec2 texCoord = texCoords[texIndex];
    vec2 atlasOffsetSize = vec2(16.0, 16.0);

    v_TexCoord = (texCoord / atlasOffsetSize) + (atlasIndexes / atlasOffsetSize);
    //v_TexCoord = texCoord;

    gl_Position = pvm * vec4(position, 1.0);
}