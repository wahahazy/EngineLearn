#ifndef READ_OBJ_MODEL_H
#define READ_OBJ_MODEL_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "GeometryGenerator.h"
#include "GameObject.h"

namespace obj
{
	struct MaterialInfo
	{
		MaterialInfo() 
		{ 
			m_ambient = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
			m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			m_specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			m_texture_path = "";
			m_name = "";
		}

		XMFLOAT4 m_ambient;
		XMFLOAT4 m_diffuse;
		XMFLOAT4 m_specular; // w = 此材质的光泽度

		std::string m_texture_path;
		std::string m_name;
	};

	class ObjReader
	{
	public:
		bool ReadObjModel(const char* in_file_path);
		bool ReadMtl(const std::string& in_prefix, const std::string& in_mtl_path);
		void SplitBlankSpace(const std::string&  in_source_str, std::vector<std::string>& in_split_strs);

		std::unordered_map<std::string, std::shared_ptr<MaterialInfo>> m_all_material_infos;
		std::vector<std::shared_ptr<MeshData>> m_mesh_datas;	//几何体顶点信息和索引信息集合
		std::vector<std::string> m_materials_name;
		std::string m_model_name;
		XMFLOAT3 m_boundingbox_min, m_boundingbox_max;
	};


	class ObjModel
	{
	public:
		ObjModel();
		~ObjModel();

		bool CreateModel(ComPtr<ID3D11Device> in_g_pd3dDevice, const ObjReader& in_reader);
		void XM_CALLCONV SetScaleMatrix(XMMATRIX in_scale_matrix);
		void SetTranslationMatrix(float in_x, float in_y, float in_z);
		void SetRotationMatrix(float in_radian);
		BoundingBox GetLocalBoundingBox();
		void Draw(ComPtr<ID3D11DeviceContext>  in_g_pImmediateContext, GameObjectRender &in_render);
		void Draw(ComPtr<ID3D11DeviceContext>  in_g_pImmediateContext, ShadowMapRender &in_render);
	private:
		std::vector<GameObject> m_model_sub_meshes;
		BoundingBox m_bounding_box;
		std::string m_model_name;
	};
}

#endif
