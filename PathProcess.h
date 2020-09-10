#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>
#include <vector>
using namespace std;
namespace fs = std::experimental::filesystem;

class PathProcess 
{
    public:
        PathProcess(){};
        PathProcess(string path_);
        ~PathProcess(){};
        bool is_directory();
        bool is_file();
        void set_path(string path_);
        string open_file();
        vector<string> file_list();
        string get_path();
        string get_name();
    private:
        string path;
        fs::file_status s;
};