#include "ReadObjModel.h"
#include <fstream>

bool ObjModel::ReadObjModel(const char* in_file_path)
{
	std::string path_prefix;
	std::ifstream in_file(in_file_path);
	if (in_file.is_open()) {
		std::string line;
		std::vector<std::string> split_strs;
		std::vector<XMFLOAT3> positions;
		std::vector<XMFLOAT3> normals;
		std::vector<XMFLOAT2> texcoords;
		bool read_vertex_data_ready = false;
		while (std::getline(in_file, line)) 
		{
			split_strs.clear();
			SplitBlankSpace(line,split_strs);
			if (split_strs.size() != 0)
			{
				if (split_strs[0] == "o")
				{
					m_model_name = split_strs[1].c_str();
				}
				else if (split_strs[0] == "mtllib")
				{
					if (!ReadMtl(path_prefix, split_strs[1].c_str()))
						return false;
				}
				else if (split_strs[0] == "v")
				{
					positions.push_back(XMFLOAT3(std::atof(split_strs[1].c_str()), std::atof(split_strs[2].c_str()), std::atof(split_strs[3].c_str())));
				}
				else if (split_strs[0] == "vt")
				{
					texcoords.push_back(XMFLOAT2(std::atof(split_strs[1].c_str()), std::atof(split_strs[2].c_str())));
				}
				else if (split_strs[0] == "vn")
				{
					normals.push_back(XMFLOAT3(std::atof(split_strs[1].c_str()), std::atof(split_strs[2].c_str()), std::atof(split_strs[3].c_str())));
				}
				else if (split_strs[0] == "usemtl")
				{
					//m_materals[split_strs[1].c_str()] = m_materal_list[m_mtl_index];
					//m_mtl_index++;
					//也先放着
				}
				else if (split_strs[0] == "s")
				{
					//不会用，先放着
				}
				else if (split_strs[0] == "f")
				{
					if(!read_vertex_data_ready)
					{
						read_vertex_data_ready = true;
						uint32_t vertex_num = positions.size();
						if (normals.size() == 0)
						{
							if (texcoords.size() == 0)
							{
								m_mesh_data.VertexDataUseState(true,false,false);
							}
							else if (texcoords.size() == vertex_num)
							{
								m_mesh_data.VertexDataUseState(true, false, true);
							}
							else
							{
								return false;
							}
						}
						else if (normals.size() == vertex_num)
						{
							if (texcoords.size() == 0)
							{
								m_mesh_data.VertexDataUseState(true, true, false);
							}
							else if (texcoords.size() == vertex_num)
							{
								m_mesh_data.VertexDataUseState(true, true, true);
							}
							else
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}

					std::size_t start = 0, index = 0;
					uint32_t pos_index=0, normal_index=0, tex_index=0;
					for (int32_t i = 1; i < split_strs.size(); i++)
					{
						index = split_strs[i].find_first_of("/");
						pos_index = atoi(split_strs[i].substr(start, index).c_str());
						start = index;
						index = split_strs[i].find_first_of("/");
						if(index - start != 1)
							normal_index = atoi(split_strs[i].substr(start, index).c_str());
						if (split_strs.size() - index != 1)
							tex_index = atoi(split_strs[i].substr(index+1).c_str());

						if(normal_index && tex_index)
							m_mesh_data.AddVertexData(VertexData(positions[pos_index-1],normals[normal_index-1],texcoords[tex_index-1]));
						else if(normal_index && !tex_index)
							m_mesh_data.AddVertexData(VertexData(positions[pos_index - 1], normals[normal_index - 1]));
						else if(!normal_index && tex_index)
							m_mesh_data.AddVertexData(VertexData(positions[pos_index - 1], texcoords[tex_index - 1]));
						else
							m_mesh_data.AddVertexData(VertexData(positions[pos_index - 1]));
					}
				}
			}
		}
		in_file.close();
	}
}

bool ObjModel::ReadMtl(const std::string& in_prefix, const std::string& in_mtl_path)
{
	std::string mtl_path = in_prefix + in_mtl_path;
	std::ifstream in_file(mtl_path);
	if (in_file.is_open()) {
		std::string line;
		std::vector<std::string> split_strs;
		Material temp_mtl;
		while (std::getline(in_file, line))
		{
			split_strs.clear();
			SplitBlankSpace(line, split_strs);
			if (split_strs.size() != 0)
			{
				if (split_strs[0] == "newmtl")
				{
					if (temp_mtl.m_name != "")
					{
						m_materals[temp_mtl.m_name] = temp_mtl;
					}
					temp_mtl = Material();
				}
				else if (split_strs[0] == "Ns")
				{
					temp_mtl.Ambient.w = atof(split_strs[1].c_str());
				}
				else if (split_strs[0] == "Ka")
				{
					temp_mtl.Ambient.x = atof(split_strs[1].c_str());
					temp_mtl.Ambient.y = atof(split_strs[2].c_str());
					temp_mtl.Ambient.z = atof(split_strs[3].c_str());
				}
				else if (split_strs[0] == "Kd")
				{
					temp_mtl.Diffuse.x = atof(split_strs[1].c_str());
					temp_mtl.Diffuse.y = atof(split_strs[2].c_str());
					temp_mtl.Diffuse.z = atof(split_strs[3].c_str());
				}
				else if (split_strs[0] == "Ks")
				{
					temp_mtl.Specular.x = atof(split_strs[1].c_str());
					temp_mtl.Specular.y = atof(split_strs[2].c_str());
					temp_mtl.Specular.z = atof(split_strs[3].c_str());
				}
				else if (split_strs[0] == "Ni")
				{
					//先不考虑
				}
				else if (split_strs[0] == "d")
				{
					temp_mtl.Ambient.w = atof(split_strs[1].c_str());
					temp_mtl.Diffuse.w = atof(split_strs[1].c_str());
				}
				else if (split_strs[0] == "illum")
				{
					//先不考虑
				}
				else if (split_strs[0] == "map_Kd")
				{
					temp_mtl.m_texture_path = (in_prefix + split_strs[1]).c_str();
				}
			}

		}
		in_file.close();
	}
}

void ObjModel::SplitBlankSpace(const std::string&  in_source_str, std::vector<std::string>& in_split_strs)
{
	std::size_t start = in_source_str.find_first_not_of(" "), index = in_source_str.find_first_of(" ");
	while (start!= in_source_str.npos && index != in_source_str.npos)
	{
		in_split_strs.push_back(in_source_str.substr(start,index));
		start = in_source_str.find_first_not_of(" ", index);
		index = in_source_str.find_first_of(" ", start);
	}
}