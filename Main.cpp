/*
 Generated by Darkpassanger123 - 2020
*/

#include <boost/filesystem.hpp>

#include <iostream>
#include <windows.h>
#include <fstream>
#include <iterator>
#include <set>
#include <vector>

using namespace boost::filesystem;


std::vector<boost::filesystem::path> FilesToDelete;
std::uint64_t TotalFileSize = 0;

std::wstring GetWorkingDirectory()
{
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);

    std::wstring result = std::wstring(buffer);

    return result.substr(0, result.find_last_of(L"\\/"));
}

void PrintW(const wchar_t* format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    vwprintf(format, argptr);

    va_end(argptr);
}

std::set<boost::filesystem::path> getDirContents(const std::wstring& dirName)
{
    std::set<boost::filesystem::path> paths;
    std::copy
    (boost::filesystem::directory_iterator(dirName)
        , boost::filesystem::directory_iterator()
        , std::inserter(paths, paths.end())
    );

    return paths;
}

bool ShouldDeleteFile(const std::wstring& extention)
{
    static std::vector<std::wstring> ExtensionsToDelete =
    {
     L".a",
     L".app",
     L".d",
     L".db",
     L".dylib",
     L".gnc",
     L".iobj",
     L".ipch",
     L".ipdb",
     L".la",
     L".lai",
     L".lastbuildstate",
     L".lo",
     L".log",
     L".map",
     L".mod",
     L".o",
     L".obj",
     L".opendb",
     L".out",
     L".pch",
     L".pdb",
     L".slo",
     L".smod",
     L".so",
     L".suo",
     L".tlog",
     L".unsuccessfullbuild",
     L".vc",
     L".vc.db"
    };
 
    auto it = find(ExtensionsToDelete.begin(), ExtensionsToDelete.end(), extention);
    
    if (it != ExtensionsToDelete.end())
        return true;
    
    return false;
}


void HandleFileDelete(const boost::filesystem::path& path)
{
    while (boost::filesystem::exists(path))
    {
        boost::filesystem::remove(path);
    }
}

void ReadFolder(const std::wstring& dirName)
{
    for (const boost::filesystem::directory_entry& p : boost::filesystem::directory_iterator(dirName))
    {

        if (boost::filesystem::is_directory(p))
        {       
            // Handle Folder
            ReadFolder(p.path().c_str());  
        }      
        else
        {
            // Handle File
            if (ShouldDeleteFile(p.path().extension().c_str()))
            {
                FilesToDelete.push_back(p.path());
                TotalFileSize += boost::filesystem::file_size(p.path());
            }   
        }
                             
    }

}


int main(int argc, char* argv[])
{
   
    PrintW(L"*** Welcome To Darkpassanger123 Visual Studio Project Cleaner *** \n\n");

    if (argc < 2)
    {
        PrintW(L"Itterating through working directory folder and all of its subfolders. \n");
        PrintW(L"Working Directory:\n%s \n\n", GetWorkingDirectory());

        ReadFolder(GetWorkingDirectory());
    }
    else if (argc == 2)
    {
        
        std::string a(argv[1]);
        auto Path = std::wstring(a.begin(), a.end());
      
        PrintW(L"Path: %s \n", Path);
        
        if (!boost::filesystem::exists(Path))
        {
            PrintW(L"Error: Please Enter A Valid Path \n");
          
            system("pause");
            return 0;
        }
        
        ReadFolder(Path);
    }
    else if (argc > 3)
    {
        PrintW(L"Error: Please Only Enter 1 Argument Which Directory To Itterate \n\n");

        system("pause");
        return 0;
    }

 
    PrintW(L"Total Of: %i target file found \n", FilesToDelete.size());
    PrintW(L"Total Size Of Files: [%i/mb] \n", TotalFileSize / 1024 / 1024);

    std::wstring Option;
  
    while (Option == L"")
    {

        PrintW(L"Want to countinue to delete target files ? [Y/N/Info] \n");
        std::getline(std::wcin, Option);
        transform(Option.begin(), Option.end(), Option.begin(), towlower);

        if (Option == "y")
        {
            for (size_t i = 0; i < FilesToDelete.size(); i++)
            {
                auto File = FilesToDelete.at(i);
                PrintW(L"Deleting: %s \n", File.filename().c_str());
                HandleFileDelete(File);
            }

            PrintW(L"%i files are successfully deleted, [%i/mb] worth of space is Cleaned \n", FilesToDelete.size(), TotalFileSize / 1024 / 1024);
        }
        else if (Option == "info")
        {
           
            PrintW(L"\n\n\nWill Delete This Files... \n\n\n");
            for (size_t i = 0; i < FilesToDelete.size(); i++)
            {
                auto File = FilesToDelete.at(i);
                PrintW(L"%s \n", File.c_str());
            }

            PrintW(L"\n\n\nTotal Of: %i target file found \n", FilesToDelete.size());
            PrintW(L"Total Size Of Files: [%i/mb] \n", TotalFileSize / 1024 / 1024);

            Option = L"";
        }
        else
        {
            PrintW(L"Deletion is aborted \n");
        }
    }

   
    system("pause");

    return 0;
}