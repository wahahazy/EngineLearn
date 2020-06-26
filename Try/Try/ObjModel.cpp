#include "ObjModel.h"
#include <fstream>

namespace obj
{
	bool ObjReader::ReadObjModel(const char* in_file_path)
	{
		std::string path_prefix;
		std::size_t path_slash_index;
		if ((path_slash_index = std::string(in_file_path).find_last_of('/')) == std::wstring::npos &&
			(path_slash_index = std::string(in_file_path).find_last_of('\\')) == std::wstring::npos)
		{
			path_prefix == "";
		}
		else
		{
			path_prefix = std::string(in_file_path).substr(0, path_slash_index+1);
		}
		
		std::ifstream in_file(in_file_path);
		if (in_file.is_open()) {
			std::string line;
			std::vector<std::string> split_strs;
			std::vector<XMFLOAT3> positions;
			std::vector<XMFLOAT3> normals;
			std::vector<XMFLOAT2> texcoords;
			XMVECTOR boundingbox_min = g_XMInfinity, boundingbox_max = g_XMNegInfinity;
			std::shared_ptr<MeshData> mesh_data = nullptr;
			while (std::getline(in_file, line))
			{
				split_strs.clear();
				SplitBlankSpace(line, split_strs);
				if (split_strs.size() != 0)
				{
					if (split_strs[0] == "#")
						continue;
					else if (split_strs[0] == "o")
					{
						m_model_name = split_strs[1];
					}
					else if (split_strs[0] == "mtllib")
					{
						if (!ReadMtl(path_prefix, split_strs[1].c_str()))
							return false;
					}
					else if (split_strs[0] == "v")
					{
						// d3d使用的是左手坐标系，obj使用的是右手坐标系
						XMFLOAT3 pos = XMFLOAT3(std::atof(split_strs[1].c_str()), std::atof(split_strs[2].c_str()), -std::atof(split_strs[3].c_str()));
						positions.push_back(pos);
						XMVECTOR vec_pos = XMLoadFloat3(&pos);
						boundingbox_max = XMVectorMax(boundingbox_max, vec_pos);
						boundingbox_min = XMVectorMin(boundingbox_min, vec_pos);
					}
					else if (split_strs[0] == "vt")
					{
						// d3d的纹理坐标起始点在左上角
						texcoords.push_back(XMFLOAT2(std::atof(split_strs[1].c_str()), 1.0f - std::atof(split_strs[2].c_str())));
					}
					else if (split_strs[0] == "vn")
					{
						// d3d使用的是左手坐标系，obj使用的是右手坐标系
						normals.push_back(XMFLOAT3(std::atof(split_strs[1].c_str()), std::atof(split_strs[2].c_str()), -std::atof(split_strs[3].c_str())));
					}
					else if (split_strs[0] == "usemtl")
					{
						mesh_data = std::make_shared<MeshData>();
						m_mesh_datas.push_back(mesh_data);
						m_materials_name.push_back(split_strs[1]);
					}
					else if (split_strs[0] == "s")
					{
						//不会用，先放着
					}
					else if (split_strs[0] == "f")
					{
						//右手坐标系转成了左手坐标系。若还是保持原有的顺时针或者逆时针，则三角形顶点顺序要反一下。
						std::size_t start, index;
						uint32_t pos_index, normal_index, tex_index;
						for (int32_t i = split_strs.size() - 1; i >= 1; i--)
						{
							start = 0, index = 0;
							pos_index = 0, normal_index = 0, tex_index = 0;

							index = split_strs[i].find_first_of("/");
							if (index == split_strs[i].npos)
								return false;
							pos_index = atoi(split_strs[i].substr(start, index - start).c_str());
							start = index + 1;
							index = split_strs[i].find_first_of("/", start);
							if (start == split_strs[i].npos || index == split_strs[i].npos)
								return false;
							if (index != start)
								tex_index = atoi(split_strs[i].substr(start, index- start).c_str());
							if (split_strs[i].size() - index > 1)
								normal_index = atoi(split_strs[i].substr(index + 1, split_strs[i].size() - index - 1).c_str());

							if (normal_index && tex_index)
								mesh_data->AddVertexData(VertexData(positions[pos_index - 1], normals[normal_index - 1], texcoords[tex_index - 1]));
							else if (normal_index && !tex_index)
								mesh_data->AddVertexData(VertexData(positions[pos_index - 1], normals[normal_index - 1]));
							else if (!normal_index && tex_index)
								mesh_data->AddVertexData(VertexData(positions[pos_index - 1], texcoords[tex_index - 1]));
							else
								mesh_data->AddVertexData(VertexData(positions[pos_index - 1]));
						}
					}
				}
			}
			in_file.close();

			XMStoreFloat3(&m_boundingbox_max, boundingbox_max);
			XMStoreFloat3(&m_boundingbox_min, boundingbox_min);

			return true;
		}
		else
			return false;
	}

	bool ObjReader::ReadMtl(const std::string& in_prefix, const std::string& in_mtl_path)
	{
		std::string mtl_path = in_prefix + in_mtl_path;
		std::ifstream in_file(mtl_path);
		if (in_file.is_open()) {
			std::string line;
			std::vector<std::string> split_strs;
			std::shared_ptr<MaterialInfo> temp_mtl = nullptr;
			while (std::getline(in_file, line))
			{
				split_strs.clear();
				SplitBlankSpace(line, split_strs);
				if (split_strs.size() != 0)
				{
					if (split_strs[0] == "newmtl")
					{
						std::string name = split_strs[1];
						temp_mtl = std::make_shared<MaterialInfo>();
						temp_mtl->m_name = name;
						m_all_material_infos[name] = temp_mtl;
					}
					else if (split_strs[0] == "Ns")
					{
						temp_mtl->m_specular.w = atof(split_strs[1].c_str());
					}
					else if (split_strs[0] == "Ka")
					{
						temp_mtl->m_ambient.x = atof(split_strs[1].c_str());
						temp_mtl->m_ambient.y = atof(split_strs[2].c_str());
						temp_mtl->m_ambient.z = atof(split_strs[3].c_str());
					}
					else if (split_strs[0] == "Kd")
					{
						temp_mtl->m_diffuse.x = atof(split_strs[1].c_str());
						temp_mtl->m_diffuse.y = atof(split_strs[2].c_str());
						temp_mtl->m_diffuse.z = atof(split_strs[3].c_str());
					}
					else if (split_strs[0] == "Ks")
					{
						temp_mtl->m_specular.x = atof(split_strs[1].c_str());
						temp_mtl->m_specular.y = atof(split_strs[2].c_str());
						temp_mtl->m_specular.z = atof(split_strs[3].c_str());
					}
					else if (split_strs[0] == "Ni")
					{
						//先不考虑
					}
					else if (split_strs[0] == "d")
					{
						temp_mtl->m_ambient.w = atof(split_strs[1].c_str());
						temp_mtl->m_diffuse.w = atof(split_strs[1].c_str());
					}
					else if (split_strs[0] == "illum")
					{
						//先不考虑
					}
					else if (split_strs[0] == "map_Kd")
					{
						temp_mtl->m_texture_path = in_prefix + split_strs[1];
					}
				}

			}
			in_file.close();
			return true;
		}
		else
			return false;
	}

	void ObjReader::SplitBlankSpace(const std::string&  in_source_str, std::vector<std::string>& in_split_strs)
	{
		std::size_t start = in_source_str.find_first_not_of(" "), index = in_source_str.find_first_of(" ");
		while (start != in_source_str.npos && index != in_source_str.npos)
		{
			in_split_strs.push_back(in_source_str.substr(start, index - start));
			start = in_source_str.find_first_not_of(" ", index);
			index = in_source_str.find_first_of(" ", start);
		}
		if (start != in_source_str.npos)
		{
			in_split_strs.push_back(in_source_str.substr(start, in_source_str.size() - start));
		}
	}


	//------------------------------------------------------------------------------------------------------------
	// ObjModel class
	ObjModel::ObjModel()
	{

	}

	ObjModel::~ObjModel()
	{

	}

	bool ObjModel::CreateModel(ComPtr<ID3D11Device> in_g_pd3dDevice, const ObjReader& in_reader)
	{
		m_model_sub_meshes.resize(in_reader.m_mesh_datas.size());

		BoundingBox::CreateFromPoints(m_bounding_box, XMLoadFloat3(&in_reader.m_boundingbox_min), XMLoadFloat3(&in_reader.m_boundingbox_max));

		for (size_t i = 0; i < in_reader.m_mesh_datas.size(); ++i)
		{
			auto sub_mesh = in_reader.m_mesh_datas[i];
			m_model_sub_meshes[i].SetBuffer(in_g_pd3dDevice, *sub_mesh.get());

			// 创建纹理
			auto mtl_it = in_reader.m_all_material_infos.find(in_reader.m_materials_name[i]);
			if (mtl_it != in_reader.m_all_material_infos.end())
			{
				auto mtl_info = mtl_it->second;				
				auto texture_path = (mtl_info->m_texture_path).c_str();

				int wchar_size = MultiByteToWideChar(CP_OEMCP, 0, texture_path, strlen(texture_path) + 1, NULL, 0);
				wchar_t *textrue_path_wchar = new wchar_t[wchar_size];
				MultiByteToWideChar(CP_OEMCP, 0, texture_path, strlen(texture_path) + 1, textrue_path_wchar, wchar_size);
				ComPtr<ID3D11ShaderResourceView> texture;
				HRESULT hr = CreateWICTextureFromFile(in_g_pd3dDevice.Get(), textrue_path_wchar, nullptr, texture.GetAddressOf());
				if (FAILED(hr))
					return false;

				m_model_sub_meshes[i].SetTexture(texture, texture);

				Material mtl(mtl_info->m_ambient, mtl_info->m_diffuse, mtl_info->m_specular);
				m_model_sub_meshes[i].SetMaterial(mtl);
			}
			else
			{
				// 没有材质时候的默认值
				Material mtl(XMFLOAT4(1.0f, 1.0f,1.0f,1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
				m_model_sub_meshes[i].SetMaterial(mtl);
			}

		}

		return true;
	}

	void XM_CALLCONV ObjModel::SetScaleMatrix(XMMATRIX in_scale_matrix)
	{
		for (auto& part : m_model_sub_meshes)
		{
			part.SetScaleMatrix(in_scale_matrix);
		}
	}

	void ObjModel::SetTranslationMatrix(float in_x, float in_y, float in_z)
	{
		for (auto& part : m_model_sub_meshes)
		{
			part.UpdateTranslationMatrix(in_x, in_y, in_z);
		}
	}

	void ObjModel::SetRotationMatrix(float in_radian)
	{
		for (auto& part : m_model_sub_meshes)
		{
			part.SetRotationMatrixY(in_radian);
		}
	}

	BoundingBox ObjModel::GetLocalBoundingBox()
	{
		return m_bounding_box;
	}

	void ObjModel::Draw(ComPtr<ID3D11DeviceContext>  in_g_pImmediateContext, GameObjectRender &in_render)
	{
		for (auto& sub_mesh : m_model_sub_meshes)
		{
			sub_mesh.Draw(in_g_pImmediateContext, in_render);
		}
	}

	void ObjModel::Draw(ComPtr<ID3D11DeviceContext>  in_g_pImmediateContext, ShadowMapRender &in_render)
	{
		for (auto& sub_mesh : m_model_sub_meshes)
		{
			sub_mesh.Draw(in_g_pImmediateContext, in_render);
		}
	}
}
