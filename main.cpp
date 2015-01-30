#include <stdlib.h>
#include <stdio.h>
#include "DIFK/DIFKFile.h"
#include "DIFK/Block.h"
//#include <windows.h>
#include <assert.h>
#include <string.h>

using namespace DIFK;

void writeObj(const char* texturePath, int vertexArraySize, float *vertexArray, int texCoordArraySize, float *texCoordArray, int normalsArraySize, float *normalsArray, int indicesArraySize, unsigned short *indicesArray) {
	printf("#Vertices\n");
	for(int i=0; i < vertexArraySize; i+=3)
		printf("v %f %f %f\n", vertexArray[i], vertexArray[i+1], vertexArray[i+2]);

	printf("#Normals\n");
	for(int i=0; i < normalsArraySize; i+=3)
		printf("vn %f %f %f\n", normalsArray[i], normalsArray[i+1], normalsArray[i+2]);

	printf("#Tex coord\n");
	for(int i=0; i < texCoordArraySize; i+=2)
		printf("vt %f %f\n", texCoordArray[i], texCoordArray[i+1]);

	printf("#Faces\n");
	for(int i=0; i < indicesArraySize; i+=3)
		printf("f %d/%d/%d %d/%d/%d %d/%d/%d\n",
				indicesArray[i]+1, indicesArray[i]+1, indicesArray[i]+1,
				indicesArray[i+1]+1, indicesArray[i+1]+1, indicesArray[i+1]+1,
				indicesArray[i+2]+1, indicesArray[i+2]+1, indicesArray[i+2]+1);
}

void writeCollada(const char* texturePath, int vertexArraySize, float *vertexArray, int texCoordArraySize, float *texCoordArray, int normalsArraySize, float *normalsArray, int indicesArraySize, unsigned short *indicesArray) {
	const char *textureFileNameStart = texturePath + strlen(texturePath) - 1;
	float scaleY = 1;

	while(textureFileNameStart > texturePath && *textureFileNameStart != '/' && *textureFileNameStart != '\\')
		textureFileNameStart--;

	textureFileNameStart++;

	printf(
		"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		"<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">\n"
		"  <asset>\n"
		"    <created>2013-07-09T20:33:25</created>\n"
		"    <modified>2013-07-09T20:33:25</modified>\n"
		"    <unit name=\"meter\" meter=\"1\"/>\n"
		"    <up_axis>Z_UP</up_axis>\n"
		"  </asset>\n"
		"  <library_images>\n"
		"    <image id=\"texture\" name=\"texture\">\n"
		"      <init_from>%s</init_from>\n"
		"    </image>\n"
		"  </library_images>\n"
		"  <library_effects>\n"
		"    <effect id=\"effect\">\n"
		"      <profile_COMMON>\n"
		"        <newparam sid=\"surface\">\n"
		"          <surface type=\"2D\">\n"
		"            <init_from>texture</init_from>\n"
		"          </surface>\n"
        "        </newparam>\n"
		"        <newparam sid=\"sampler\">\n"
		"          <sampler2D>\n"
		"            <source>surface</source>\n"
		"          </sampler2D>\n"
		"        </newparam>\n"
		"        <technique sid=\"common\">\n"
		"          <phong>\n"
		"            <emission>\n"
		"              <color sid=\"emission\">0 0 0 1</color>\n"
		"            </emission>\n"
		"            <ambient>\n"
		"              <texture texture=\"sampler\" texcoord=\"UVMap\"/>\n"
		"            </ambient>\n"
		"            <diffuse>\n"
		"              <texture texture=\"sampler\" texcoord=\"UVMap\"/>\n"
		"            </diffuse>\n"
		"            <specular>\n"
		"              <texture texture=\"sampler\" texcoord=\"UVMap\"/>\n"
		"            </specular>\n"
		"            <shininess>\n"
		"              <float sid=\"shininess\">5</float>\n"
		"            </shininess>\n"
		"            <index_of_refraction>\n"
		"             <float sid=\"index_of_refraction\">1</float>\n"
		"            </index_of_refraction>\n"
		"          </phong>\n"
        "        </technique>\n"
		"      </profile_COMMON>\n"
		"    </effect>\n"
		"  </library_effects>\n"
		"  <library_materials>\n"
		"    <material id=\"material\" name=\"anatema\">\n"
		"      <instance_effect url=\"#effect\"/>\n"
		"    </material>\n"
		"  </library_materials>\n"
		"  <library_geometries>\n"
		"    <geometry id=\"anatema_test-mesh\" name=\"anatema.test\">\n"
		"      <mesh>\n"
		"        <source id=\"anatema_test-mesh-positions\">\n"
		"          <float_array id=\"anatema_test-mesh-positions-array\" count=\"%d\">"
		"            ", textureFileNameStart, vertexArraySize);

		for(int i=0; i < vertexArraySize; i += 3)
			printf("%f %f %f ", vertexArray[i], vertexArray[i+1]*scaleY, vertexArray[i+2]);

	printf(""
		"          </float_array>\n"
		"          <technique_common>\n"
		"            <accessor source=\"#anatema_test-mesh-positions-array\" count=\"%d\" stride=\"3\">\n"
		"              <param name=\"X\" type=\"float\"/>\n"
		"              <param name=\"Y\" type=\"float\"/>\n"
		"              <param name=\"Z\" type=\"float\"/>\n"
		"            </accessor>\n"
		"          </technique_common>\n"
		"        </source>\n"
        "        <source id=\"anatema_test-mesh-normals\">\n"
		"          <float_array id=\"anatema_test-mesh-normals-array\" count=\"%d\">"
		"            ", vertexArraySize/3, normalsArraySize);

		for(int i=0; i < normalsArraySize; i+=3)
			printf("%f %f %f ", normalsArray[i], normalsArray[i+1]*scaleY, normalsArray[i+2]);

	printf(""
		"          </float_array>\n"
		"          <technique_common>\n"
		"            <accessor source=\"#anatema_test-mesh-normals-array\" count=\"%d\" stride=\"3\">\n"
		"              <param name=\"X\" type=\"float\"/>\n"
		"              <param name=\"Y\" type=\"float\"/>\n"
		"              <param name=\"Z\" type=\"float\"/>\n"
		"            </accessor>\n"
		"          </technique_common>\n"
		"        </source>\n"
        "        <source id=\"anatema_test-mesh-map-0\">\n"
		"          <float_array id=\"anatema_test-mesh-map-0-array\" count=\"%d\">"
		"            ", normalsArraySize/3, texCoordArraySize);

		for(int i=0; i < texCoordArraySize; i+=2)
			printf("%f %f ", texCoordArray[i], texCoordArray[i+1]);

	printf(""
		"          </float_array>\n"
		"          <technique_common>\n"
		"            <accessor source=\"#anatema_test-mesh-map-0-array\" count=\"%d\" stride=\"2\">\n"
		"              <param name=\"S\" type=\"float\"/>\n"
		"              <param name=\"T\" type=\"float\"/>\n"
		"            </accessor>\n"
		"          </technique_common>\n"
		"        </source>\n"
        "        <vertices id=\"anatema_test-mesh-vertices\">\n"
        "          <input semantic=\"POSITION\" source=\"#anatema_test-mesh-positions\"/>\n"
        "        </vertices>\n"
        "        <triangles count=\"%d\" material=\"material\">\n"
        "          <input semantic=\"VERTEX\" source=\"#anatema_test-mesh-vertices\" offset=\"0\"/>\n"
        "          <input semantic=\"NORMAL\" source=\"#anatema_test-mesh-normals\" offset=\"1\"/>\n"
        "          <input semantic=\"TEXCOORD\" source=\"#anatema_test-mesh-map-0\" offset=\"2\" set=\"0\"/>\n"
        "          <vcount>\n"
        "            ", texCoordArraySize/2, indicesArraySize/3);

		for(int i=0; i < indicesArraySize/3; i++)
			printf("3 ");

	printf("\n"
		"          </vcount>\n"
		"          <p>\n"
		"            ");

		for(int i=0; i < indicesArraySize; i++)
			printf("%5d %5d %5d ", indicesArray[i], indicesArray[i], indicesArray[i]);

	printf("\n"
		"          </p>\n"
        "        </triangles>\n"
		"      </mesh>\n"
		"    </geometry>\n"
		"  </library_geometries>\n"
		"  <library_visual_scenes>\n"
		"    <visual_scene id=\"Scene\" name=\"Scene\">\n"
		"      <node id=\"anatema_test\" name=\"anatema_test\" type=\"NODE\">\n"
		"	     <translate sid=\"location\">0 0 0</translate>\n"
		"	     <rotate sid=\"rotationZ\">0 0 1 0</rotate>\n"
		"	     <rotate sid=\"rotationY\">0 1 0 0</rotate>\n"
		"	     <rotate sid=\"rotationX\">1 0 0 0</rotate>\n"
		"	     <scale sid=\"scale\">1 1 1</scale>\n"
		"	     <instance_geometry url=\"#anatema_test-mesh\">\n"
		"          <bind_material>\n"
		"            <technique_common>\n"
		"              <instance_material symbol=\"material\" target=\"#material\">\n"
		"                <bind_vertex_input semantic=\"UVMap\" input_semantic=\"TEXCOORD\" input_set=\"0\"/>\n"
		"              </instance_material>\n"
		"            </technique_common>\n"
		"          </bind_material>\n"
        "        </instance_geometry>\n"
		"      </node>\n"
		"    </visual_scene>\n"
		"  </library_visual_scenes>\n"
		"  <scene>\n"
		"    <instance_visual_scene url=\"#Scene\"/>\n"
		"  </scene>\n"
		"</COLLADA>\n");
}

void parseFunc(DIFKFile* difkFile) {
	int vertexArraySize;
	int texCoordArraySize;
	int normalsArraySize;
	int indicesArraySize;

	float *vertexArray;
	float *texCoordArray;
	float *normalsArray;
	unsigned short *indicesArray;
	const char *texturePath;

	Block *nx3_mesh_block, *dataBlock;

	texturePath = difkFile->getRootBlock(0)->        //Template GUID = 209BBB41-681F-4B9B-9744-4D88E1413DCC
					getBlock(1)->        //Template array
					getBlock(0)->        //Template GUID = 52BCCAA6-3C16-4286-8B9E-1A798F9D94DE
					getBlock(1)->        //Template array
					getBlock(0)->        //Template GUID = 81BCE071-AC76-496F-9C7D-19885FD118B6
					getBlock(1)->        //String
					getData<const char*>(0);

	nx3_mesh_block = difkFile->getRootBlock(1)->        //Template GUID = A6D25AEB-A735-1FEF-C17D-EE2117498226
					getBlock(1)->        //Template array
					getBlock(0)->        //Template GUID = 1718DC1B-1DB1-458A-9C7E-C3D46FC4585B
					getBlock(4)->        //Template array
					getBlock(0);        //Template GUID = C817F7B0-E4E7-40FB-97B3-2B97CC0521

	dataBlock = nx3_mesh_block->getBlock(2)->    //Template array
					getBlock(0);        //Template GUID = 1C77954B-CDD5-4615-B7AD-F23BD3D0C23E

	vertexArraySize = dataBlock->getBlock(1)->getData<int>(0);
	vertexArray     = dataBlock->getBlock(2)->getDataPtr<float*>();
	normalsArraySize = dataBlock->getBlock(3)->getData<int>(0);
	normalsArray     = dataBlock->getBlock(4)->getDataPtr<float*>();
	texCoordArraySize = dataBlock->getBlock(5)->getData<int>(0);
	texCoordArray     = dataBlock->getBlock(6)->getDataPtr<float*>();

	indicesArraySize = nx3_mesh_block->getBlock(3)->getData<int>(0);
	indicesArray     = nx3_mesh_block->getBlock(4)->getDataPtr<unsigned short*>();

	writeObj(texturePath, vertexArraySize, vertexArray, texCoordArraySize, texCoordArray, normalsArraySize, normalsArray, indicesArraySize, indicesArray);
	//writeCollada(texturePath, vertexArraySize, vertexArray, texCoordArraySize, texCoordArray, normalsArraySize, normalsArray, indicesArraySize, indicesArray);
}

int main(int argc, char* argv[])
{
	FILE* file;
	DIFKFile* difkFile;
	const char* filename = "anatema01_body.nx3";

	if(argc > 1)
		filename = argv[1];

	file = fopen(filename, "rb");
	if(!file)
		exit(-2);

	difkFile = new DIFKFile;
	difkFile->load(file);
	fclose(file);
	difkFile->dumpToStdout();
	//parseFunc(difkFile);

    return 0;
}
