/*
	This program can be used to generate point cloud from 3d model
	It's depend on model import library Assimp
	Usage: meshtopc.exe [input model file] [output text file] [point number upper bound]
	Set [point number upper bound] to generate fewer/sparser point cloud
	Or a point cloud with full detial
	(Windows version)
														--Author Sentao Li
															sentaoli@usc.edu
*/

#include <windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
using namespace std;
#include "assimp/Importer.hpp"      // C++ importer interface
#include "assimp/scene.h"           // Output data structure
#include "assimp/postprocess.h"     // Post processing flags


#ifdef _DEBUG
#pragma comment(lib, "assimp/Debug/assimp-vc140-mt.lib")
#else
#pragma comment(lib, "assimp/Release/assimp-vc140-mt.lib")
#endif

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "malformed input" << endl;
		cout << "meshtopc.exe [input model file] [output text file] [point number upper bound]" << endl;
	}
	Assimp::Importer modelLoader;
	cout << "start read model" << endl;
	const aiScene* model = modelLoader.ReadFile(argv[1],
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	size_t num = 0;
	for (int m = 0; m < model->mNumMeshes; m++)
	{
		auto mesh = model->mMeshes[m];
		num += mesh->mNumVertices;
	}
	cout << num << " vertices found!" << endl;
	cout << "start out put" << endl;
	int de = 1;
	if (argc > 3)
	{
		int bound = atoi(argv[3]);
		if (num > bound)
		{
			de = num / bound + (num % bound > 0);
		}
	}
	num = 0;
	ofstream fout;
	fout.open(argv[2]);
	for (int m = 0; m < model->mNumMeshes; m++)
	{
		auto mesh = model->mMeshes[m];
		for (int v = 0; v < mesh->mNumVertices; v++)
		{
			if (v%de == 0)
			{
				auto& vertex = mesh->mVertices[v];
				fout << vertex.x << "," << vertex.y << "," << vertex.z << endl;
				num++;
			}
		}
	}
	fout.close();
	cout << num << " point generated!" << endl;
	return 0;
}