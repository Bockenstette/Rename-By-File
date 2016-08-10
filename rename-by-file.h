#include "Includes.h"

using namespace std;

typedef map<string, string> Name2NameMap;

int main(int argument_count, char* arguments[]);
void parseDirectory(char* directory_path, vector<dirent*> *files);
void parseNameFile(char* file_path, Name2NameMap *name_change_map);
void help();
