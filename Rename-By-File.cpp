#include "Includes.h"
#include "rename-by-file.h"

using namespace std;

int main(int argument_count, char* arguments[])
{
    if(argument_count != 3 || argument_count != 2)
    {
        help();
        return -1;
    }

	_chdir(arguments[1]);

	vector<dirent*> files;
	parseDirectory(arguments[1], &files);

	Name2NameMap name_map;
	parseNameFile((argument_count == 2 ? arguments[2] : arguments[1]), &name_map);

	for each (pair<string, string> name_pair in name_map)
	{
		//TODO: Search for file base on name_pair.first, then replace with name_pair.second while being extension agnostic
	}

    return 0;
}

void parseDirectory(char* sDirectoryPath, vector<dirent*> *rFiles)
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

void parseNameFile(char* file_path, Name2NameMap *name_change_map)
{
	std::string current_line;
    fstream name_file;
	name_file.open(file_path);

	if(!name_file.is_open())
	{
		cout << "Error - Could not open file";
		return;
	}

	/* Go through all lines, save the left part of line as old name, right half as new name */
	while(getline(name_file, current_line))
	{
		int name_seperator_location;
		current_line.find_first_of(" ", name_seperator_location);

		string old_name = current_line.substr(0, name_seperator_location);
		string new_name = current_line.substr(name_seperator_location + 1, current_line.length() - name_seperator_location - 1);

		name_change_map->insert(pair<string, string>(old_name, new_name));
	}
}

void help()
{
    cout << "\n\n";
    cout << "Rename-By-File Help:\trename-by-file -d directory [-n name-source] [-s seperator]\n";
    cout << "\t directory: Directory with files to be renamed\n";
    cout << "\t[name-source]: specify name source, defaults to directory arg\n";
	cout << "\t[seperator]: specify seperation character in name file";
    cout << "\n\n";
}