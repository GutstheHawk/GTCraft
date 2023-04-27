#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main()
{

	vec4 texColor = texture2D(u_Texture, v_TexCoord);
	color = texColor;

	//vec2 texCoord = v_TexCoord;

	//color = vec4(1.0, 1.0, 1.0, 1.0);

}