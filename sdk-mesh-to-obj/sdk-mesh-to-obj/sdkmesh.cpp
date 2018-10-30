#include "sdkmesh.h"

void Sdkmesh::DoCheck()
{
	static_assert(sizeof(VertexElement) == 8, "Decl structure size incorrect");
	static_assert(sizeof(Sdkmesh::SdkmeshHeader) == 104, "SDK Mesh structure size incorrect");
	static_assert(sizeof(Sdkmesh::SdkmeshVertexBufferHeader) == 288, "SDK Mesh structure size incorrect");
	static_assert(sizeof(Sdkmesh::SdkmeshIndexBufferHeader) == 32, "SDK Mesh structure size incorrect");
	static_assert(sizeof(Sdkmesh::SdkmeshMesh) == 224, "SDK Mesh structure size incorrect");
	static_assert(sizeof(Sdkmesh::SdkmeshSubset) == 144, "SDK Mesh structure size incorrect");
	static_assert(sizeof(Sdkmesh::SdkmeshFrame) == 184, "SDK Mesh structure size incorrect");
	static_assert(sizeof(Sdkmesh::SdkmeshMaterial) == 1256, "SDK Mesh structure size incorrect");
	//static_assert(sizeof(DXUT::SDKANIMATION_FILE_HEADER) == 40, "SDK Mesh structure size incorrect");
	//static_assert(sizeof(DXUT::SDKANIMATION_DATA) == 40, "SDK Mesh structure size incorrect");
	//static_assert(sizeof(DXUT::SDKANIMATION_FRAME_DATA) == 112, "SDK Mesh structure size incorrect");
}

void Sdkmesh::LoadSdkmeshHeader(std::ifstream& inputStream, std::streampos fileSize)
{
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(Sdkmesh::SdkmeshHeader))
		throw std::exception("EOF before reading Sdkmesh_header");

	inputStream.read((char*)&sdkmesh_header.version, sizeof(sdkmesh_header.version));
	inputStream.read((char*)&sdkmesh_header.IsBigEndian, sizeof(sdkmesh_header.IsBigEndian));
	inputStream.read((char*)&sdkmesh_header.HeaderSize, sizeof(sdkmesh_header.HeaderSize));
	inputStream.read((char*)&sdkmesh_header.version, sizeof(sdkmesh_header.version));

}

void Sdkmesh::LoadSdkmeshVertexBufferHeader(std::ifstream& inputStream, std::streampos fileSize)
{

}


void Sdkmesh::LoadSdkemshIndexBufferHeader(std::ifstream& inputStream, std::streampos fileSize)
{

}


void Sdkmesh::LoadSdkmeshMesh(std::ifstream& inputStream, std::streampos fileSize)
{

}


void Sdkmesh::LoadSdkmeshSubset(std::ifstream& inputStream, std::streampos fileSize)
{

}


void Sdkmesh::LoadSdkmeshFrame(std::ifstream& inputStream, std::streampos fileSize)
{

}


void Sdkmesh::LoadSdkmeshMaterial(std::ifstream& inputStream, std::streampos fileSize)
{

}

void Sdkmesh::LoadSdkmeshVertexBuffer(std::ifstream& inputStreamm, std::streampos fileSize)
{

}

void Sdkmesh::LoadSdkmeshIndexBuffer(std::ifstream& inputStream, std::streampos fileSize)
{

}

Sdkmesh::Sdkmesh(std::ifstream& inputStream, std::streampos fileSize)
{
	LoadSdkmeshHeader(inputStream, fileSize);
	LoadSdkmeshVertexBufferHeader(inputStream, fileSize);
	LoadSdkemshIndexBufferHeader(inputStream, fileSize);
	LoadSdkmeshMesh(inputStream, fileSize);
	LoadSdkmeshSubset(inputStream, fileSize);
	LoadSdkmeshFrame(inputStream, fileSize);
	LoadSdkmeshMaterial(inputStream, fileSize);

	LoadSdkmeshVertexBuffer(inputStream, fileSize);
	LoadSdkmeshIndexBuffer(inputStream, fileSize);
}

void Sdkmesh::CreateFromFile(std::ifstream& inputStream, std::streampos fileSize)
{

}