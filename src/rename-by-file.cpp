#include "libraries.h"
#include "rename-by-file.h"

using namespace std;

int main(int argument_count, char* arguments[])
{
    if(argument_count > 4 || argument_count < 2)
    {
        help();
        return -1;
    }

	//Get all name pairs from the text file
	strpairvec name_vector;
	parse_name_file((argument_count == 3 ? arguments[2] : arguments[1]), 
		            (argument_count == 4 ? *(arguments[3]) : ' '), 
		             name_vector);

	//Get first file of any type from directory w/ files to be renamed
	char first_file[MAX_PATH];
	memset(first_file, '\0', MAX_PATH);
	strcat(first_file, arguments[1]);
	strcat(first_file, "*");
	
	SetCurrentDirectory(arguments[1]);

	WIN32_FIND_DATA file;
	HANDLE file_handle = FindFirstFileA(first_file, &file);

	try
	{
		char new_name[MAX_PATH];
		do
		{
			memset(new_name, '\0', MAX_PATH);
			for (unsigned i = 0; i < name_vector.size(); i++)
			{
				//If the old name from the text file is within the current filename
				//TODO: This will most likely run into problems with smaller file names, maybe handle better
				if (strstr(file.cFileName, name_vector[i].first.c_str()) != NULL)
				{
					//get the file extension
					char* extension = strrchr(file.cFileName, '.');

					strcat(new_name, name_vector[i].second.c_str());
					strcat(new_name, extension);

					//Rename the current file then delete the current pair
					rename(file.cFileName, new_name);
					name_vector.erase(name_vector.begin() + i);
					break;
				}
			}
		} while (FindNextFile(file_handle, &file) != 0);
	}
	catch (exception e)
	{
		//Outputting the DWORD GetLastError() returns as an error code is ugly as sin...
		cout << e.what() << " || " << GetLastError();
	}

    return 0;
}

void parse_name_file(char* file_path, char seperator, strpairvec &name_vector)
{
	string current_line;
    fstream name_file;

	//TODO: Make it so it defaults to .txt -> .rtf, then quits if no unique of either, unless it is specified

	name_file.open(file_path);

	if(!name_file.is_open())
	{
		cout << "Error - Could not open file";
		return;
	}

	/* Go through all lines, save the left part of line as old name, right half as new name */
	while(getline(name_file, current_line))
	{
		int name_seperator_location = current_line.find_first_of(seperator, 0);

		// "123 456" -> <"123", "456">
		string old_name = current_line.substr(0, name_seperator_location);
		string new_name = current_line.substr(name_seperator_location + 1, current_line.length() - name_seperator_location - 1);

		largefirst_insert(strpair(old_name, new_name), name_vector);
	}
}

//Inserts at first instance of size being larger than or equal to the one after it
void largefirst_insert(strpair pair, strpairvec &name_vector)
{
	//first and second case
	if (name_vector.size() == 0)
	{
		name_vector.push_back(pair);
		return;
	}
	else if(name_vector.size() == 1)
	{ 
		pair.first.length() >= name_vector[0].first.length() ? name_vector.insert(name_vector.begin(), pair) : name_vector.push_back(pair);
		return;
	}

	for (int i = 0; i < name_vector.size(); i++)
	{
		if (pair.first.length() >= name_vector[i].first.length())
		{
			name_vector.insert(name_vector.begin() + i, pair);
			return;
		}
	}

	name_vector.push_back(pair);
}

void help()
{
    cout << "\n\n";
    cout << "rename-by-file <directory> [name-source] [seperator]\n";
    cout << "\t<directory>: Directory with files to be renamed\n";
    cout << "\t[name-source]: specify name source, defaults to directory arg\n";
	cout << "\t[seperator]: specify seperation character in name file";
    cout << "\n\n";
}