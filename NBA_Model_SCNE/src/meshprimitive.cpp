#include <meshprimitive.h>
#include <databuffer.h>
#include <common.h>
#include <bin_codec.h>

bool USE_DEBUG_LOGS   = false;
bool INCLUDE_LODS     = false;
uint8_t WEIGHT_BITS   = 16;

void decodeOctahedralNorms(CDataBuffer* tanFrameBf, Mesh& mesh)
{
	if (tanFrameBf->getFormat() != "R10G10B10A2_UINT")
		return;

	Vec3 normal;
	Vec4 encoded;
	auto& data = tanFrameBf->data;

	for (int i = 0; i < data.size(); i += 4)
	{
		encoded = { data[i + 0], data[i + 1], data[i + 2], data[i + 3] };

		normal.x = (encoded.x * (1.0f / 1023.0f)) * 2.0f - 1.0f;
		normal.y = (encoded.y * (1.0f / 1023.0f)) * 2.0f - 1.0f;
		normal.z = 1.0f - fabsf(normal.x) - fabsf(normal.y);

		float t = fmaxf(-normal.z, 0.0f);
		normal.x += (normal.x >= 0.0f) ? -t : t;
		normal.y += (normal.y >= 0.0f) ? -t : t;
		normal.normalize();

		// push vertex data
		mesh.normals.push_back(normal.x);
		mesh.normals.push_back(normal.y);
		mesh.normals.push_back(normal.z);
	};

};

void StGeoLOD::parse(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
		case enPrimTag::PM_START:
			start = it.value();
			break;
		case enPrimTag::PM_COUNT:
			count = it.value();
			break;
		default:
			break;
		};
	}
}

void StGeoPrim::load(JSON& obj)
{
	for (JSON::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		auto key = common::chash(it.key());

		switch (key)
		{
		case enPrimTag::PM_BLENDINDEXRANGE:
			blendIndexRange = { it.value()[0], it.value()[1] };
			break;
		case enPrimTag::PM_MATERIAL:
			material_name = it.value();
			break;
		case enPrimTag::PM_MESH:
			name = it.value();
			break;
		case enPrimTag::PM_TYPE:
			type = it.value();
			break;
		case enPrimTag::PM_START:
			data_begin = it.value();
			break;
		case enPrimTag::PM_COUNT:
			count = it.value(); // (it.value() * UINT16_MAX ) / 3
			break;
		case enPrimTag::PM_DUV_0:
			uv_deriv.push_back(it.value());
			break;
		case enPrimTag::PM_DUV_1:
			uv_deriv.push_back(it.value());
			break;
		case enPrimTag::PM_DUV_2:
			uv_deriv.push_back(it.value());
			break;
		case enPrimTag::PM_LODLIST:
			for (auto& child : it.value())
			{	// Get prim LOD data from JSON array
				StGeoLOD geo;
				geo.parse(child);
				this->lods.push_back(geo);
			}
			break;
		default:
			break;
		};
	}
}

void GeomDef::pushPrimLods(StGeoPrim& prim, std::vector<StGeoPrim>& prim_vec)
{
	if (prim.lods.empty())
	{
		// No lods - only push main prim geo
		prim_vec.push_back(prim);
		return;
	}

	int num_lods = (INCLUDE_LODS) ? prim.lods.size() : 1;

	for (int i = 0; i < num_lods; i++)
	{
		auto& lod = prim.lods[i];

		// Inherit source geometry from primitive but alter triangle list
		StGeoPrim newPrim = prim;
		newPrim.data_begin = lod.start;
		newPrim.count = lod.count;

		// Format and push to scene.
		newPrim.name += (i > 0) ? "_LOD" + std::to_string(i) : "";
		prim_vec.push_back(newPrim);
	}
}

void GeomDef::setMeshVtxs(CDataBuffer* posBf, Mesh& mesh)
{
	// calculate total components
	BinaryCodec codec(posBf->getEncoding(), posBf->getType());
	auto numChannels = codec.num_channels();
	bool usingWAxis  = (numChannels == 4);

	/* Format vertex coord mesh data - ignore every W position coord */
	for (int i = 0; i < posBf->data.size(); i++)
	{
		auto& vtx = posBf->data[i];

		if (!usingWAxis)
		{
			mesh.vertices.push_back(vtx);
		}
		else if ((i + 1) % 4 != 0)
		{	// skip W
			mesh.vertices.push_back(vtx);
		}
	}

	// Update each vertex transform 
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		auto& vtx = mesh.vertices[i];

		if (!posBf->translate.empty() && !posBf->scale.empty())
		{
			// Apply Scale and Offset
			auto& scale  = posBf->scale[i % 3];
			auto& offset = posBf->translate[i % 3];

			vtx = (vtx * scale) + offset;
		}
	}
}

void GeomDef::addMeshUVMap(CDataBuffer* texBf, Mesh& mesh)
{
	/* Format uv coord mesh data - ignore every W position coord */
	UVMap channel{ texBf->id };

	for (int i = 0; i < texBf->data.size(); i++)
	{
		auto coord = texBf->data[i];

		if (!texBf->translate.empty() && !texBf->scale.empty())
		{
			// Apply Scale and Offset transforms
			auto& scale = texBf->scale[i % 2];
			auto& offset = texBf->translate[i % 2];

			coord = (coord * scale) + offset;
		}

		// Flip Y-Axis
		coord = (i % 2 != 0) ? -(coord - 1.0f) : coord;

		channel.map.push_back(coord);
	}

	mesh.uvs.push_back(channel);
}

void GeomDef::calculateVtxNormals(CDataBuffer* tanBf, Mesh& mesh)
{
	auto size = tanBf->data.size();
	if (size == 0 || size % 4 != 0)
		return;

	// Unpack data
	mesh.tangent_frames = tanBf->data;
	::decodeOctahedralNorms(tanBf, mesh);
}

