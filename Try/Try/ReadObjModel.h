#ifndef READ_OBJ_MODEL_H
#define READ_OBJ_MODEL_H

#include <iostream>
#include <unordered_map>
#include <string>
#include "LightTypeInfo.h"
#include "GeometryGenerator.h"

class ObjModel
{
public:
	bool ReadObjModel(const char* in_file_path);
	bool ReadMtl(const std::string& in_prefix, const std::string& in_mtl_path);
	void SplitBlankSpace(const std::string&  in_source_str, std::vector<std::string>& in_split_strs);

private:
	std::unordered_map<const char*, Material> m_materals;
	//int32_t m_mtl_index = 0;
	MeshData m_mesh_data;				//几何体顶点信息和索引信息集合
	const char* m_model_name;
};

#endif
