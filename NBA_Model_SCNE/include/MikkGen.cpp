#include "MikkGen.h"
#include "meshstructs.h"

MikkTCalc::MikkTCalc(Mesh* mesh)
    : m_mesh(mesh)
{
    this->setCallbacks();
}

void MikkTCalc::setCallbacks()
{
    iface.m_getNumFaces = get_num_faces;
    iface.m_getNumVerticesOfFace = get_num_vertices_of_face;

    iface.m_getNormal = get_normal;
    iface.m_getPosition = get_position;
    iface.m_getTexCoord = get_tex_coords;
    iface.m_setTSpaceBasic = set_tspace_basic;

    context.m_pInterface = &iface;
}
static void clear_mesh_tangents(Mesh* mesh)
{
    int numVerts = mesh->vertices.size() / 3;

    /* Clear old tangent space data from target mesh  */
    mesh->tangents. resize (numVerts * 4 );
    mesh->binormals.resize (numVerts);
}

void MikkTCalc::generate() 
{
    /* Clear old tangent space data from target mesh  */
    clear_mesh_tangents(this->m_mesh);

    /* Interface with Mikk struct and generate mesh tangents */
    try {
        context.m_pUserData = m_mesh;
        genTangSpaceDefault(&this->context); }
    catch (...){
        printf("\nFailed to generate tangent space.");
    }
}

int MikkTCalc::get_num_faces(const SMikkTSpaceContext* context) 
{
    Mesh* working_mesh = static_cast<Mesh*> (context->m_pUserData);
    return working_mesh->triangles.size();
}

int MikkTCalc::get_num_vertices_of_face(const SMikkTSpaceContext* context, const int iFace) 
{
    //Mesh* working_mesh = static_cast<Mesh*> (context->m_pUserData);
    return 3;
}

void MikkTCalc::get_position(const SMikkTSpaceContext* context,
    float* outpos,
    const int iFace, const int iVert) 
{
    Mesh* mesh = static_cast<Mesh*> (context->m_pUserData);

    auto& index = mesh->triangles[iFace][iVert];
    auto vertex = mesh->vertex(index);

    outpos[0] = vertex.x;
    outpos[1] = vertex.y;
    outpos[2] = vertex.z;
}

void MikkTCalc::get_normal(const SMikkTSpaceContext* context,
    float* outnormal,
    const int iFace, const int iVert)
{
    Mesh* mesh = static_cast<Mesh*> (context->m_pUserData);

    auto& index = mesh->triangles[iFace][iVert];
    auto normal = mesh->normal(index);

    outnormal[0] = normal.x;
    outnormal[1] = normal.y;
    outnormal[2] = normal.z;
}

void MikkTCalc::get_tex_coords(const SMikkTSpaceContext* context,
    float* outuv,
    const int iFace, const int iVert) 
{
    Mesh* mesh = static_cast<Mesh*> (context->m_pUserData);

    if (mesh->uvs.empty()){
        throw std::runtime_error("Mesh is missing uv's, cannot generate mikktspace.");
    }

    auto& uv_map = mesh->uvs.front();
    auto& index = mesh->triangles[iFace][iVert];
    Vec2 texcoord = uv_map.texcoord(index);

    outuv[0] = texcoord.x;
    outuv[1] = texcoord.y;
}

void MikkTCalc::set_tspace_basic(const SMikkTSpaceContext* context,
    const float* tangentu,
    const float fSign, const int iFace, const int iVert) 
{
    Mesh* mesh = static_cast<Mesh*> (context->m_pUserData);

    auto& index = mesh->triangles[iFace][iVert];
    int offset = (index * 4);

    mesh->tangents.at(offset)   = tangentu[0];
    mesh->tangents.at(offset+1) = tangentu[1];
    mesh->tangents.at(offset+2) = tangentu[2];
    mesh->binormals.at(index)   = fSign;
}

int MikkTCalc::get_vertex_index(const SMikkTSpaceContext* context, int iFace, int iVert)
{
    Mesh* mesh = static_cast<Mesh*> (context->m_pUserData);

    auto& index = mesh->triangles[iFace][iVert];
    return index;
}


