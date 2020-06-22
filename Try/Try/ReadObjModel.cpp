#include "ReadObjModel.h"

using namespace DirectX;
using namespace std::experimental;
bool ObjModel::ReadObjModel(const wchar_t* in_obj_file_path)
{
	if (in_obj_file_path && filesystem::exists(in_obj_file_path))
	{
		m_vertices.clear();
		m_indices.clear();

		std::ifstream infile(in_obj_file_path, std::ios::in);

		if (infile.good())
		{
			std::vector<XMFLOAT3> positions;
			std::vector<XMFLOAT3> normals;
			std::vector<XMFLOAT2> texcoords;

			std::string textline;
			std::vector<const char*> results;
			bool vertex_data_read_end = false;
			while (!infile.fail())
			{
				getline(infile, textline);
				results.clear();
				SplitBlankSpace(textline, results);
				if (results.size() != 0)
				{
					if (results[0] == "v")
					{
						positions.push_back(XMFLOAT3(std::atof(results[1]), std::atof(results[2]), std::atof(results[3])));
					}
					else if (results[0] == "vt")
					{
						texcoords.push_back(XMFLOAT2(std::atof(results[1]), std::atof(results[2])));
					}
					else if (results[0] == "vn")
					{
						normals.push_back(XMFLOAT3(std::atof(results[1]), std::atof(results[2]), std::atof(results[3])));
					}
					else if (results[0] == "usemtl")
					{

					}
					else if (results[0] == "s")
					{

					}
					else if (results[0] == "f")
					{
						if (vertex_data_read_end)
						{
							
						}
						else
						{
							vertex_data_read_end = true;
							uint32_t vertex_num = positions.size();
							if (normals.size() != 0 && normals.size() == vertex_num)
							{
								if (texcoords.size() != 0 && texcoords.size() == vertex_num)
								{
									for (uint32_t index = 0; index < vertex_num; index++)
									{
										m_vertices.push_back(VertexData(positions[index],normals[index],texcoords[index]));
									}
								}
								else
								{
									for (uint32_t index = 0; index < vertex_num; index++)
									{
										m_vertices.push_back(VertexData(positions[index], normals[index]));
									}
								}
							}
							else
							{
								if (texcoords.size() != 0 && texcoords.size() == vertex_num)
								{
									for (uint32_t index = 0; index < vertex_num; index++)
									{
										m_vertices.push_back(VertexData(positions[index], texcoords[index]));
									}
								}
								else
								{
									for (uint32_t index = 0; index < vertex_num; index++)
									{
										m_vertices.push_back(VertexData(positions[index]));
									}
								}
							}
						}
					}
					else if (results[0] == "o")
					{
						m_model_name = results[1];
					}
					else if (results[0] == "mtllib")
					{
						if (!ReadMaterial(results[1]))
						{
							return false;
						}
					}
				}
			}
		}
		infile.close();

		return true;
	}
	else
		return false;
}

bool ObjModel::ReadMaterial(const wchar_t* in_obj_mtl_path)
{
	if (in_obj_mtl_path && filesystem::exists(in_obj_mtl_path))
	{
		return true;
	}
	else
		return false;
}

void ObjModel::AddVertexData()
{

}

void ObjModel::AddIndices()
{

}

void ObjModel::SplitBlankSpace(const std::string& in_resource_str, std::vector<const char*>& in_dest_strs)
{
	std::string str = in_resource_str;
	std::size_t start = 0, index;
	std::string substr;
	index = str.find_first_of(" ", start);
	while (index!=std::string::npos)
	{
		substr = str.substr(start, index - start);
		in_dest_strs.push_back(substr.c_str());
		start = str.find_first_not_of(" ", index);
		if (start == std::string::npos)
			return;
		index = str.find_first_of(" ", start);
	}

}