# include <cstdlib>
# include <cmath>
# include <cstdio>
# include <iostream>
# include <iomanip>
# include <fstream>
# include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "obj.h"

using namespace std; 

void ParseMaterial(ObjModel* data, char* memory, size_t size, int i);
//------------------------------------------------------------------------------------


ObjModel* ObjLoadModel(char* memory, size_t size)
{
	char* p = NULL, * e = NULL;
   ObjModel* ret = (ObjModel*) calloc(1, sizeof(ObjModel));
   memset(ret, 0, sizeof(ObjModel));

	p = memory;
	e = memory + size;
	
	while (p != e)
	{
		   if (memcmp(p, "mtllib", 6) == 0) ret->nMaterial++; 
      else if (memcmp(p, "vn", 2) == 0) ret->nNormal++;
      else if (memcmp(p, "vt", 2) == 0) ret->nTexCoord++;
      else if (memcmp(p, "v",  1) == 0) ret->nVertex++;
      else if (memcmp(p, "f",  1) == 0) ret->nTriangle++;
	  

      while (*p++ != (char) 0x0A);
   }
	cout << "There are: " << ret->nMaterial << " materials" << endl;
   ret->VertexArray   = (ObjVertex*)   malloc(sizeof(ObjVertex) * ret->nVertex);
   ret->NormalArray   = (ObjNormal*)   malloc(sizeof(ObjNormal) * ret->nNormal++);
   ret->TexCoordArray = (ObjTexCoord*) malloc(sizeof(ObjTexCoord) * ret->nTexCoord++);
   ret->TriangleArray = (ObjTriangle*) malloc(sizeof(ObjTriangle) * ret->nTriangle);
   ret->MaterialArray = (ObjMaterial*) malloc(sizeof(ObjMaterial) * ret->nMaterial);

	p = memory;
	
   int nV = 0, nN = 1, nT = 1, nF = 0, nM = 0;
	
	while (p != e)
	{
      if (memcmp(p, "vn", 2) == 0)
      {
         sscanf(p, "vn %f %f %f", &ret->NormalArray[nN].x,
                                  &ret->NormalArray[nN].y,
                                  &ret->NormalArray[nN].z);
         nN++;
      }
      else if (memcmp(p, "vt", 2) == 0)
      {
         sscanf(p, "vt %f %f", &ret->TexCoordArray[nT].u,
                               &ret->TexCoordArray[nT].v);
         nT++;
      }
      else if (memcmp(p, "v", 1) == 0) 
      {
         sscanf(p, "v %f %f %f", &ret->VertexArray[nV].x,
                                 &ret->VertexArray[nV].y,
                                 &ret->VertexArray[nV].z);
         nV++;
      }
      else if (memcmp(p, "f", 1) == 0)
      {
         sscanf(p, "f %d/%d/%d %d/%d/%d %d/%d/%d", &ret->TriangleArray[nF].Vertex[0],
                                                   &ret->TriangleArray[nF].TexCoord[0],
                                                   &ret->TriangleArray[nF].Normal[0],
                                                   &ret->TriangleArray[nF].Vertex[1],
                                                   &ret->TriangleArray[nF].TexCoord[1],
                                                   &ret->TriangleArray[nF].Normal[1],
                                                   &ret->TriangleArray[nF].Vertex[2],
                                                   &ret->TriangleArray[nF].TexCoord[2],
                                                   &ret->TriangleArray[nF].Normal[2]);
         nF++;
      }
	  else if (memcmp(p, "mtllib", 6) == 0){
		  sscanf(p, "mtllib %s", &ret->MaterialArray[nM].material);
		  nM++;
	  }
	  
      while (*p++ != (char) 0x0A);
   }
     
   return ret;
}

void ParseMaterial(ObjModel* data, char* memory, size_t size, int i){
	int acount = 0;
	int dcount = 0;
	int scount = 0;
	int mcount = 0;
	int uvmap = 0;
    char* p = NULL, * e = NULL;
	p = memory;
	e = memory + size;
	cout << "Successfully opened the material..." << "\n";
	//cout << i << "\n";
	
		while (p != e)
		{
		       if (memcmp(p, "Ka", 2) == 0) acount++;
		  else if (memcmp(p, "Kd",  2) == 0) dcount++;
		  else if (memcmp(p, "Ks",  2) == 0) scount++;
		  else if (memcmp(p, "newmtl", 6) == 0) mcount++;
		  else if (memcmp(p, "map_Kd", 6) == 0) uvmap++;

		  while (*p++ != (char) 0x0A);
		 }
		
		/* 
		ONLY IF AMBIENT/DIFFUSE/SPECULAR ARE A NON-ZERO VALUE
		*/
		
		data->MaterialArray[i].nAmbient = acount;
		data->MaterialArray[i].nDiffuse = dcount; 
		data->MaterialArray[i].nSpecular = scount;
		data->MaterialArray[i].nMatIndex = mcount;
		
		data->MaterialArray[i].AmbientArray  = (ObjAmbient*)   malloc(sizeof(ObjAmbient) * data->MaterialArray[i].nMatIndex);
		data->MaterialArray[i].DiffuseArray  = (ObjDiffuse*)   malloc(sizeof(ObjDiffuse) * data->MaterialArray[i].nMatIndex);
		data->MaterialArray[i].SpecularArray  = (ObjSpecular*)   malloc(sizeof(ObjSpecular) * data->MaterialArray[i].nMatIndex);
		/* 
		OR ELSE WE ALLOCATE ONE ARRAY ELEMENT AND ZERO IT OUT
		*/

		cout << "INDEX:" << data->MaterialArray[i].nMatIndex << endl;
		


		//cout << data->MaterialArray[i].nAmbient << data->MaterialArray[i].nDiffuse << data->MaterialArray[i].nSpecular << endl;

	p = memory;

	int nA = 0, nD = 0, nS = 0;

	while (p != e)
	{   //SANITY CHECK

		
		//VERY UNSAFE
		if(memcmp(p, "map_Kd", 6) == 0){
			sscanf(p, "map_Kd %s", &data->MaterialArray[i].uvmap);
		}
		//END VERY UNSAFE

		if(data->MaterialArray[i].nAmbient == data->MaterialArray[i].nMatIndex){
		  if (memcmp(p, "Ka", 2) == 0)
		  {
			  if(nA < data->MaterialArray[i].nAmbient){
		    sscanf(p, "Ka %f %f %f", &data->MaterialArray[i].AmbientArray[nA].r,
		                             &data->MaterialArray[i].AmbientArray[nA].g,
		                             &data->MaterialArray[i].AmbientArray[nA].b);
			nA++;
			  }
		  }
		}else {
			//nA = data->MaterialArray[i].nAmbient;
			if(nA < data->MaterialArray[i].nMatIndex){
				data->MaterialArray[i].AmbientArray[nA].r = 0;
				data->MaterialArray[i].AmbientArray[nA].g = 0;
				data->MaterialArray[i].AmbientArray[nA].b = 0;
				nA++;
			}
		}

		if(data->MaterialArray[i].nDiffuse == data->MaterialArray[i].nMatIndex){
			if(memcmp(p, "Kd", 2) == 0){
				 if(nD < data->MaterialArray[i].nDiffuse){
				  sscanf(p, "Kd %f %f %f", &data->MaterialArray[i].DiffuseArray[nD].r,
										   &data->MaterialArray[i].DiffuseArray[nD].g,
										   &data->MaterialArray[i].DiffuseArray[nD].b); 
				  nD++;
			  }
			}
		}else {
			//nD = data->MaterialArray[i].nDiffuse;
			if(nD < data->MaterialArray[i].nMatIndex){
				data->MaterialArray[i].DiffuseArray[nD].r = 0;
				data->MaterialArray[i].DiffuseArray[nD].g = 0;
				data->MaterialArray[i].DiffuseArray[nD].b = 0;
				nD++;
			}
		}
		if(data->MaterialArray[i].nSpecular == data->MaterialArray[i].nMatIndex){
			 if(memcmp(p, "Ks", 2) == 0){
				 if(nS < data->MaterialArray[i].nSpecular){
				 sscanf(p, "Ks %f %f %f", &data->MaterialArray[i].SpecularArray[nS].r,
				  	                      &data->MaterialArray[i].SpecularArray[nS].g,
									      &data->MaterialArray[i].SpecularArray[nS].b); 
			 nS++;
			 }
			}
		}else{
			//nS = data->MaterialArray[i].nSpecular; 
			if(nS < data->MaterialArray[i].nMatIndex){
				data->MaterialArray[i].SpecularArray[nS].r = 0;
				data->MaterialArray[i].SpecularArray[nS].g = 0;
				data->MaterialArray[i].SpecularArray[nS].b = 0;
				nS++;
			}
		}

	while (*p++ != (char) 0x0A);
	}

}


size_t ObjLoadFile(char* szFileName, char** memory)
{
	cout << "Attempting to open:" << szFileName << endl;
	size_t bytes = 0;
	FILE* file = fopen(szFileName, "rt");

	if (file != NULL)
   {
	   fseek(file, 0, SEEK_END);
	   size_t end = ftell(file);
	   fseek(file, 0, SEEK_SET);
   	
	   *memory = (char*) malloc(end);
	   bytes = fread(*memory, sizeof(char), end, file);

	   fclose(file);
   }
	return bytes;
}

void CreateXmodelExport(char* fileout, ObjModel* data){
	int a = 0, d = 0, s = 0;
	int n = 0;
	ofstream fout(fileout); 
	fout << "//Call of Duty 4 XMODEL_EXPORT File - Generated by Silicone_Milk's TOXEC Program" << "\n";
	fout << "//Homepage: http://www.codmapping.com" << "\n";
	fout << "//Email: silicone.milk@nerdcorestudios.net" << "\n";
	fout << "\n" << "MODEL" << "\n";
	fout << "VERSION 6" << "\n"; 
	fout << "\n" << "NUMBONES 1" << "\n";
	fout << "BONE 0 -1 \"TAG_ORIGIN\"" << "\n";
	fout << "\n" << "BONE 0" << "\n";
	fout << "OFFSET" << " " << "0.000000," << " " << "0.000000," << " " << "0.000000" << "\n";
	fout << "SCALE" << " " << "1.000000," << " " << "1.000000," << " " << "1.000000" << "\n";
	fout << "X" << " " << "1.000000," << " " << "0.000000," << " " << "0.000000" << "\n";
	fout << "Y" << " " << "0.000000," << " " << "1.000000," << " " << "0.000000" << "\n";
	fout << "Z" << " " << "0.000000," << " " << "0.000000," << " " << "1.000000" << "\n";
	//verts
	fout << "\n" << "NUMVERTS" << " " << data->nVertex << "\n";
	for(int i=0; i < data->nVertex; i++){
		fout << "VERT" << " " << i << "\n";
		fout << "OFFSET" << " "
			 << data->VertexArray[i].x << " "
			 << data->VertexArray[i].y << " "
			 << data->VertexArray[i].z << "\n";
		fout << "BONES" << " " << "1" << "\n";
		fout << "BONE" << " " << "0" << " " << "1.000000" << "\n";
	}

	//fout << "\n" << "NUMNORMALS" << " " << data->nNormal << "\n";
	fout << "\n" << "NUMFACES" << " " << data->nTriangle << "\n";
	int vertex;
	int vertex2;
	int vertex3;
	for(int l =0; l < data->nTriangle; l++){
		fout << "TRI" << " " << "0 0 0 0" << "\n";
		vertex = data->TriangleArray[l].Vertex[0];
		vertex2 = data->TriangleArray[l].Vertex[1];
		vertex3 = data->TriangleArray[l].Vertex[2];
	    fout << "VERT" << " " << (vertex - 1) << "\n";
		fout << fixed << "NORMAL" << " " << data->NormalArray[data->TriangleArray[l].Normal[0]].x * -1 << " "
			                    << data->NormalArray[data->TriangleArray[l].Normal[0]].y * -1<< " "
								<< data->NormalArray[data->TriangleArray[l].Normal[0]].z * -1<< "\n";
		fout << fixed << "COLOR" << " " << data->MaterialArray[0].DiffuseArray[0].r << " "
			                            << data->MaterialArray[0].DiffuseArray[0].g << " " 
										<< data->MaterialArray[0].DiffuseArray[0].b << " "
										<< "1.000000" << "\n";
		fout << "UV" << " " << "1" << " "
							<< data->TexCoordArray[data->TriangleArray[l].TexCoord[0]].u << " "
							<< data->TexCoordArray[data->TriangleArray[l].TexCoord[0]].v << "\n";
		fout << "VERT" << " " << (vertex2 - 1) << "\n";
		fout << fixed << "NORMAL" << " " << data->NormalArray[data->TriangleArray[l].Normal[1]].x * -1<< " "
			                    << data->NormalArray[data->TriangleArray[l].Normal[1]].y * -1<< " "
								<< data->NormalArray[data->TriangleArray[l].Normal[1]].z  * -1<< "\n";
		fout << fixed << "COLOR" << " " << data->MaterialArray[0].DiffuseArray[0].r << " "
			                            << data->MaterialArray[0].DiffuseArray[0].g << " " 
										<< data->MaterialArray[0].DiffuseArray[0].b << " "
										<< "1.000000" << "\n";
		fout << "UV" << " " << "1" << " "
							<< data->TexCoordArray[data->TriangleArray[l].TexCoord[1]].u << " "
							<< data->TexCoordArray[data->TriangleArray[l].TexCoord[1]].v << "\n";
		fout << "VERT" << " " << (vertex3 - 1) << "\n";
		fout << fixed << "NORMAL" << " " << data->NormalArray[data->TriangleArray[l].Normal[2]].x * -1<< " "
			                    << data->NormalArray[data->TriangleArray[l].Normal[2]].y * -1<< " "
								<< data->NormalArray[data->TriangleArray[l].Normal[2]].z * -1<< "\n";
		fout << fixed << "COLOR" << " " << data->MaterialArray[0].DiffuseArray[0].r << " "
			                            << data->MaterialArray[0].DiffuseArray[0].g << " " 
										<< data->MaterialArray[0].DiffuseArray[0].b << " "
										<< "1.000000" << "\n";
		fout << "UV" << " " << "1" << " "
							<< data->TexCoordArray[data->TriangleArray[l].TexCoord[2]].u << " "
							<< data->TexCoordArray[data->TriangleArray[l].TexCoord[2]].v << "\n";
	}
	fout << "\n" << "NUMOBJECTS" << " " << "1" << endl;
	fout << "OBJECT" << " " << "0" << " " << "\"the_object\"" << endl;
	for(int i = 0; i<data->nMaterial; i++){
		for(int index = 0; index < data->MaterialArray[i].nMatIndex; index++){
			fout << "\n" << "NUMMATERIALS" << " " << data->nMaterial << endl;
			int material_num = data->nMaterial;
			char the_uv[1000];
			int length = ((strlen(data->MaterialArray[i].uvmap)) - 4);
			for(int k = 0; k < length; k++){
                the_uv[k] = data->MaterialArray[i].uvmap[k];
			}
			fout << "MATERIAL" << " " << (material_num - 1) << " " << "\"" << the_uv << "_col\""
				 << " " << "\"Lambert\"" << " " << "\"" << data->MaterialArray[i].uvmap << "\"" << endl;
			fout << "COLOR" << " " << "0.000000" << " " << "0.000000" << " " << "0.000000" << " " << "1.000000" << endl;
			fout << "TRANSPARENCY" << " " << "0.000000" << " " << "0.000000" << " " << "0.000000" << " " << "1.000000" << endl;
			fout << "AMBIENTCOLOR" << " " << "0.000000" << " " << "0.000000" << " " << "0.000000" << " " << "1.000000" << endl;
			fout << "INCANDESCENCE" << " " << "0.000000" << " " << "0.000000" << " " << "0.000000" << " " << "1.000000" << endl;
			fout << "COEFFS" << " " << "0.800000" << " " << "0.000000" << endl;
			fout << "GLOW" << " " << "0.000000" << " " << "0.000000" << endl;
			fout << "REFRACTIVE" << " " << "0" << " " << "1.000000" << endl;
			fout << "SPECULARCOLOR" << " " << "0.000000" << " " << "0.000000" << " " << "0.000000" << " " << "1.000000" << endl;
			fout << "REFLECTIVECOLOR" << " " << "0.000000" << " " << "0.000000" << " " << "0.000000" << " " << "1.000000" << endl;
			fout << "REFLECTIVE" << " " << "0" << " " << "-1.000000" << endl;
			fout << "BLINN" << " " << "-1.000000" << " " << "-1.000000" << endl;
			fout << "PHONG" << " " << "-1.000000" << endl;
		}
	}
	fout.close();


}
