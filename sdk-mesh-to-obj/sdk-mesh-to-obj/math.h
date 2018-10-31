// math utilities made out of pure covert purpose
// I know it's ugly. If you have interest in improving this file, please send pull request. Much appreciation!

/// @author: Hank Zhang

#ifndef _MATH_H
#define _MATH_H

#include <cstdint>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>

typedef unsigned char byte;
const uint32_t SDKMESH_FILE_VERSION = 101;
const uint32_t MAX_VERTEX_ELEMENTS = 32;
const uint32_t MAX_VERTEX_STREAMS = 16;
const uint32_t MAX_FRAME_NAME = 100;
const uint32_t MAX_MESH_NAME = 100;
const uint32_t MAX_SUBSET_NAME = 100;
const uint32_t MAX_MATERIAL_NAME = 100;
const uint32_t MAX_TEXTURE_NAME = 260;
const uint32_t MAX_MATERIAL_PATH = 260;
const uint32_t INVALID_FRAME = uint32_t(-1);
const uint32_t INVALID_MESH = uint32_t(-1);
const uint32_t INVALID_MATERIAL = uint32_t(-1);
const uint32_t INVALID_SUBSET = uint32_t(-1);
const uint32_t INVALID_ANIMATION_DATA = uint32_t(-1);
const uint32_t INVALID_SAMPLER_SLOT = uint32_t(-1);
const uint32_t ERROR_RESOURCE_VALUE = 1;

enum class DeclarationMethod : byte
{
	Default = 0,
	PartialU = 1,
	PartialV = 2,
	CrossUV = 3,
	UV = 4,
	Lookup = 5,
	LookupPresampled = 6
};

enum class DeclarationType : byte
{
	Float1 = 0,
	Float2 = 1,
	Float3 = 2,
	Float4 = 3,
	Color = 4,
	Ubyte4 = 5,
	Short2 = 6,
	Short4 = 7,
	UByte4N = 8,
	Short2N = 9,
	Short4N = 10,
	UShort2N = 11,
	UShort4N = 12,
	UDec3 = 13,
	Dec3N = 14,
	HalfTwo = 15,
	HalfFour = 16,
	Unused = 17
};

enum class DeclarationUsage : byte
{
	Position = 0,
	BlendWeight = 1,
	BlendIndices = 2,
	Normal = 3,
	PointSize = 4,
	TextureCoordinate = 5,
	Tangent = 6,
	Binormal = 7,
	TessellateFactor = 8,
	PositionTransformed = 9,
	Color = 10,
	Fog = 11,
	Depth = 12,
	Sample = 13
};

struct VertexElement
{
public:
	uint16_t offset;
	uint16_t stream;
	DeclarationType type;
	DeclarationMethod method;
	DeclarationUsage usage;
	byte usage_index;

	/*VertexElement(uint16_t stream, uint16_t offset, DeclarationType declarationType, DeclarationMethod declarationMethod, DeclarationUsage declarationUsage, byte usageIndex)
		: stream(stream), offset(offset), type(declarationType), method(declarationMethod), usage(declarationUsage), usage_index(usageIndex) {}*/
};

class Vec2
{
public:
	float X, Y;
	Vec2(float value) : X(value), Y(value){}
	Vec2(float x, float y) : X(x), Y(y) {}
};

class Vec3
{
public:
	float X, Y, Z;
	Vec3(float value) : X(value), Y(value), Z(value) {}
	Vec3(Vec2 value, float z) : X(value.X), Y(value.Y), Z(z) {}
	Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

class Vec4
{
public:
	float X, Y, Z, A;
};

class Color4
{
public:
	float a=1, b, g, r;
	
	Color4() {}
};

class Mat4
{
public:
	float matrix[4][4];

	Mat4() {}
};

class PosNormalTexTan
{
public:
	Vec3 pos;
	Vec3 norm;
	Vec2 tex;
	Vec3 tan;

	PosNormalTexTan(Vec3 position, Vec3 normal, Vec2 texture, Vec3 tangent)
		: pos(position), norm(normal), tex(texture), tan(tangent) {}
};

#endif