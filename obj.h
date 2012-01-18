#ifndef _OBJ_H
#define _OBJ_H
#include <string>
using namespace std; 

struct ObjVertex{
	float x, y, z; 
}; 
typedef ObjVertex ObjNormal; 

struct ObjTexCoord{
	float u, v; 
}; 
struct ObjDiffuse{
	float r, g, b; 
};
typedef ObjDiffuse ObjAmbient; 
typedef ObjDiffuse ObjSpecular; 

struct ObjMaterial{
	char material[256]; //material name for loading
	char matindex[256]; //256 materials supported.
	char uvmap[256]; //uvmap name for applying textures...

	int nAmbient, nDiffuse, nSpecular, nMatIndex, nUVMap; //count PER material. MUST STAY HERE

	//Per Material Parsing Info
	ObjAmbient*    AmbientArray; //sizeof current material ambient count
	ObjDiffuse*    DiffuseArray; //sizeof current material diffuse count
	ObjSpecular*   SpecularArray; //sizeof current material specular count


};

struct ObjTriangle{
	int Vertex[3]; 
	int Normal[3]; 
	int TexCoord[3]; 
}; 

struct ObjModel{
	unsigned int nVertex, nNormal, nTexCoord, nTriangle; //object parsing
	int nMaterial;

/*OBJECT PARSING ARRAYS*/
	ObjVertex* VertexArray; 
	ObjNormal* NormalArray; 
	ObjTexCoord* TexCoordArray; 
	ObjTriangle* TriangleArray; //indexing

/*MATERIAL PARSING ARRAYS*/
	ObjMaterial*   MaterialArray; // indexing
}; 

ObjModel* ObjLoadModel(char*, size_t); 
size_t    ObjLoadFile(char*, char**);
void      ParseMaterial(ObjModel*, char*, size_t, int); 
void      CreateXmodelExport(char*, ObjModel*); 

#endif