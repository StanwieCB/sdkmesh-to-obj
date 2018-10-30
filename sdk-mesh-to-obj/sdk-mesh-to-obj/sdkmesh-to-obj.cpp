// An open source sdkmesh to obj converter
// supports multiple mesh conversion
// please star and refer my repo if you find it helpful, thanks!
// https://github.com/StanwieCB/sdkmesh-to-obj

/// @author: Hank Zhang

#include "obj.h"
#include "sdkmesh.h"

#include <iostream>
#include <fstream>

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

int Convert(const std::string& inputFile, const std::string& outputFile)
{
	std::ifstream input(inputFile, std::ios::binary | std::ios::in);

	// traverse to count size
	// a bit silly
	std::streampos fileSize = input.tellg();
	input.seekg(0, std::ios::end);
	fileSize = input.tellg() - fileSize;
	input.seekg(0, std::ios::beg);

	std::ofstream output(outputFile, std::ios::out | std::ios::binary);

	Sdkmesh sdkmeshInstane(input, fileSize);

	input.close();
	output.close();
	return 0;
}

int main(int argc, char** argv)
{
	InputParser parser = InputParser(argc, argv);
	std::string inputFile;
	std::string outputFile;

	//Sdkmesh sdkmesh1();

	// parse input
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

	/*if (Convert(inputFile, outputFile))
		return -1;*/
	
	return 0;
}