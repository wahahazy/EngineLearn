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
	void AddIndices();						//��ȡ����indices����Ҫ-1����Ϊobj�ļ��Ǵ�1��ʼ�ġ�

	void SplitBlankSpace(const std::string& in_resource_str, std::vector<const char*>& in_dest_strs);
private:
	std::string m_model_name;
	std::vector<Material> m_materials;
	std::vector<VertexData> m_vertices;		//�����嶥����Ϣ����
	std::vector<DWORD> m_indices;			//������������
};

#endif