#pragma once

typedef glm::tvec3<GLubyte> ubyte3;

#pragma pack(push, 1)
struct Vertex {
	ubyte3 Position;
	ubyte3 texIndex_lightingVal_type;
};
#pragma pack(pop)

static void genNegXSide(Vertex* vertexes, uint8_t* x, uint8_t* y, uint8_t* z, uint8_t type, int* i)
{
	uint8_t texIndex = 0;
	uint8_t lightingVal = 0;

	Vertex v0 = {};
	v0.Position = ubyte3(*x, *y, *z);
	texIndex = { 1 };
	lightingVal = { 153 };
	v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v0;

	Vertex v1 = {};
	v1.Position = ubyte3(*x, *y, *z + 1);
	texIndex = { 0 };
	lightingVal = { 153 };
	v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v1;

	Vertex v2 = {};
	v2.Position = ubyte3(*x, *y + 1, *z);
	texIndex = { 2 };
	lightingVal = { 153 };
	v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v2;

	Vertex v3 = {};
	v3.Position = ubyte3(*x, *y + 1, *z);
	texIndex = { 2 };
	lightingVal = { 153 };
	v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v3;

	Vertex v4 = {};
	v4.Position = ubyte3(*x, *y, *z + 1);
	texIndex = { 0 };
	lightingVal = { 153 };
	v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v4;

	Vertex v5 = {};
	v5.Position = ubyte3(*x, *y + 1, *z + 1);
	texIndex = { 3 };
	lightingVal = { 153 };
	v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v5;
}

static void genPosXSide(Vertex* vertexes, uint8_t* x, uint8_t* y, uint8_t* z, uint8_t type, int* i)
{
	uint8_t texIndex = 0;
	uint8_t lightingVal = 0;

	Vertex v0 = {};
	v0.Position = ubyte3(*x + 1, *y, *z);
	texIndex = { 0 };
	lightingVal = { 153 };
	v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v0;

	Vertex v1 = {};
	v1.Position = ubyte3(*x + 1, *y + 1, *z);
	texIndex = { 3 };
	lightingVal = { 153 };
	v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v1;

	Vertex v2 = {};
	v2.Position = ubyte3(*x + 1, *y, *z + 1);
	texIndex = { 1 };
	lightingVal = { 153 };
	v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v2;

	Vertex v3 = {};
	v3.Position = ubyte3(*x + 1, *y + 1, *z);
	texIndex = { 3 };
	lightingVal = { 153 };
	v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v3;

	Vertex v4 = {};
	v4.Position = ubyte3(*x + 1, *y + 1, *z + 1);
	texIndex = { 2 };
	lightingVal = { 153 };
	v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v4;

	Vertex v5 = {};
	v5.Position = ubyte3(*x + 1, *y, *z + 1);
	texIndex = { 1 };
	lightingVal = { 153 };
	v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v5;
}

static void genNegYSide(Vertex* vertexes, uint8_t* x, uint8_t* y, uint8_t* z, uint8_t type, int* i)
{
	uint8_t texIndex = 0;
	uint8_t lightingVal = 0;

	Vertex v0 = {};
	v0.Position = ubyte3(*x, *y, *z);
	texIndex = { 0 };
	lightingVal = { 102 };
	v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v0;

	Vertex v1 = {};
	v1.Position = ubyte3(*x + 1, *y, *z);
	texIndex = { 3 };
	lightingVal = { 102 };
	v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v1;

	Vertex v2 = {};
	v2.Position = ubyte3(*x, *y, *z + 1);
	texIndex = { 1 };
	lightingVal = { 102 };
	v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v2;

	Vertex v3 = {};
	v3.Position = ubyte3(*x + 1, *y, *z);
	texIndex = { 3 };
	lightingVal = { 102 };
	v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v3;

	Vertex v4 = {};
	v4.Position = ubyte3(*x + 1, *y, *z + 1);
	texIndex = { 2 };
	lightingVal = { 102 };
	v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v4;

	Vertex v5 = {};
	v5.Position = ubyte3(*x, *y, *z + 1);
	texIndex = { 1 };
	lightingVal = { 102 };
	v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v5;
}

static void genPosYSide(Vertex* vertexes, uint8_t* x, uint8_t* y, uint8_t* z, uint8_t type, int* i)
{
	uint8_t texIndex = 0;
	uint8_t lightingVal = 0;

	Vertex v0 = {};
	v0.Position = ubyte3(*x, *y + 1, *z);
	texIndex = { 0 };
	lightingVal = { 254 };
	v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v0;

	Vertex v1 = {};
	v1.Position = ubyte3(*x, *y + 1, *z + 1);
	texIndex = { 1 };
	lightingVal = { 254 };
	v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v1;

	Vertex v2 = {};
	v2.Position = ubyte3(*x + 1, *y + 1, *z);
	texIndex = { 3 };
	lightingVal = { 254 };
	v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v2;

	Vertex v3 = {};
	v3.Position = ubyte3(*x, *y + 1, *z + 1);
	texIndex = { 1 };
	lightingVal = { 254 };
	v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v3;

	Vertex v4 = {};
	v4.Position = ubyte3(*x + 1, *y + 1, *z + 1);
	texIndex = { 2 };
	lightingVal = { 254 };
	v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v4;

	Vertex v5 = {};
	v5.Position = ubyte3(*x + 1, *y + 1, *z);
	texIndex = { 3 };
	lightingVal = { 254 };
	v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v5;
}

static void genNegZSide(Vertex* vertexes, uint8_t* x, uint8_t* y, uint8_t* z, uint8_t type, int* i)
{
	uint8_t texIndex = 0;
	uint8_t lightingVal = 0;

	Vertex v0 = {};
	v0.Position = ubyte3(*x, *y, *z);
	texIndex = { 0 };
	lightingVal = { 203 };
	v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v0;

	Vertex v1 = {};
	v1.Position = ubyte3(*x, *y + 1, *z);
	texIndex = { 3 };
	lightingVal = { 203 };
	v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v1;

	Vertex v2 = {};
	v2.Position = ubyte3(*x + 1, *y, *z);
	texIndex = { 1 };
	lightingVal = { 203 };
	v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v2;

	Vertex v3 = {};
	v3.Position = ubyte3(*x, *y + 1, *z);
	texIndex = { 3 };
	lightingVal = { 203 };
	v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v3;

	Vertex v4 = {};
	v4.Position = ubyte3(*x + 1, *y + 1, *z);
	texIndex = { 2 };
	lightingVal = { 203 };
	v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v4;

	Vertex v5 = {};
	v5.Position = ubyte3(*x + 1, *y, *z);
	texIndex = { 1 };
	lightingVal = { 203 };
	v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v5;
}

static void genPosZSide(Vertex* vertexes, uint8_t* x, uint8_t* y, uint8_t* z, uint8_t type, int* i)
{
	uint8_t texIndex = 0;
	uint8_t lightingVal = 0;

	Vertex v0 = {};
	v0.Position = ubyte3(*x, *y, *z + 1);
	texIndex = { 0 };
	lightingVal = { 203 };
	v0.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v0;

	Vertex v1 = {};
	v1.Position = ubyte3(*x + 1, *y, *z + 1);
	texIndex = { 1 };
	lightingVal = { 203 };
	v1.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v1;

	Vertex v2 = {};
	v2.Position = ubyte3(*x, *y + 1, *z + 1);
	texIndex = { 3 };
	lightingVal = { 203 };
	v2.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v2;

	Vertex v3 = {};
	v3.Position = ubyte3(*x, *y + 1, *z + 1);
	texIndex = { 3 };
	lightingVal = { 203 };
	v3.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v3;

	Vertex v4 = {};
	v4.Position = ubyte3(*x + 1, *y, *z + 1);
	texIndex = { 1 };
	lightingVal = { 203 };
	v4.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v4;

	Vertex v5 = {};
	v5.Position = ubyte3(*x + 1, *y + 1, *z + 1);
	texIndex = { 2 };
	lightingVal = { 203 };
	v5.texIndex_lightingVal_type = ubyte3(texIndex, lightingVal, type);
	vertexes[(*i)++] = v5;
}