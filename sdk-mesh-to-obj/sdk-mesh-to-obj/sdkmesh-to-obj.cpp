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
	// a bit silly but seems no < O(n^2) way, if you have please PR, much appreciation!

	// vertices pos
	output << "# List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.0\n";
	for (auto mesh : sdkmesh.GetSdkmeshMesh())
	{
		if (mesh.VertexBuffers[0] > sdkmesh.GetSdkmeshHeader().NumVertexBuffers)
			return -1;

		std::vector<PosNormalTexTan> buffer = vertexBuffers[mesh.VertexBuffers[0]];
		for (auto vertex : buffer)
			output << "v " << vertex.pos.X << " " << vertex.pos.Y << " " << vertex.pos.Z << " 1.0\n";
	}

	// vertices tex
	output << "# List of texture coordinates, in (u, v [,w]) coordinates, these will vary between 0 and 1, w is optional and defaults to 0.\n";
	for (auto mesh : sdkmesh.GetSdkmeshMesh())
	{
		if (mesh.VertexBuffers[0] > sdkmesh.GetSdkmeshHeader().NumVertexBuffers)
			return -1;

		std::vector<PosNormalTexTan> buffer = vertexBuffers[mesh.VertexBuffers[0]];
		for (auto vertex : buffer)
			output << "vt " << vertex.tex.X << " " << vertex.tex.Y << " 0\n";
	}

	// vertices norm
	output << "# List of vertex normals in (x,y,z) form; normals might not be unit vectors.\n";
	for (auto mesh : sdkmesh.GetSdkmeshMesh())
	{
		if (mesh.VertexBuffers[0] > sdkmesh.GetSdkmeshHeader().NumVertexBuffers)
			return -1;

		std::vector<PosNormalTexTan> buffer = vertexBuffers[mesh.VertexBuffers[0]];
		for (auto vertex : buffer)
			output << "vn " << vertex.norm.X << " " << vertex.norm.Y << " " << vertex.norm.Z << "\n";
	}

	// mesh indices
	output << " # Polygonal face element v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3.\n";
	for (auto mesh : sdkmesh.GetSdkmeshMesh())
	{
		if (mesh.IndexBuffer > sdkmesh.GetSdkmeshHeader().NumIndexBuffers)
			return -1;

		std::vector<int> buffer = indexBuffers[mesh.IndexBuffer];

		// triangle mesh
		int numTriangles = buffer.size() / 3;
		for (int i = 0; i < numTriangles; i++)
		{
			output << "f " << buffer[3 * i] << "/" << buffer[3 * i] << "/" << buffer[3 * i] << " ";
			output << buffer[3 * i + 1] << "/" << buffer[3 * i + 1] << "/" << buffer[3 * i + 1] << " ";
			output << buffer[3 * i + 2] << "/" << buffer[3 * i + 2] << "/" << buffer[3 * i + 2] << "\n";
		}
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
	if (outputFile.substr(outputFile.size() - 4) != ".obj")
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

	Sdkmesh sdkmesh1 = Sdkmesh();
	sdkmesh1.DoCheck();

	// parse arguments
	if (parser.CmdOptionExists("-i"))
		inputFile = parser.GetCmdOption("-i");
	else
	{
		std::cout << "Empty input file name!" << std::endl;
		return -1;
	}
	if (parser.CmdOptionExists("-o"))
		inputFile = parser.GetCmdOption("-o");
	else
	{
		std::cout << "Empty output file name!" << std::endl;
		return -1;
	}

	std::cout << "Input target: " << inputFile << std::endl;
	std::cout << "Output target: " << inputFile << std::endl;
	
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

	sdkmesh1.CreateFromFile(input, fileSize);
	std::cout << sdkmesh1.GetSdkmeshHeader().NumMeshes << std::endl;
	std::cout << sdkmesh1.GetSdkmeshHeader().NumVertexBuffers << std::endl;
	std::cout << sdkmesh1.GetSdkmeshHeader().NumIndexBuffers << std::endl;
	std::cout << sdkmesh1.GetSdkmeshMesh()[1].NumVertexBuffers << std::endl;
	std::cout << sdkmesh1.GetSdkmeshMesh()[1].NumSubsets << std::endl;
	std::cout << input.tellg() << std::endl;
	std::cout << fileSize << std::endl;*/

	if (Convert(inputFile, outputFile) == -1)
		return -1;

	std::cout << "Convert: " << inputFile << " to " << outputFile << " finished" << std::endl;
	
	return 0;
}