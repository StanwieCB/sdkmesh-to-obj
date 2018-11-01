// sdkmesh is an exclusive entension for model used in D3D >= 9
// sdkmesh wiki https://github.com/walbourn/contentexporter/wiki/SDKMESH
// sdkmesh class reference https://github.com/Microsoft/DXUT/blob/master/Optional/SDKmesh.cpp
// sdkmesh class reference https://github.com/Microsoft/DXUT/blob/master/Optional/SDKmesh.h
// sdkmesh class reference https://github.com/walbourn/directxtkmodelviewer/blob/master/DirectXTK/Src/ModelLoadSDKMESH.cpp

/// @author: Hank Zhang

#ifndef _SKDMESH_H
#define _SDKMESH_H

#include "math.h"
#include <fstream>

class Sdkmesh
{
private:
	struct SdkmeshHeader
	{
		//basic information
		uint32_t version;
		byte IsBigEndian;
		uint64_t HeaderSize;
		uint64_t NonBufferDataSize;
		uint64_t BufferDataSize;

		//Stats
		uint32_t NumVertexBuffers;
		uint32_t NumIndexBuffers;
		uint32_t NumMeshes;
		uint32_t NumTotalSubsets;
		uint32_t NumFrames;
		uint32_t NumMaterials;

		//Offsets to Data
		uint64_t VertexStreamHeadersOffset;
		uint64_t IndexStreamHeadersOffset;
		uint64_t MeshDataOffset;
		uint64_t SubsetDataOffset;
		uint64_t FrameDataOffset;
		uint64_t MaterialDataOffset;
	};

	struct SdkmeshVertexBufferHeader
	{
		uint64_t NumVertices;
		uint64_t SizeBytes;
		uint64_t StrideBytes;
		VertexElement Decl[MAX_VERTEX_ELEMENTS];
		uint64_t DataOffset;
	};

	struct SdkmeshIndexBufferHeader
	{
		uint64_t NumIndices;
		uint64_t SizeBytes;
		uint32_t IndexType;

		union 
		{
			uint64_t DataOffset;
		};
	};

	struct SdkmeshMesh
	{
		char Name[MAX_MESH_NAME];
		uint8_t NumVertexBuffers;
		uint32_t VertexBuffers[MAX_VERTEX_STREAMS];
		uint32_t IndexBuffer;
		uint32_t NumSubsets;
		uint32_t NumFrameInfluences; //aka bones
		Vec3 BoundingBoxCenter;
		Vec3 BoundingBoxExtents;

		union
		{
			uint64_t SubsetOffset;		
		};
		union
		{
			uint64_t FrameInfluenceOffset;		
		};
	};

	struct SdkmeshSubset
	{
		char Name[MAX_SUBSET_NAME];
		uint32_t MaterialID;
		uint32_t PrimitiveType;
		uint64_t IndexStart;
		uint64_t IndexCount;
		uint64_t VertexStart;
		uint64_t VertexCount;
	};

	struct SdkmeshFrame
	{
		char Name[MAX_FRAME_NAME];
		uint32_t Mesh;
		uint32_t ParentFrame;
		uint32_t ChildFrame;
		uint32_t SiblingFrame;
		Mat4 Matrix;
		uint32_t AnimationDataIndex;		//Used to index which set of keyframes transforms this frame
	};

	struct SdkmeshMaterial
	{
		char    Name[MAX_MATERIAL_NAME];
		// Use MaterialInstancePath
		char    MaterialInstancePath[MAX_MATERIAL_PATH];
		// Or fall back to d3d8-type materials
		char    DiffuseTexture[MAX_TEXTURE_NAME];
		char    NormalTexture[MAX_TEXTURE_NAME];
		char    SpecularTexture[MAX_TEXTURE_NAME];

		Color4 Diffuse;
		Color4 Ambient;
		Color4 Specular;
		Color4 Emissive;
		float Power;

		// force padding
		union
		{
			uint64_t Force64_1;			//Force the union to 64bits
		};
		union
		{
			uint64_t Force64_2;			//Force the union to 64bits
		};
		union
		{
			uint64_t Force64_3;			//Force the union to 64bits
		};
		union
		{
			uint64_t Force64_4;			//Force the union to 64bits
		};
		union
		{
			uint64_t Force64_5;		    //Force the union to 64bits
		};
		union
		{
			uint64_t Force64_6;			//Force the union to 64bits
		};
	};

	SdkmeshHeader sdkmesh_header;
	std::vector<SdkmeshVertexBufferHeader> sdkmesh_vertex_buffer_headers;
	std::vector<SdkmeshIndexBufferHeader> sdkmesh_index_buffer_headers;
	std::vector<SdkmeshMesh> sdkmesh_meshes;
	std::vector<SdkmeshSubset> sdkmesh_subsets;
	std::vector<SdkmeshFrame> sdkmesh_frames;
	std::vector<SdkmeshMaterial> sdkmesh_materials;

	std::vector<std::vector<PosNormalTexTan>> vertex_buffers;
	std::vector<std::vector<int>> index_buffers;

public:
	Sdkmesh() { DoCheck(); }
	Sdkmesh(std::ifstream& inputStream, std::streampos fileSize);

	~Sdkmesh() {};

	void DoCheck();

	void CreateFromFile(std::ifstream& inputStream, std::streampos fileSize);

	void LoadSdkmeshHeader(std::ifstream& inputStream, std::streampos fileSize);
	void LoadSdkmeshVertexBufferHeader(std::ifstream& inputStream, std::streampos fileSize);
	void LoadSdkemshIndexBufferHeader(std::ifstream& inputStream, std::streampos fileSize);
	void LoadSdkmeshMesh(std::ifstream& inputStream, std::streampos fileSize);
	void LoadSdkmeshSubset(std::ifstream& inputStream, std::streampos fileSize);
	void LoadSdkmeshFrame(std::ifstream& inputStream, std::streampos fileSize);
	void LoadSdkmeshMaterial(std::ifstream& inputStream, std::streampos fileSize);
	
	void LoadSdkmeshVertexBuffer(std::ifstream& inputStream, std::streampos fileSize);
	void LoadSdkmeshIndexBuffer(std::ifstream& inputStream, std::streampos fileSize);
};

#endif 