#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "obj.h"

using namespace std; 

int main(){
	char* memory = NULL; 
	string stripped;  
	string extend_obj = ".obj";
	string extend_mtl = ".mtl";
	string extend_xmodel = ".XMODEL_EXPORT"; 
	string obj_extended;
	string mtl_extended;
	string xmodel_extended;
	char obj_filename[256];
	//char mtl_filename[256];
	//string temp_mtl_filename;
	char xmodel_filename[256];
	

    cout << "TOXEC - The OBJ to Xmodel_Export Converter" << endl;
	cout << "Version 0.2 - Hacked Together by Silicone_Milk" << endl;
	cout << "Date: 2/15/08 at 10:26pm (-8 GMT)" << endl;
	cout << "Revision Date: 1/5/09 at 12:07am (-8 GMT)" << endl;
	cout << "-------------------------------------" << endl;
	cout << "Enter the path to your OBJ file:" << endl;
	getline(cin, stripped, '\.');
	obj_extended = stripped + extend_obj; 
	strcpy(obj_filename, obj_extended.c_str());
	
	xmodel_extended = stripped + extend_xmodel; 
	strcpy(xmodel_filename, xmodel_extended.c_str()); 


    size_t bytes = ObjLoadFile(obj_filename, &memory);
    ObjModel* model = ObjLoadModel(memory, bytes);
	cout << "successfully opened:" << " " << obj_filename << endl;

	for(int i=0;i < model->nMaterial;i++){
		cout << "looking up: " << model->MaterialArray[i].material << "\n";
		cout << i << endl;
		size_t bytes2 = ObjLoadFile(model->MaterialArray[i].material, &memory);
		ParseMaterial(model, memory, bytes2, i);
	}

	CreateXmodelExport(xmodel_filename, model); 

	//free(model->NormalArray);
	//free(model->TexCoordArray);
	//free(model->VertexArray); 
	//free(model->TriangleArray);
	//free(model->MaterialArray->AmbientArray);
	//free(model->MaterialArray->DiffuseArray);
	//free(model->MaterialArray->SpecularArray);
	//free(model->MaterialArray);
	//free(model); 

	system("PAUSE"); 
	
	return 0; 
}
