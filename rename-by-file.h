#include "libraries.h"

using namespace std;

typedef map<string, string> Name2NameMap;

int main(int argument_count, char* arguments[]);
void parse_directory(char* directory_path, vector<dirent*> *files);
void parse_name_file(char* file_path, pairvec &name_vector);
void largefirst_insert(strpair pair, pairvec &name_vector);
void help();
