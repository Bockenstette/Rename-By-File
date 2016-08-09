#include "stdafx.h"
#include <vector>

class DirectoryParser
{
public:
	DirectoryParser();
	~DirectoryParser();

	static void parse(char* sDirectoryPath, std::vector<dirent*> *rFiles);

private:

};