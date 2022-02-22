#include <iostream>
#include <cstring>        // for strcat()
#include <io.h>
#include <list>
#include <vector>

//OPENMESH
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>


#ifdef _DEBUG
#pragma comment(lib, "OpenMeshCored.lib")
#pragma comment(lib, "OpenMeshToolsd.lib")
#else
#pragma comment(lib, "OpenMeshCore.lib")
#pragma comment(lib, "OpenMeshTools.lib")
#endif // _DEBUG


typedef OpenMesh::TriMesh_ArrayKernelT<> MyTriMesh;
using namespace OpenMesh;
using namespace std;


vector<string> listFiles(const char* dir)
{
    vector<string> filesname;
    char dirNew[200];
    strcpy_s(dirNew, dir);
    strcat_s(dirNew, "\\*.*");    // 在目录后面加上"\\*.*"进行第一次搜索

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(dirNew, &findData);
    if (handle == -1)        // 检查是否成功
        return filesname;

    do
    {
        if (findData.attrib & _A_SUBDIR)
        {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;

            cout << findData.name << "\t<dir>\n";

            // 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
            strcpy_s(dirNew, dir);
            strcat_s(dirNew, "\\");
            strcat_s(dirNew, findData.name);

            listFiles(dirNew);
        }
        else
            //cout << findData.name << "\t" << findData.size << " bytes.\n";
            filesname.push_back(findData.name);
    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
    return filesname;
}
int main(int argc, char** argv)
{
    MyTriMesh mesh;
    IO::Options ropt;
    ropt += IO::Options::Binary;
    ropt += IO::Options::Swap;


    //char filename[256];
    char objpath[] = "D:\\temp\\openmesh\\obj\\";
    char offpath[] = "D:\\temp\\openmesh\\off\\";
    
    vector<string> filesname = listFiles(objpath);
    

    //vector<string>::iterator i;

    for (int i = 0; i < filesname.size(); i++)
    {
        //要转换的obj文件地址
        string objfn = string(objpath) + filesname[i];
        if (!IO::read_mesh(mesh, objfn, ropt))
        {
            std::cerr << "Error loading mesh from file " << objfn << std::endl;
            return false;
        }
        cout << filesname[i] << endl;

        //目的off文件地址
        string offfn = string(offpath) + filesname[i].substr(0, filesname[i].length()-3) + "off";
         
        if (!OpenMesh::IO::write_mesh(mesh, offfn))
        {
            std::cerr << "Cannot write mesh to file " << offfn << std::endl;
        }
        else
            cout << "successfully save mesh to " << offfn << endl;
    }


    //for (int i = 0; i < 1; i++) {
    //    //Load Mesh with openmesh
    //    
    //    //
    //    //要转换的obj文件地址
    //    string objename = objpath + "a";
    //    cout<< objename <<endl;
    //    if (!IO::read_mesh(mesh, objename, ropt))
    //    {
    //        std::cerr << "Error loading mesh from file " << objename << std::endl;
    //        return false;
    //    }
    //    
    //    //目的off文件地址
    //    if (!OpenMesh::IO::write_mesh(mesh, filename))
    //    {
    //        std::cerr << "Cannot write mesh to file " << filename << std::endl;
    //    }
    //    else
    //        cout << "successfully save mesh to " << filename << endl;
    //}



}