#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() 
{
	//int texIndex = texcoord.w;
	//color = vec4(texcoord.w / 128.0, texcoord.w / 256.0, texcoord.w / 512.0, 1.0);
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;

	//vec4 texColor = texture(u_Texture, v_TexCoord);
	//color = texColor;
}