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
	pairvec name_vector;
	parse_name_file((argument_count == 3 ? arguments[2] : arguments[1]), 
		            (argument_count == 4 ? *(arguments[3]) : ' '), 
		             name_vector);

	//Get first file from directory w/ files to be renamed
	WIN32_FIND_DATA file;
	HANDLE file_handle = FindFirstFile(arguments[1], &file);

	try
	{
		do
		{
			for (unsigned i = 0; i < name_vector.size(); i++)
			{
				//If the old name from the text file is within the current filename
				//TODO: This will most likely run into problems with smaller file names, maybe handle better
				if (strstr(file.cAlternateFileName, name_vector[i].first.c_str()) != NULL)
				{
					//Rename the current file then delete the current pair
					rename(file.cAlternateFileName, name_vector[i].second.c_str());
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

void parse_name_file(char* file_path, char seperator, pairvec &name_vector)
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
		current_line.find_first_of(seperator, name_seperator_location);

		// "123 456" -> <"123", "456">
		string old_name = current_line.substr(0, name_seperator_location);
		string new_name = current_line.substr(name_seperator_location + 1, current_line.length() - name_seperator_location - 1);

		largefirst_insert(strpair(old_name, new_name), name_vector);
	}
}

//Inserts at first instance of size being larger than the one after it
void largefirst_insert(strpair pair, pairvec &name_vector)
{
	//first and second case
	if (name_vector.size() == 0)
	{
		name_vector.push_back(pair);
		return;
	}
	else if(name_vector.size() == 1)
	{ 
		pair.first > name_vector[0].first ? name_vector.push_back(pair) : name_vector.emplace_back(pair);
		return;
	}

	pairvec::iterator it = name_vector.begin();
	do
	{
		if (pair.first.length() >= (it + 1)->first.length())
		{
			name_vector.insert(it, pair);
			break;
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