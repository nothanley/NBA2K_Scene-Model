#include <sceneupdate.h>
#include <common.h>
#include <scenereader.h>
#include <modelreader.h>
#include <nbamodel.h>
#include <sstream>
#include <bin_codec.h>

CSceneUpdate::CSceneUpdate(const char* path, const bool fix_mesh)
	:
	CSceneFile(path),
	m_data(NULL),
	m_numVtxComponents(3),
	doMeshFix(fix_mesh)
{
}

CSceneUpdate::~CSceneUpdate()
{
}

void CSceneUpdate::clear()
{
	m_data = nullptr;
}

inline static void transformVertices( const CDataBuffer* vertexBuffer, std::vector<float>& verts, const int numChannels)
{
	auto& tfmScale  = vertexBuffer->scale;
	auto& tfmOffset = vertexBuffer->translate;

	// Perform object transforms
	for (int i = 0; i < verts.size(); i++)
	{
		auto& vtx = verts[i];

		if (!tfmOffset.empty() && !tfmScale.empty())
		{
			// Apply Scale and Offset
			auto& scale  = tfmScale [i % numChannels];
			auto& offset = tfmOffset[i % numChannels];

			vtx = (vtx - offset) / scale;
		}
	}
}

inline static void EncodeOctahedralNormal(Vec4* Encoded, const Vec3& Normal)
{
	float s = 1.0f / (fabsf(Normal.x) + fabsf(Normal.y) + fabsf(Normal.z));
	float nx = Normal.x * s;
	float ny = Normal.y * s;
	float nz = Normal.z * s;
	if (nz >= 0.0) {
		float octWrapX = (1.0f - fabsf(ny)) * (nx >= 0.0f ? 1.0f : -1.0f);
		float octWrapY = (1.0f - fabsf(nx)) * (ny >= 0.0f ? 1.0f : -1.0f);
		nx = octWrapX;
		ny = octWrapY;
	}
	Encoded->x = int((nx * 0.5f + 0.5f) * 1023.0f);
	Encoded->y = int((ny * 0.5f + 0.5f) * 1023.0f);
}

inline static Vec3 decodeOctahedralNormal(const Vec3& encoded)
{
	Vec3 normal;

	normal.x = (encoded.x * (1.0f / 1023.0f)) * 2.0f - 1.0f;
	normal.y = (encoded.y * (1.0f / 1023.0f)) * 2.0f - 1.0f;
	normal.z = 1.0f - fabsf(normal.x) - fabsf(normal.y);

	float t = fmaxf(-normal.z, 0.0f);
	normal.x += (normal.x >= 0.0f) ? -t : t;
	normal.y += (normal.y >= 0.0f) ? -t : t;
	normal.normalize();

	return normal;
};

void CSceneUpdate::update(StUpdatePkg* data)
{
	/* Update scene source*/
	m_data = data;
	printf("\n[CInterface] .SCNE File ready for inject. Inject Method: %d", m_data->search_method);

	/* Perform update*/
	this->findTarget();
	this->updateTarget();

	/* Reset member data */
	this->clear();
}

void CSceneUpdate::findTarget()
{
	if (!m_data) return;
	
	//printf("\n[CSceneUpdate] Searching for mesh - NumVerts: %d- NumTris: %d...", m_data->numVerts, m_data->numFaces);

	for (auto& model : m_scene->models()) 
		for (auto& mesh : model->getMeshes())
		{
			bool hasVtxMatch = (mesh->vertices.size()  == m_data->numVerts);
			bool hasTriMatch = (mesh->triangles.size() == m_data->numFaces);

			if (hasVtxMatch && hasTriMatch) 
			{	// All mesh criteria met - store mesh target
				m_targetMesh = mesh;
				return;
			}
		}
}

void CSceneUpdate::updateTarget()
{
	if (!m_targetMesh) return;

	// Assign vertex ids
	printf("\n[CSceneUpdate] Updating mesh: %s\n", m_targetMesh->name.c_str());
	this->buildVertexMap();
	
	// Build Update Mesh
	m_updateMesh = std::make_shared<Mesh>();
	this->getUpdatedVertices();
	this->getUpdatedNormals();

	// Update streams
	this->updateVertexBuffer();
	this->updateTangentBuffer();

	// Log status
	common::ShowMessageBox( "Updated Scene Mesh: " + m_targetMesh->name );
}

void CSceneUpdate::buildVertexMap()
{
	m_vertexMap.clear();
	auto& scan_type = m_data->search_method;
	auto& numVerts  = m_data->numVerts;

	// Use Vertex ID Scan
	for (int i = 0; i < numVerts; i++){
		this->m_vertexMap.push_back(i);
	};
};

void CSceneUpdate::getUpdatedVertices()
{
	// Initialize vertex buffer
	auto posBf = (CDataBuffer*)m_targetMesh->vertex_ref;
	if (!posBf) throw std::runtime_error("Cannot load empty vertex buffer.");

	// Get stream attributes
	BinaryCodec codec(posBf->getEncoding(), posBf->getType());
	auto& verts        = m_updateMesh->vertices;
	m_numVtxComponents = codec.num_channels();
	bool  hasCoordW    = m_numVtxComponents == 4;

	// Build data vector
	for (int i = 0; i < m_vertexMap.size(); i++)
	{
		auto& index      = m_vertexMap[i];
		auto& new_vertex = m_data->position[index];

		verts.push_back(new_vertex);

		// Insert W value if needed
		if (hasCoordW && ((i+1) % 3 == 0))
			verts.push_back(1.0f);
	}

	// Perform transforms
	::transformVertices(posBf, verts, m_numVtxComponents);
	if (doMeshFix) m_updateMesh->alignPosition(true, m_numVtxComponents);
};

inline static void updateTangentFrameVec(const std::vector<float>& tanFrames, const std::vector<float>& normals, std::vector<float>& dst)
{
	// Validate mesh vectors
	int numTans = tanFrames.size() / 4;
	int numNrms = normals.size() / 3;
	if (numTans != numNrms)
		throw std::runtime_error("Unsupported tangent frame encoding. Failed to update.");

	// Encode octahedral normals...
	for (int i = 0; i < numTans; i++)
	{
		Vec4 encoded{
			tanFrames[(i * 4)],
			tanFrames[(i * 4) + 1],
			tanFrames[(i * 4) + 2],
			tanFrames[(i * 4) + 3]
		};

		Vec3 normal{
			normals[(i * 3)],
			normals[(i * 3) + 1],
			normals[(i * 3) + 2] };

		EncodeOctahedralNormal(&encoded, normal);

		dst.push_back(encoded.x);
		dst.push_back(encoded.y);
		dst.push_back(encoded.z);
		dst.push_back(encoded.w);
	}
}
void CSceneUpdate::getUpdatedNormals()
{
	// Initialize vertex buffer
	auto tanBf = (CDataBuffer*)m_targetMesh->normals_ref;
	if (!tanBf || tanBf->getFormat() != "R10G10B10A2_UINT")
		throw std::runtime_error("Unsupported tangent frame encoding. Failed to update.");

	// Get stream attributes & update mesh normals
	auto& normals    = m_updateMesh->normals;
	auto& tanFrames  = m_targetMesh->tangent_frames;

	for (int i = 0; i < m_vertexMap.size(); i++)
	{
		auto& index  = m_vertexMap[i];
		auto& normal = m_data->normals[index];
		normals.push_back(normal);
	}
	if (doMeshFix) m_updateMesh->alignNormals(true);

	// Update tangent frame vectors 
	updateTangentFrameVec(tanFrames, normals, m_updateMesh->tangent_frames);
}

void CSceneUpdate::updateVertexBuffer()
{
	auto posBf = (CDataBuffer*)m_targetMesh->vertex_ref;

	if (!posBf) {
		printf("\n[CSceneUpdate] Failed to locate mesh data.");
		return;
	}

	// Get updated mesh data vector
	auto& mesh_data = m_updateMesh->vertices;
	auto  buffer    = posBf->getBinary();
	char* src       = (char*)buffer.data();

	// update/overwrite binary data
	BinaryCodec codec( posBf->getEncoding(), posBf->getType() );
	codec.update(src, mesh_data.size(), mesh_data, posBf->getDataOffset(), posBf->getStride());

	// save buffer to source file
	posBf->saveBinary(src, buffer.size());
}

void CSceneUpdate::updateTangentBuffer()
{
	auto tanBf = (CDataBuffer*)m_targetMesh->normals_ref;

	if (!tanBf) {
		printf("\n[CSceneUpdate] Failed to locate mesh data.");
		return;
	}

	// Get updated mesh data vector
	auto& mesh_data = m_updateMesh->tangent_frames;
	auto  buffer    = tanBf->getBinary();
	char* src       = (char*)buffer.data();

	// update/overwrite binary data
	BinaryCodec codec(tanBf->getEncoding(), tanBf->getType() );
	codec.update(src, mesh_data.size(), mesh_data, tanBf->getDataOffset(), tanBf->getStride());

	// save buffer to source file
	tanBf->saveBinary(src, buffer.size());
}


