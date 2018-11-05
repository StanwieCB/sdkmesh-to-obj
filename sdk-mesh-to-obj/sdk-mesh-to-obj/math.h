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

#include "umHalf.h"

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

#define Dec3NMaskX 0x7fc00000
#define Dec3NMaskY 0x001ff000
#define Dec3NMaskZ 0x000007fc
#define Dec3NMaskXS 0x80000000
#define Dec3NMaskYS 0x00200000
#define Dec3NMaskZS 0x00000800

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

// https://docs.microsoft.com/en-us/windows/desktop/direct3d9/d3dvertexelement9
struct VertexElement
{
public:
	uint16_t stream;
	uint16_t offset;
	DeclarationType type;
	DeclarationMethod method;
	DeclarationUsage usage;
	byte usage_index;

	/*VertexElement(uint16_t stream, uint16_t offset, DeclarationType declarationType, DeclarationMethod declarationMethod, DeclarationUsage declarationUsage, byte usageIndex)
		: stream(stream), offset(offset), type(declarationType), method(declarationMethod), usage(declarationUsage), usage_index(usageIndex) {}*/
};

class HalfTwo
{
public:
	HalfFloat X;
	HalfFloat Y;
	HalfTwo() { X = 0.0, Y = 0.0; }
};

class Dec3
{
public:
	// X  Y  Z  w
	// 10 10 10 2  bit long
	uint32_t total;
	Dec3() : total(0) {};
};

class Vec2
{
public:
	float X, Y;
	Vec2() : X(0), Y(0) {}
	Vec2(float value) : X(value), Y(value){}
	Vec2(float x, float y) : X(x), Y(y) {}
};

class Vec3
{
public:
	float X, Y, Z;
	Vec3() : X(0), Y(0), Z(0) {}
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

	PosNormalTexTan() {};
	PosNormalTexTan(Vec3 position, Vec3 normal, Vec2 texture, Vec3 tangent)
		: pos(position), norm(normal), tex(texture), tan(tangent) {}
};


// Dec3N and HalfTwo are really tricky to decode
class PosNormalTexTan_9
{
public:
	Vec3 pos;
	Dec3 norm;
	HalfTwo tex;
	Dec3 tan;

	PosNormalTexTan_9() {};
	PosNormalTexTan_9(Vec3 position, Dec3 normal, HalfTwo texture, Dec3 tangent)
		: pos(position), norm(normal), tex(texture), tan(tangent) {}
};


#endif