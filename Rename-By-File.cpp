#include "libraries.h"
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
	parse_directory(arguments[1], &files);

	pairvec name_vector;
	parse_name_file((argument_count == 2 ? arguments[2] : arguments[1]), name_vector);

	for each (strpair name_pair in name_vector)
	{
		//TODO: Search for file base on name_pair.first, then replace with name_pair.second while being extension agnostic
	}

    return 0;
}

void parse_directory(char* sDirectoryPath, vector<dirent*> *rFiles)
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

void parse_name_file(char* file_path, pairvec &name_vector)
{
	string current_line;
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
		int name_seperator_location = 0;
		current_line.find_first_of(" ", name_seperator_location);

		string old_name = current_line.substr(0, name_seperator_location);
		string new_name = current_line.substr(name_seperator_location + 1, current_line.length() - name_seperator_location - 1);

		largefirst_insert(strpair(old_name, new_name), name_vector);
	}
}

void largefirst_insert(strpair pair, pairvec &name_vector)
{
	pairvec::iterator it = name_vector.begin();
	do
	{
		if (pair.first.length() >= (it + 1)->first.length())
		{
			name_vector.insert(it, pair);
		}
	} while (++it != name_vector.end());
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