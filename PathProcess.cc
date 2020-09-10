#include "PathProcess.h"

PathProcess::PathProcess(string path_)
{
    set_path(path_);
}

bool PathProcess::is_directory()
{
    return fs::is_directory(s);
}

bool PathProcess::is_file()
{
    return fs::is_regular_file(s);
}

void PathProcess::set_path(string path_)
{
    /*if (path_ == "/")
    {
        //not path, default path of server code location
        system("pwd > temp.txt");
        ifstream f("temp.txt",std::ios::binary);
        stringstream ss;
        ss << f.rdbuf();
        path_ = ss.str();
        path_.pop_back();
        f.close();
        system("rm temp.txt");
    }*/
    if (path_ == "/~") path_ = "/home/risry"; //linux ~ path
    ifstream f(path_);
    if (!f.is_open() && path_[0] == '/')
    {
        path_ = path_.substr(1); // remove slah like /Crt--->Crt
    }
    path = path_;
    s = fs::status(path);
    f.close();
}

string PathProcess::open_file()
{
    //open file and return file content
    ifstream f(path,std::ios::binary);
    stringstream ss;
    ss << f.rdbuf();
    f.close();
    return ss.str();
}
vector<string> PathProcess::file_list()
{
    //list directory file
    string temp;
    vector<string> list;
    temp = "ls " + path + " > temp.txt";
    system(temp.c_str());
    ifstream f("temp.txt",std::ios::binary);
    temp.clear();
    if (path != "/")
        list.push_back("..");
    while (getline(f,temp))
    {
        if (temp == "temp.txt") continue;
        list.push_back(temp);
        //cout << temp << endl;
    }
    system("rm temp.txt");
    f.close();
    return list;
}
string PathProcess::get_path()
{ 
    //return path, not file  like /home/
    if (path.back() != '/') return path + "/";
    return path;   
}
string PathProcess::get_name()
{
    //return file name, not path, like /home/test.txt ---> return test.txt
    int pos = path.rfind("/");
    return path.substr(pos + 1);
}