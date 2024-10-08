#include <meshstructs.h>
#include <json.hpp>
#pragma once

using JSON = nlohmann::ordered_json;

// GLOBAL EXPORT SETTINGS
extern bool USE_DEBUG_LOGS;   // toggles debug logs  - (could be preprocessor def but this lets client toggle)
extern bool INCLUDE_LODS;     // toggles lower level meshes
extern uint8_t WEIGHT_BITS;

// Known prim JSON keys
enum enModelData {
	TRANSFORM = 2483404129,
	PRIM = 2089476285,
	VERTEXFORMAT = 4088860140,
	VERTEXSTREAM = 308573839,
	INDEXBUFFER = 498649527,
	WEIGHTBITS = 1694376895,
	MATRIXWEIGHTBUFFER = 2286398671,
	MORPH = 229585099
};

enum enPrimTag {
	PM_BLENDINDEXRANGE = 483033871,
	PM_MATERIAL = 3427514740,
	PM_MESH = 2089354642,
	PM_TYPE = 2089627879,
	PM_COUNT = 217729102,
	PM_DUV_0 = 2089048676,
	PM_DUV_1 = 2089048677,
	PM_DUV_2 = 2089048678,
	PM_LODLIST = 3895677344,
	PM_START = 236861875
};

// Mesh primitive data structs - unpacked JSON object linked with databuffer
struct StGeoLOD
{
	int count = NULL;
	int start = NULL;

	void parse(JSON& json);
};

struct StGeoPrim
{
	std::string name;
	std::string type;
	std::string material_name;

	int count = NULL;
	int64_t data_begin = -1;

	Vec2 blendIndexRange{ 0,0 };
	std::vector<Array2D> uv_deriv;
	std::vector<StGeoLOD> lods;
	std::shared_ptr<Mesh> mesh;

	void load(JSON& json);
};

// Prim Build+Helper Methods
class CDataBuffer;
namespace GeomDef
{
	void pushPrimLods(StGeoPrim& prim, std::vector<StGeoPrim>& prim_vec);
	void setMeshVtxs(CDataBuffer* posBf, Mesh& mesh);
	void calculateVtxNormals(CDataBuffer* tanBf, Mesh& mesh);
	void addMeshUVMap(CDataBuffer* texBf, Mesh& mesh);
};

