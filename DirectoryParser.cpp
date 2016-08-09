#include "DirectoryParser.h"
#include "stdafx.h"

DirectoryParser::DirectoryParser()
{
}

DirectoryParser::~ DirectoryParser()
{
}

void DirectoryParser::parse(char* sDirectoryPath, std::vector<dirent*> *rFiles)
{
	DIR *pDirectory;
	struct dirent *pEntry;

	rFiles->clear();

	if ((pDirectory = opendir(sDirectoryPath)) != NULL) 
	{
		/* print all the files and directories within directory */
		while ((pEntry = readdir(pDirectory)) != NULL) 
		{
			rFiles->push_back(pEntry);
		}
		closedir(pDirectory);
	}
}
