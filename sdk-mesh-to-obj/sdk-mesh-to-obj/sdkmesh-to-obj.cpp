// An open source sdkmesh to obj converter
// supports multiple mesh conversion
// please star and refer my repo if you find it helpful, thanks!
// https://github.com/StanwieCB/sdkmesh-to-obj

/// @author: Hank Zhang

#include "obj.h"
#include "sdkmesh.h"

#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem;

class InputParser {
public:
	InputParser(int &argc, char **argv) 
	{
		for (int i = 1; i < argc; ++i)
			this->args.push_back(std::string(argv[i]));
	}

	const std::string& GetCmdOption(const std::string &option) const 
	{
		std::vector<std::string>::const_iterator itr = std::find(this->args.begin(), this->args.end(), option);
		return *(++itr);
	}

	bool CmdOptionExists(const std::string &option) const 
	{
		std::vector<std::string>::const_iterator itr = std::find(this->args.begin(), this->args.end(), option);
		if (itr != args.end() && ++itr != args.end())
			return true;
		return false;
	}
private:
	std::vector <std::string> args;
};

int WriteOBJ(Sdkmesh& sdkmesh, std::ofstream& output)
{
	/// we assume each mesh is bounded to one index buffer and one vertex buffer
	/// this could be hazardous, if you find any exception or have robust sulotion, please PR or email me

	if ((sdkmesh.GetSdkmeshHeader().NumMeshes != sdkmesh.GetSdkmeshHeader().NumVertexBuffers) ? 1 : 
		(sdkmesh.GetSdkmeshHeader().NumMeshes != sdkmesh.GetSdkmeshHeader().NumIndexBuffers ? 1 :
		 sdkmesh.GetSdkmeshHeader().NumVertexBuffers != sdkmesh.GetSdkmeshHeader().NumIndexBuffers))
	{
		std::cout << "Currently doesn't support sdkmesh with different mesh number/vertex buffer/index buffer" << std::endl;
		return -1;
	}

	const std::vector<std::vector<PosNormalTexTan>> vertexBuffers = sdkmesh.GetSdkmeshVertexBuffer();
	std::vector<std::vector<int>> indexBuffers = sdkmesh.GetSdkmeshIndexBuffer();

	// traverse to encode
	unsigned cnt = 0;
	uint32_t prevIndexNumTotal = 0;
	for (; cnt < sdkmesh.GetSdkmeshHeader().NumMeshes; cnt++)
	{
		const Sdkmesh::SdkmeshMesh& mesh = sdkmesh.GetSdkmeshMesh()[cnt];
		// check buffer number
		if (mesh.VertexBuffers[0] > sdkmesh.GetSdkmeshHeader().NumVertexBuffers)
			return -1;

		if (mesh.NumVertexBuffers > 1)
			return -1;

		//vertices
		std::vector<PosNormalTexTan> vb = vertexBuffers[mesh.VertexBuffers[0]];
		output << "# " << vb.size() << " vertices of mesh " << cnt << "\n\n";

		if (sdkmesh.GetSdkmeshVertexBufferHeader()[cnt].Decl[0].usage == DeclarationUsage::Position)
		{
			for (auto vertex : vb)
				output << "v " << vertex.pos.X << " " << vertex.pos.Y << " " << vertex.pos.Z << "\n";
			output << "\n";
		}

		if (sdkmesh.GetSdkmeshVertexBufferHeader()[cnt].Decl[1].usage == DeclarationUsage::Normal)
		{
			for (auto vertex : vb)
				output << "vn " << vertex.norm.X << " " << vertex.norm.Y << " " << vertex.norm.Z << "\n";
			output << "\n";
		}

		if (sdkmesh.GetSdkmeshVertexBufferHeader()[cnt].Decl[2].usage == DeclarationUsage::TextureCoordinate)
		{
			for (auto vertex : vb)
				output << "vt " << vertex.tex.X << " " << vertex.tex.Y << "\n";
			output << "\n";
		}

		// check buffer number
		if (mesh.IndexBuffer > sdkmesh.GetSdkmeshHeader().NumIndexBuffers)
			return -1;

		// triangle mesh
		std::vector<int> ib = indexBuffers[mesh.IndexBuffer];
		output << "g mesh_" << cnt << "_" << mesh.Name << "\n";

		int numTriangles = ib.size() / 3;
		for (int i = 0; i < numTriangles; i++)
		{
			int fir = ib[3 * i] + 1 + prevIndexNumTotal;
			int sec = ib[3 * i + 1] + 1 + prevIndexNumTotal;
			int thr = ib[3 * i + 2] + 1 + prevIndexNumTotal;
			output << "f " << fir << "/" << fir << "/" << fir << " ";
			output << sec << "/" << sec << "/" << sec << " ";
			output << thr << "/" << thr << "/" << thr << "\n";
		}

		prevIndexNumTotal += sdkmesh.GetSdkemshIndexBufferHeader()[cnt].NumIndices;

		// if (cnt == 2)
		//	break;
	}

	return 0;
}

// exclusively for D3D9 with PNTT_9
int WriteOBJ_9(Sdkmesh& sdkmesh, std::ofstream& output)
{
	/// we assume each mesh is bounded to one index buffer and one vertex buffer
	/// this could be hazardous, if you find any exception or have robust sulotion, please PR or email me

	if ((sdkmesh.GetSdkmeshHeader().NumMeshes != sdkmesh.GetSdkmeshHeader().NumVertexBuffers) ? 1 :
		(sdkmesh.GetSdkmeshHeader().NumMeshes != sdkmesh.GetSdkmeshHeader().NumIndexBuffers ? 1 :
			sdkmesh.GetSdkmeshHeader().NumVertexBuffers != sdkmesh.GetSdkmeshHeader().NumIndexBuffers))
	{
		std::cout << "Currently doesn't support sdkmesh with different mesh number/vertex buffer/index buffer" << std::endl;
		return -1;
	}

	const std::vector<std::vector<PosNormalTexTan_9>> vertexBuffers = sdkmesh.GetSdkmeshVertexBuffer_9();
	std::vector<std::vector<int>> indexBuffers = sdkmesh.GetSdkmeshIndexBuffer();

	// traverse to encode
	unsigned cnt = 0;
	uint32_t prevVertexNumTotal = 0;
	for (; cnt < sdkmesh.GetSdkmeshHeader().NumMeshes; cnt++)
	{
		const Sdkmesh::SdkmeshMesh& mesh = sdkmesh.GetSdkmeshMesh()[cnt];
		// check buffer number
		if (mesh.VertexBuffers[0] > sdkmesh.GetSdkmeshHeader().NumVertexBuffers)
			return -1;

		if (mesh.NumVertexBuffers > 1)
			return -1;

		//vertices
		std::vector<PosNormalTexTan_9> vb = vertexBuffers[mesh.VertexBuffers[0]];
		output << "# " << vb.size() << " vertices of mesh " << cnt << "\n\n";

		for (auto vertex : vb)
			output << "v " << vertex.pos.X << " " << vertex.pos.Y << " " << vertex.pos.Z << "\n";
		output << "\n";

		for (auto vertex : vb)
		{
			// value
			float X, Y, Z;
			X = (float)((vertex.norm.total & Dec3NMaskX) >> 22);
			Y = (float)((vertex.norm.total & Dec3NMaskY) >> 12);
			Z = (float)((vertex.norm.total & Dec3NMaskZ) >> 2);
			// sign bit
			if ((vertex.norm.total & Dec3NMaskXS) >> 31)
				X = -X;
			if ((vertex.norm.total & Dec3NMaskYS) >> 21)
				Y = -Y;
			if ((vertex.norm.total & Dec3NMaskZS) >> 11)
				Z = -Z;
			// normalize [-511.0, 511.0]
			output<< "vn " << (X / 511.0) << " " << (Y / 511.0) << " " << (Z / 511.0) << "\n";
		}
		output << "\n";

		for (auto vertex : vb)
			output << "vt " << (float)vertex.tex.X << " " << (float)vertex.tex.Y << "\n";
		output << "\n";

		// check buffer number
		if (mesh.IndexBuffer > sdkmesh.GetSdkmeshHeader().NumIndexBuffers)
			return -1;

		// triangle mesh
		std::vector<int> ib = indexBuffers[mesh.IndexBuffer];
		output << "g mesh_" << cnt << "_" << mesh.Name << "\n";

		int numTriangles = ib.size() / 3;
		for (int i = 0; i < numTriangles; i++)
		{
			int fir = ib[3 * i] + 1 + prevVertexNumTotal;
			int sec = ib[3 * i + 1] + 1 + prevVertexNumTotal;
			int thr = ib[3 * i + 2] + 1 + prevVertexNumTotal;
			output << "f " << fir << "/" << fir << "/" << fir << " ";
			output << sec << "/" << sec << "/" << sec << " ";
			output << thr << "/" << thr << "/" << thr << "\n";
		}

		prevVertexNumTotal += sdkmesh.GetSdkmeshVertexBufferHeader()[cnt].NumVertices;

		// if (cnt == 2)
		//	break;
	}

	return 0;
}

int WriteMTL(Sdkmesh& sdkmesh, std::ofstream& output)
{
	/// write mtl if sdkmesh has material definition
	// test number
	if (sdkmesh.GetSdkmeshHeader().NumMaterials < 1)
		return 0;

	output << "# Materials for converted sdkmesh\n\n";
	std::vector<Sdkmesh::SdkmeshMaterial> materials = sdkmesh.GetSdkmeshMaterial();
	for (auto mat : materials)
	{
		output << "newmtl " << mat.Name << "\n";

		// brutally convert non-defined attributes
		output << "Ns 10.0000\n";
		output << "Ni 1.5000\n";
		output << "Tr 0.0000\n";
		output << "illum 2\n";

		// hack for squidroom
		output << "Ka 0.5880 0.5880 0.5880\n";
		output << "Kd 0.5880 0.5880 0.5880\n";
		
		// Color
		output << "Ks " << mat.Specular.r << " " << mat.Specular.g << " " << mat.Specular.b << "\n";
		output << "Ks " << mat.Emissive.r << " " << mat.Emissive.g << " " << mat.Emissive.b << "\n";

		fs::path dir = mat.MaterialInstancePath;
		std::string matDstr = mat.DiffuseTexture;
		std::string matSstr = mat.SpecularTexture;
		std::string matNstr = mat.NormalTexture;
		if (!matDstr.empty())
		{
			fs::path matD = (matDstr.substr(0,matDstr.size()-4) + ".png");
			output << "map_Ka " << dir / matD << "\n";
			output << "map_Kd " << dir / matD << "\n";
		}
		if (!matSstr.empty())
		{
			fs::path matS = (matSstr.substr(0, matSstr.size() - 4) + ".png");
			output << "map_Ks " << dir / matS << "\n";
		}
		if (!matNstr.empty())
		{
			fs::path matN = (matNstr.substr(0, matNstr.size() - 4) + ".png");
			output << "norm " << dir / matN << "\n";
			output << "map_bump " << dir / matN << "\n";
		}
		output << "\n";
	}
}

int Convert(const std::string& inputFile, const std::string& outputFile)
{
	std::ifstream input(inputFile, std::ios::binary | std::ios::in);
	if (!input.good())
	{
		std::cout << "Input file does not exist" << std::endl;
		input.close();
		return -1;
	}
	if (outputFile.size() <= 4 || outputFile.substr(outputFile.size() - 4) != ".obj")
	{
		std::cout << "Please speficy an .obj output" << std::endl;
		return -1;
	}
	std::ofstream output_obj(outputFile, std::ios::out | std::ios::binary);
	std::ofstream output_mtl(outputFile.substr(0, outputFile.size() - 4)+".mtl", std::ios::out | std::ios::binary);

	// traverse to count size
	// a bit silly but an accurate way
	std::streampos fileSize = input.tellg();
	input.seekg(0, std::ios::end);
	fileSize = input.tellg() - fileSize;
	input.seekg(0, std::ios::beg);

	Sdkmesh sdkmeshInstane;
	sdkmeshInstane.CreateFromFile_9(input, fileSize);
	sdkmeshInstane.DoCheck();

	std::cout << input.tellg() << std::endl;
	std::cout << fileSize << std::endl;

	// start to dump into .obj output
	if (WriteOBJ_9(sdkmeshInstane, output_obj) == -1)
	{
		std::cout << "Dump into .obj failed" << std::endl;
		input.close();
		output_obj.close();
		output_mtl.close();
		return -1;
	}

	if (WriteMTL(sdkmeshInstane, output_mtl) == -1)
	{
		std::cout << "Dump into .mtl failed" << std::endl;
		input.close();
		output_obj.close();
		output_mtl.close();
		return -1;
	}

	input.close();
	output_obj.close(); 
	output_mtl.close();
	return 0;
}

int main(int argc, char** argv)
{
	InputParser parser = InputParser(argc, argv);
	std::string inputFile;
	std::string outputFile;

	// parse arguments
	if (parser.CmdOptionExists("-i"))
		inputFile = parser.GetCmdOption("-i");
	else
	{
		std::cout << "Empty input file name!" << std::endl;
		return -1;
	}
	if (parser.CmdOptionExists("-o"))
		outputFile = parser.GetCmdOption("-o");
	else
	{
		std::cout << "Empty output file name!" << std::endl;
		return -1;
	}

	std::cout << "Input target: " << inputFile << std::endl;
	std::cout << "Output target: " << outputFile << std::endl;

	// convert
	if (Convert(inputFile, outputFile) == -1)
		return -1;

	std::cout << "Convert " << inputFile << " to " << outputFile << " finished" << std::endl;
	
	return 0;
}