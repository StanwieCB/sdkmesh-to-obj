// An open source sdkmesh to obj converter
// supports multiple mesh conversion
// please star and refer my repo if you find it helpful, thanks!
// https://github.com/StanwieCB/sdkmesh-to-obj

/// @author: Hank Zhang

#include "obj.h"
#include "sdkmesh.h"

#include <iostream>

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
	/// this could be hazardous, if you find any exception or have robust sulution, please PR or email me

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
	uint64_t prevIndexNumTotal = 0;
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
	std::ofstream output(outputFile, std::ios::out | std::ios::binary);

	// traverse to count size
	// a bit silly but an accurate way
	std::streampos fileSize = input.tellg();
	input.seekg(0, std::ios::end);
	fileSize = input.tellg() - fileSize;
	input.seekg(0, std::ios::beg);

	Sdkmesh sdkmeshInstane(input, fileSize);
	sdkmeshInstane.DoCheck();

	std::cout << input.tellg() << std::endl;
	std::cout << fileSize << std::endl;

	// start to dump into .obj output
	if (WriteOBJ(sdkmeshInstane, output) == -1)
	{
		std::cout << "Dump into .obj failed" << std::endl;
		input.close();
		output.close();
	}

	input.close();
	output.close();
	return 0;
}

int main(int argc, char** argv)
{
	InputParser parser = InputParser(argc, argv);
	std::string inputFile;
	std::string outputFile;

	Sdkmesh test1 = Sdkmesh();

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
	
	// for testing
	/*std::ifstream input(inputFile, std::ios::binary | std::ios::in);
	if (!input.good())
	{
		std::cout << "Input file does not exist" << std::endl;
		return -1;
	}

	std::streampos fileSize = input.tellg();
	input.seekg(0, std::ios::end);
	fileSize = input.tellg() - fileSize;
	input.seekg(0, std::ios::beg);

	test1.CreateFromFile(input, fileSize);

	std::cout << input.tellg() << std::endl;
	std::cout << fileSize << std::endl;*/

	/*std::cout << test1.GetSdkmeshHeader().NumMeshes << std::endl;
	std::cout << test1.GetSdkmeshHeader().NumVertexBuffers << std::endl;
	std::cout << test1.GetSdkmeshHeader().NumIndexBuffers << std::endl;
	std::cout << test1.GetSdkmeshMesh()[1].NumVertexBuffers << std::endl;
	std::cout << test1.GetSdkmeshMesh()[1].NumSubsets << std::endl;
	std::cout << input.tellg() << std::endl;
	std::cout << fileSize << std::endl;*/

	if (Convert(inputFile, outputFile) == -1)
		return -1;

	std::cout << "Convert " << inputFile << " to " << outputFile << " finished" << std::endl;
	
	return 0;
}