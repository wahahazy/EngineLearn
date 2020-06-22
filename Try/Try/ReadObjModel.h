#ifndef READ_OBJ_MODEL_H
#define READ_OBJ_MODEL_H

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <filesystem>
#include "LightTypeInfo.h"
#include "GeometryGenerator.h"

class ObjModel
{
public:
	bool ReadObjModel(const wchar_t* in_obj_file_path);
protected:
	bool ReadMaterial(const char* in_obj_mtl_path);
	void AddVertexData();
	void AddIndices();						//读取到的indices，需要-1。因为obj文件是从1开始的。

	void SplitBlankSpace(const std::string& in_resource_str, std::vector<const char*>& in_dest_strs);
private:
	std::string m_model_name;
	std::vector<Material> m_materials;
	std::vector<VertexData> m_vertices;		//几何体顶点信息集合
	std::vector<DWORD> m_indices;			//三角形索引集
};

#endif