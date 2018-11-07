#include "sdkmesh.h"
#include <cassert>
#include <iostream>

void Sdkmesh::DoCheck()
{
	// cpp lacks binary standard in class and struct padding
	// so we have to make sure that the size of our headers are exacting the same as the input binary file's
	static_assert(sizeof(VertexElement) == 8, "Decl structure size incorrect");
	static_assert(sizeof(SdkmeshHeader) == 104, "SdkmeshHeader structure size incorrect");
	static_assert(sizeof(SdkmeshVertexBufferHeader) == 288, "SdkmeshVertexBufferHeader structure size incorrect");
	static_assert(sizeof(SdkmeshIndexBufferHeader) == 32, "SdkmeshIndexBufferHeader structure size incorrect");
	static_assert(sizeof(SdkmeshMesh) == 224, "SdkmeshMesh structure size incorrect");
	static_assert(sizeof(SdkmeshSubset) == 144, "SdkmeshSubset structure size incorrect");
	static_assert(sizeof(SdkmeshFrame) == 184, "SdkmeshFrame structure size incorrect");
	static_assert(sizeof(SdkmeshMaterial) == 1256, "SdkmeshMaterial structure size incorrect");

	// not supported for animation
	//static_assert(sizeof(DXUT::SDKANIMATION_FILE_HEADER) == 40, "SDK Mesh structure size incorrect");
	//static_assert(sizeof(DXUT::SDKANIMATION_DATA) == 40, "SDK Mesh structure size incorrect");
	//static_assert(sizeof(DXUT::SDKANIMATION_FRAME_DATA) == 112, "SDK Mesh structure size incorrect");
}

void Sdkmesh::LoadSdkmeshHeader(std::ifstream& inputStream, std::streampos fileSize)
{
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(Sdkmesh::SdkmeshHeader))
		throw std::exception("EOF before reading Sdkmesh_header");

	// deprecated
	/*inputStream.read((char*)&sdkmesh_header.version, sizeof(sdkmesh_header.version));
	inputStream.read((char*)&sdkmesh_header.IsBigEndian, sizeof(sdkmesh_header.IsBigEndian));
	inputStream.seekg(3, std::ios::cur);
	inputStream.read((char*)&sdkmesh_header.HeaderSize, sizeof(sdkmesh_header.HeaderSize));
	inputStream.read((char*)&sdkmesh_header.NonBufferDataSize, sizeof(sdkmesh_header.NonBufferDataSize));
	inputStream.read((char*)&sdkmesh_header.BufferDataSize, sizeof(sdkmesh_header.BufferDataSize));
	inputStream.read((char*)&sdkmesh_header.NumVertexBuffers, sizeof(sdkmesh_header.NumVertexBuffers));
	inputStream.read((char*)&sdkmesh_header.NumIndexBuffers, sizeof(sdkmesh_header.NumIndexBuffers));
	inputStream.read((char*)&sdkmesh_header.NumMeshes, sizeof(sdkmesh_header.NumMeshes));
	inputStream.read((char*)&sdkmesh_header.NumTotalSubsets, sizeof(sdkmesh_header.NumTotalSubsets));
	inputStream.read((char*)&sdkmesh_header.NumFrames, sizeof(sdkmesh_header.NumFrames));
	inputStream.read((char*)&sdkmesh_header.NumMaterials, sizeof(sdkmesh_header.NumMaterials));*/

	inputStream.read((char*)&sdkmesh_header, sizeof(SdkmeshHeader));

	if (sdkmesh_header.version != SDKMESH_FILE_VERSION)
		throw std::exception("Not a supported SDKMESH version");

	if (sdkmesh_header.NumMeshes == 0)
		throw std::exception("No meshes found");

	if (sdkmesh_header.NumVertexBuffers == 0)
		throw std::exception("No vertex buffers found");

	if (sdkmesh_header.NumIndexBuffers == 0)
		throw std::exception("No index buffers found");

	if (sdkmesh_header.NumTotalSubsets == 0)
		throw std::exception("No subsets found");

	if (sdkmesh_header.NumMaterials == 0)
		throw std::exception("No materials found");

}

void Sdkmesh::LoadSdkmeshVertexBufferHeader(std::ifstream& inputStream, std::streampos fileSize)
{
	inputStream.seekg(sdkmesh_header.VertexStreamHeadersOffset, std::ios::beg);
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(SdkmeshVertexBufferHeader))
		throw std::exception("EOF before reading Sdkmesh_vertex_buffer_header");

	unsigned num = sdkmesh_header.NumVertexBuffers;
	sdkmesh_vertex_buffer_headers.resize(num);
	for (unsigned i = 0; i < num; i++)
	{
		inputStream.read((char*)&(sdkmesh_vertex_buffer_headers[i]), sizeof(SdkmeshVertexBufferHeader));
	}

	// vertex elemet structure (input Squidroom): 
	//    usage        type      method      size
	// 1. pos          Float3    default     
	// 2. normal       Dec3N     default     
	// 3. textureCord  HalfTwo   default
	// 4. tangent      Dec3N     default     

}


void Sdkmesh::LoadSdkemshIndexBufferHeader(std::ifstream& inputStream, std::streampos fileSize)
{
	inputStream.seekg(sdkmesh_header.IndexStreamHeadersOffset, std::ios::beg);
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(SdkmeshIndexBufferHeader))
		throw std::exception("EOF before reading Sdkmesh_index_buffer_header");

	unsigned num = sdkmesh_header.NumIndexBuffers;
	sdkmesh_index_buffer_headers.resize(num);
	for (unsigned i = 0; i < num; i++)
	{
		inputStream.read((char*)&(sdkmesh_index_buffer_headers[i]), sizeof(SdkmeshIndexBufferHeader));
	}
}


void Sdkmesh::LoadSdkmeshMesh(std::ifstream& inputStream, std::streampos fileSize)
{
	inputStream.seekg(sdkmesh_header.MeshDataOffset, std::ios::beg);
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(SdkmeshMesh))
		throw std::exception("EOF before reading Sdkmesh_meshes");

	unsigned num = sdkmesh_header.NumMeshes;
	sdkmesh_meshes.resize(num);
	
	for (unsigned i = 0; i < num; i++)
	{
		inputStream.read((char*)&(sdkmesh_meshes[i]), sizeof(SdkmeshMesh));
		//if (sdkmesh_meshes[i].NumSubsets > 1)
		//{
		//	std::cout << "mesh id: " << i << " subsets number: " << sdkmesh_meshes[i].NumSubsets << std::endl;
		//}
	}
}


void Sdkmesh::LoadSdkmeshSubset(std::ifstream& inputStream, std::streampos fileSize)
{
	inputStream.seekg(sdkmesh_header.SubsetDataOffset, std::ios::beg);
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(SdkmeshSubset))
		throw std::exception("EOF before reading Sdkmesh_SdkmeshSubset");

	unsigned num = sdkmesh_header.NumTotalSubsets;
	sdkmesh_subsets.resize(num);

	for (unsigned i = 0; i < num; i++)
	{
		inputStream.read((char*)&(sdkmesh_subsets[i]), sizeof(SdkmeshSubset));
	}
}


void Sdkmesh::LoadSdkmeshFrame(std::ifstream& inputStream, std::streampos fileSize)
{
	inputStream.seekg(sdkmesh_header.FrameDataOffset, std::ios::beg);
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(SdkmeshFrame))
		throw std::exception("EOF before reading Sdkmesh_SdkmeshFrame");

	unsigned num = sdkmesh_header.NumFrames;
	sdkmesh_frames.resize(num);

	for (unsigned i = 0; i < num; i++)
	{
		inputStream.read((char*)&(sdkmesh_frames[i]), sizeof(SdkmeshFrame));
	}
}


void Sdkmesh::LoadSdkmeshMaterial(std::ifstream& inputStream, std::streampos fileSize)
{
	inputStream.seekg(sdkmesh_header.MaterialDataOffset, std::ios::beg);
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(SdkmeshMaterial))
		throw std::exception("EOF before reading Sdkmesh_SdkmeshMaterial");

	unsigned num = sdkmesh_header.NumMaterials;
	sdkmesh_materials.resize(num);

	for (unsigned i = 0; i < num; i++)
	{
		inputStream.read((char*)&(sdkmesh_materials[i]), sizeof(SdkmeshMaterial));
	}
}

void Sdkmesh::LoadSdkmeshSubsetIndexBuffer(std::ifstream& inputStream, std::streampos fileSize)
{
	inputStream.seekg(sdkmesh_header.SubsetDataOffset, std::ios::beg);
	std::streampos pos = inputStream.tellg();
	if (fileSize - pos < sizeof(sdkmesh_header.NumTotalSubsets * sizeof(int)))
		throw std::exception("EOF before reading Sdkmesh_SdkmeshSubsetIndexBuffer");

	subset_index_buffers.resize(sdkmesh_header.NumMeshes);

	unsigned cnt = 0;
	unsigned total = 0;
	for (auto mesh : sdkmesh_meshes)
	{
		subset_index_buffers[cnt].resize(mesh.NumSubsets);
		inputStream.seekg(mesh.SubsetOffset, std::ios::beg);
		for (unsigned i = 0; i < mesh.NumSubsets; i++)
		{
			inputStream.read((char*)&(subset_index_buffers[cnt][i]), sizeof(int));
			total += 1;
		}
		cnt += 1;
	}

	if (total != sdkmesh_header.NumTotalSubsets)
		throw std::exception("Subsets number error");
}

// for D3D9 with Dec3N and HalfTwo exlucsively
void Sdkmesh::LoadSdkmeshVertexBuffer_9(std::ifstream& inputStream, std::streampos fileSize)
{
	// tricky
	uint32_t num_buffer = sdkmesh_header.NumVertexBuffers;

	vertex_buffers_9.resize(num_buffer);
	for (uint32_t i = 0; i < num_buffer; i++)
	{
		// seek corresponding offset
		inputStream.seekg(sdkmesh_vertex_buffer_headers[i].DataOffset, std::ios::beg);
		// don't knwo if we need padding StrideBytes, reserve for later

		uint64_t num_vertices = sdkmesh_vertex_buffer_headers[i].NumVertices;
		if ((uint64_t)inputStream.tellg() + num_vertices * sizeof(PosNormalTexTan_9) > fileSize)
		{
			std::cout << "EOF before reading vertex buffer " << i << std::endl;
			throw std::exception("EOF before reading vertex buffer %d", i);
		}

		std::vector<PosNormalTexTan_9> vertexBuffer;
		vertexBuffer.resize(num_vertices);
		for (uint64_t j = 0; j < num_vertices; j++)
		{
			PosNormalTexTan_9 vertex;
			inputStream.read((char*)&vertex, sizeof(PosNormalTexTan_9));
			vertexBuffer[j] = vertex;
		}
		vertex_buffers_9[i] = vertexBuffer;
	}
}

void Sdkmesh::LoadSdkmeshVertexBuffer(std::ifstream& inputStream, std::streampos fileSize)
{
	// tricky
	uint32_t num_buffer = sdkmesh_header.NumVertexBuffers;

	vertex_buffers.resize(num_buffer);
	for (uint32_t i = 0; i < num_buffer; i++)
	{
		// seek corresponding offset
		inputStream.seekg(sdkmesh_vertex_buffer_headers[i].DataOffset, std::ios::beg);
		// don't knwo if we need padding StrideBytes, reserve for later

		uint64_t num_vertices = sdkmesh_vertex_buffer_headers[i].NumVertices;
		if ((uint64_t)inputStream.tellg() + num_vertices * sizeof(PosNormalTexTan) > fileSize)
		{
			std::cout << "EOF before reading vertex buffer " << i << std::endl;
			throw std::exception("EOF before reading vertex buffer %d", i);
		}

		std::vector<PosNormalTexTan> vertexBuffer;
		vertexBuffer.resize(num_vertices);
		for (uint64_t j = 0; j < num_vertices; j++)
		{
			PosNormalTexTan vertex;
			// usage switch
			unsigned usage_ind = 0;
			while (sdkmesh_vertex_buffer_headers[i].Decl[usage_ind].type != DeclarationType::Unused)
			{
				switch (sdkmesh_vertex_buffer_headers[i].Decl[usage_ind].usage)
				{
				case DeclarationUsage::Position:
					inputStream.read((char*)&vertex.pos, sizeof(Vec3));
					break;
				case DeclarationUsage::Normal:
					inputStream.read((char*)&vertex.norm, sizeof(Vec3));
					break;
				case DeclarationUsage::TextureCoordinate:
					inputStream.read((char*)&vertex.tex, sizeof(HalfTwo));
					break;
				case DeclarationUsage::Tangent:
					inputStream.read((char*)&vertex.tan, sizeof(Vec3));
					break;
				default:
					// other cases to be explored
					break;
				}
				usage_ind++;
			}
			vertexBuffer[j] = vertex;
		}
		vertex_buffers[i] = vertexBuffer;
	}
}

void Sdkmesh::LoadSdkmeshIndexBuffer(std::ifstream& inputStream, std::streampos fileSize)
{
	// tricky
	uint32_t num_buffer = sdkmesh_header.NumIndexBuffers;

	index_buffers.resize(num_buffer);
	for (uint32_t i = 0; i < num_buffer; i++)
	{
		// seek corresponding offset
		inputStream.seekg(sdkmesh_index_buffer_headers[i].DataOffset, std::ios::beg);
		// don't knwo if we need padding StrideBytes, reserve for later

		uint64_t num_indices = sdkmesh_index_buffer_headers[i].NumIndices;
		switch (sdkmesh_index_buffer_headers[i].IndexType)
		{
		case 0:
			if ((uint64_t)inputStream.tellg() + num_indices * sizeof(uint16_t) > (uint64_t)fileSize)
			{
				std::cout << "EOF before reading index buffer " << i << std::endl;
				throw std::exception("EOF before reading index buffer %d", i);
			}
			break;
		default:
			if ((uint64_t)inputStream.tellg() + num_indices * sizeof(uint32_t) > (uint64_t)fileSize)
			{
				std::cout << "EOF before reading index buffer " << i << std::endl;
				throw std::exception("EOF before reading index buffer %d", i);
			}
			break;
		}

		std::vector<int> indexBuffer;
		indexBuffer.resize(num_indices);
		for (uint64_t j = 0; j < num_indices; j++)
		{
			// usage switch
			switch (sdkmesh_index_buffer_headers[i].IndexType)
			{
			case 0:
				inputStream.read((char*)&indexBuffer[j], sizeof(uint16_t));
				break;
			default:
				inputStream.read((char*)&indexBuffer[j], sizeof(uint32_t));
				break;
			}
		}
		index_buffers[i] = indexBuffer;
	}
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
	LoadSdkmeshSubsetIndexBuffer(inputStream, fileSize);

	LoadSdkmeshVertexBuffer(inputStream, fileSize);
	LoadSdkmeshIndexBuffer(inputStream, fileSize);
}

void Sdkmesh::CreateFromFile(std::ifstream& inputStream, std::streampos fileSize)
{
	LoadSdkmeshHeader(inputStream, fileSize);
	LoadSdkmeshVertexBufferHeader(inputStream, fileSize);
	LoadSdkemshIndexBufferHeader(inputStream, fileSize);
	LoadSdkmeshMesh(inputStream, fileSize);
	LoadSdkmeshSubset(inputStream, fileSize);
	LoadSdkmeshFrame(inputStream, fileSize);
	LoadSdkmeshMaterial(inputStream, fileSize);
	LoadSdkmeshSubsetIndexBuffer(inputStream, fileSize);

	LoadSdkmeshVertexBuffer(inputStream, fileSize);
	LoadSdkmeshIndexBuffer(inputStream, fileSize);
}

void Sdkmesh::CreateFromFile_9(std::ifstream& inputStream, std::streampos fileSize)
{
	LoadSdkmeshHeader(inputStream, fileSize);
	LoadSdkmeshVertexBufferHeader(inputStream, fileSize);
	LoadSdkemshIndexBufferHeader(inputStream, fileSize);
	LoadSdkmeshMesh(inputStream, fileSize);
	LoadSdkmeshSubset(inputStream, fileSize);
	LoadSdkmeshFrame(inputStream, fileSize);
	LoadSdkmeshMaterial(inputStream, fileSize);
	LoadSdkmeshSubsetIndexBuffer(inputStream, fileSize);

	LoadSdkmeshVertexBuffer_9(inputStream, fileSize);
	LoadSdkmeshIndexBuffer(inputStream, fileSize);
}