#define GLEW_STATIC

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include <GL/glut.h>

#include <cstdio>
#include <cstdlib>
#include "textfile.h"

#include "SOIL.h"


//#include <common_header.h>
//#include <VertexBufferObject.h>

#include <vector>
#include <vec3.hpp>

#include <glm.hpp>

#pragma comment(lib, "glew32s.lib")

#pragma comment(lib, "assimp.lib")

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flag


GLuint v,f,p;
float lpos[4] = {1,0.5,1,0};
double rotAngle = 5; // a global variable
double rotAngleVert= 0;
float rot = 0;
float red = 1;
float green = 1;
float blue = 1;


float lightRadius = 4;
float lightAngle = 10;
float lightZpos = lightRadius * (cos(lightAngle)+1);
float lightXpos = lightRadius * (sin(lightAngle)+1);

int tick = 0;

float scaleFactor = 1.0;
float xPos = 0.0;
float yPos = -0.5;
float zPos = 0.0;

GLuint skybox[6];
GLuint tex_cube;
GLuint skyv,skyf,skyp;

	float *vertexArray;
	float *normalArray;
	float *uvArray;
	float *tangentArray;
 
	int numVerts;
	Assimp::Importer importer;
	//GLVertexBuffer vboBumpMapData;


GLfloat* g_vp = NULL; // array of vertex points
GLfloat* g_vn = NULL; // array of vertex normals
GLfloat* g_vt = NULL; // array of texture coordinates
GLfloat* g_vtans = NULL;
int g_point_count = 0;

GLuint tex_diffuse;
GLuint tex_normal;

	bool loadModel(const char* path)
{


	DWORD dwFlags =
		aiProcess_Triangulate | // triangulate n-polygons
		aiProcess_ValidateDataStructure |
		aiProcess_ImproveCacheLocality |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_SortByPType |
		aiProcess_FindDegenerates |
		aiProcess_FindInvalidData |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_GenUVCoords |
		aiProcess_FixInfacingNormals |
		aiProcess_FlipUVs |
		aiProcess_TransformUVCoords;


	const aiScene* scene = importer.ReadFile(path, dwFlags);

	if (!scene)
	{
		printf("3dModel loading error: '%s'\n", importer.GetErrorString());
		return false;
	}

	// Now we can access the file's contents.
	// printf("Import of 3d scene %s succeeded.");


	aiMesh *mesh = scene->mMeshes[0];

	
	GLfloat* g_vtans = NULL;
	if (mesh->HasTangentsAndBitangents()) {
		printf("mesh has tangents and bitangents\n");
		g_vtans = (GLfloat*)malloc(mesh->mNumFaces * 3 * 3);
	}

	numVerts = mesh->mNumFaces * 3;

	vertexArray = new float[mesh->mNumFaces * 3 * 3];
	normalArray = new float[mesh->mNumFaces * 3 * 3];
	uvArray = new float[mesh->mNumFaces * 3 * 2];

	tangentArray = new float[mesh->mNumFaces * 3 * 3];

	for (unsigned int i = 0; i<mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];

		for (int j = 0; j<3; j++)
		{
			aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
			memcpy(uvArray, &uv, sizeof(float) * 2);
			uvArray += 2;

			aiVector3D normal = mesh->mNormals[face.mIndices[j]];
			memcpy(normalArray, &normal, sizeof(float) * 3);
			normalArray += 3;

			aiVector3D pos = mesh->mVertices[face.mIndices[j]];
			memcpy(vertexArray, &pos, sizeof(float) * 3);
			vertexArray += 3;
			
			aiVector3D tangent = mesh->mTangents[face.mIndices[j]];
			memcpy(tangentArray, &tangent, sizeof(float) * 3);
			tangentArray += 3;

			if (mesh->HasTextureCoords(0)) {
				//printf_s("has tex coords");
			}
			if (mesh->HasTangentsAndBitangents()) {
				//printf_s("has tangents");
			}
		}
	}

	uvArray -= mesh->mNumFaces * 3 * 2;
	normalArray -= mesh->mNumFaces * 3 * 3;
	vertexArray -= mesh->mNumFaces * 3 * 3;

	tangentArray -= mesh->mNumFaces * 3 * 3;

	//Pisut's code
	g_point_count = mesh->mNumVertices;
	// allocate memory for vertex points
    if (mesh->HasPositions ()) {
        printf ("mesh has positions\n");
        g_vp = (GLfloat*)malloc (g_point_count * 3 * sizeof (GLfloat));
    }
    if (mesh->HasNormals ()) {
        printf ("mesh has normals\n");
        g_vn = (GLfloat*)malloc (g_point_count * 3 * sizeof (GLfloat));
    }
    if (mesh->HasTextureCoords (0)) {
        printf ("mesh has texture coords\n");
        g_vt = (GLfloat*)malloc (g_point_count * 2 * sizeof (GLfloat));
    }
    if (mesh->HasTangentsAndBitangents ()) {
        printf ("mesh has tangents\n");
        g_vtans = (GLfloat*)malloc (g_point_count * 4 * sizeof (GLfloat));
    }

	for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
        if (mesh->HasPositions ()) {
            const aiVector3D* vp = &(mesh->mVertices[v_i]);
            g_vp[v_i * 3] = (GLfloat)vp->x;
            g_vp[v_i * 3 + 1] = (GLfloat)vp->y;
            g_vp[v_i * 3 + 2] = (GLfloat)vp->z;
        }
        if (mesh->HasNormals ()) {
            const aiVector3D* vn = &(mesh->mNormals[v_i]);
            g_vn[v_i * 3] = (GLfloat)vn->x;
            g_vn[v_i * 3 + 1] = (GLfloat)vn->y;
            g_vn[v_i * 3 + 2] = (GLfloat)vn->z;
        }
        if (mesh->HasTextureCoords (0)) {
            const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
            g_vt[v_i * 2] = (GLfloat)vt->x;
            g_vt[v_i * 2 + 1] = (GLfloat)vt->y;
        }
        if (mesh->HasTangentsAndBitangents ()) {
            const aiVector3D* tangent = &(mesh->mTangents[v_i]);
            const aiVector3D* bitangent = &(mesh->mBitangents[v_i]);
            const aiVector3D* normal = &(mesh->mNormals[v_i]);
 
            // put the three vectors into my vec3 struct format for doing maths
            glm::vec3 t (tangent->x, tangent->y, tangent->z);
            glm::vec3 n (normal->x, normal->y, normal->z);
            glm::vec3 b (bitangent->x, bitangent->y, bitangent->z);
            // orthogonalise and normalise the tangent so we can use it in something
            // approximating a T,N,B inverse matrix
            glm::vec3 t_i = normalize (t - n * dot (n, t));
 
            // get determinant of T,B,N 3x3 matrix by dot*cross method
            float det = (dot (cross (n, t), b));
            if (det < 0.0f) {
                det = -1.0f;
            } else {
                det = 1.0f;
            }
 
            // push back 4d vector for inverse tangent with determinant
            g_vtans[v_i * 4] = t_i.x;
            g_vtans[v_i * 4 + 1] = t_i.y;
            g_vtans[v_i * 4 + 2] = t_i.z;
            g_vtans[v_i * 4 + 3] = det;
        }
    }

	//if (NULL != g_vtans) {

		//printf("HERE3");
	//}
	/*
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	GLuint points_vbo;
	if (NULL != g_vp) {
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
		glBufferData(
			GL_ARRAY_BUFFER, 3 * g_point_count * sizeof(GLfloat), g_vp, GL_STATIC_DRAW
		);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//glBindAttribLocation(p, 0, "point");
	}


	GLuint normals_vbo;
	if (NULL != g_vn) {
		glGenBuffers(1, &normals_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
		glBufferData(
			GL_ARRAY_BUFFER, 3 * g_point_count * sizeof(GLfloat), g_vn, GL_STATIC_DRAW
		);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
	}

	GLuint texcoords_vbo;
	if (NULL != g_vt) {
		glGenBuffers(1, &texcoords_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
		glBufferData(
			GL_ARRAY_BUFFER, 2 * g_point_count * sizeof(GLfloat), g_vt, GL_STATIC_DRAW
		);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);
	}

	GLuint tangents_vbo;
	printf("HERE2");
	if (NULL != g_vtans) {
		printf("HERE1");

		glGenBuffers(1, &tangents_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, tangents_vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			4 * g_point_count * sizeof(GLfloat),
			g_vtans,
			GL_STATIC_DRAW
		);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindAttribLocation(p, 3, "tangent");

		const char* attribute_name = "tangent";
		GLint attribute_v_color = glGetAttribLocation(p, attribute_name);
		if (attribute_v_color == -1) {
			printf("Could not bind attribute ", attribute_name);
		}
		else {
			printf("location ", attribute_v_color);
		}

	}
	*/

	return true;
}


void createVBOs() 
{

	GLuint vao;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
 

    GLuint points_vbo;
    if (NULL != g_vp) {
        glGenBuffers (1, &points_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
        glBufferData (
            GL_ARRAY_BUFFER, 3 * g_point_count * sizeof (GLfloat), g_vp, GL_STATIC_DRAW
        );
		glEnableVertexAttribArray(0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        
		//glBindAttribLocation(p, 0, "point");
    }

   
    GLuint normals_vbo;
    if (NULL != g_vn) {
        glGenBuffers (1, &normals_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
        glBufferData (
            GL_ARRAY_BUFFER, 3 * g_point_count * sizeof (GLfloat), g_vn, GL_STATIC_DRAW
        );
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (1);
    }
   
    GLuint texcoords_vbo;
    if (NULL != g_vt) {
        glGenBuffers (1, &texcoords_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, texcoords_vbo);
        glBufferData (
            GL_ARRAY_BUFFER, 2 * g_point_count * sizeof (GLfloat), g_vt, GL_STATIC_DRAW
        );
        glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (2);
    }
   
    GLuint tangents_vbo;
	printf("HERE2");
    if (NULL != g_vtans) {
		printf("HERE1");

        glGenBuffers (1, &tangents_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, tangents_vbo);
        glBufferData (
            GL_ARRAY_BUFFER,
            4 * g_point_count * sizeof (GLfloat),
            g_vtans,
            GL_STATIC_DRAW
        );
		glEnableVertexAttribArray(3);
        glVertexAttribPointer (3, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindAttribLocation(p, 3, "Tangent");

		const char* attribute_name = "Tangent";
		GLint attribute_v_color = glGetAttribLocation(p, attribute_name);
		if (attribute_v_color == -1) {
			printf("Could not bind attribute ", attribute_name);
		}
		else {
			printf("location ", attribute_v_color);
		}
        
    }
	// glGenVertexArrays(1, &vao);
	//glBindVertexArray (vao);
	
}


//"Models/UmbreonHighPoly.blend"
bool loadModelOld(const char* path)
{
    

	DWORD dwFlags =
        aiProcess_Triangulate                | // triangulate n-polygons
        aiProcess_ValidateDataStructure        |
        aiProcess_ImproveCacheLocality         |
        aiProcess_RemoveRedundantMaterials    |
        aiProcess_SortByPType                |
        aiProcess_FindDegenerates            |
        aiProcess_FindInvalidData            |
        aiProcess_ConvertToLeftHanded        |
        aiProcess_GenSmoothNormals            |
        aiProcess_CalcTangentSpace            |
        aiProcess_GenUVCoords                |
        aiProcess_FixInfacingNormals        |
        aiProcess_TransformUVCoords;


    const aiScene* scene = importer.ReadFile(path, dwFlags);

	if(!scene)
	{
		printf( "3dModel loading error: '%s'\n", importer.GetErrorString() );
		return false;
	}

	// Now we can access the file's contents.
   // printf("Import of 3d scene %s succeeded.");

	
	aiMesh *mesh = scene->mMeshes[0];

	numVerts = mesh->mNumFaces*3;
 
	vertexArray = new float[mesh->mNumFaces*3*3];
	normalArray = new float[mesh->mNumFaces*3*3];
	uvArray = new float[mesh->mNumFaces*3*2];

	tangentArray = new float[mesh->mNumFaces * 3 * 3];
 
	for(unsigned int i=0;i<mesh->mNumFaces;i++)
	{
		const aiFace& face = mesh->mFaces[i];
 
		for(int j=0;j<3;j++)
		{
			aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
			memcpy(uvArray,&uv,sizeof(float)*2);
			uvArray+=2;
 
			aiVector3D normal = mesh->mNormals[face.mIndices[j]];
			memcpy(normalArray,&normal,sizeof(float)*3);
			normalArray+=3;
 
			aiVector3D pos = mesh->mVertices[face.mIndices[j]];
			memcpy(vertexArray,&pos,sizeof(float)*3);
			vertexArray+=3;

			
			aiVector3D tangent = mesh->mTangents[face.mIndices[j]];
			memcpy(tangentArray, &tangent, sizeof(float) * 3);
			tangentArray += 3;
		}
	}
 
	uvArray-=mesh->mNumFaces*3*2;
	normalArray-=mesh->mNumFaces*3*3;
	vertexArray-=mesh->mNumFaces*3*3;
	
	tangentArray-=mesh->mNumFaces*3*3;


	/*
	// Now add bump mapping data (tangent and bitanget vectors for every vertex)
	vboBumpMapData.BindVBO();
	vboBumpMapData.UploadDataToGPU(GL_STATIC_DRAW);

	
	// Tangent vector
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2*sizeof(aiVector3D), 0);
	// Bitangent vector
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2*sizeof(aiVector3D), (void*)(sizeof(aiVector3D))); 
	*/

	return true;
} 


/***************************************************************
 ** init function ***
 
 set up some default OpenGL values here

 ***************************************************************/

void init()
{
	glClearColor(0.5, 0.5, 0.5, 0.5); //background color
	glClearDepth(1.0);	//background depth value

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 1, 1000);  //setup a perspective projection

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(						//set up camera
				0.0, 0.0, 5.0,		// eye position
				0.0, 0.0, 0.0,		// lookat position
				0.0, 1.0, 0.0		// up direction
				);


	glEnable(GL_DEPTH_TEST); //enable z-buffer hidden surface removal

	glEnable(GL_LIGHTING); //enable lighting
	glEnable(GL_LIGHT0); //enable
	
	float lpos[] = { 5, 5, 5, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	//float lpos2[] = { -5, -5, -5, 0 };
	//glLightfv(GL_LIGHT0, GL_POSITION, lpos2);
	

	//glShadeModel(GL_FLAT); //flat shading
	glShadeModel(GL_SMOOTH); //smooth shading (default): enables Gouraud shading

	
	//loadModel("Models/UmbreonHighPoly.obj");
	
}






void LoadGLTextures(const char *filename)                             
{
	
	GLint DiffuseTextureID = glGetUniformLocation(p, "diffuseTexture");
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(DiffuseTextureID, 0);
	tex_diffuse;
	glGenTextures(1, &tex_diffuse);

	tex_diffuse = SOIL_load_OGL_texture
    (
        filename, 
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );

	if( 0 == tex_diffuse)
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
}



void loadWithNormalMap(const char *diffuse, const char *normal)                             
{


	GLint DiffuseTextureID = glGetUniformLocation(p, "diffuseTexture");
	GLint NormalTextureID = glGetUniformLocation(p, "normalTexture");

	glEnable(GL_TEXTURE_2D);  

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(DiffuseTextureID, 0);
    tex_diffuse;
	glGenTextures(1, &tex_diffuse);

	tex_diffuse = SOIL_load_OGL_texture
    (
        diffuse, 
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
	if( 0 == tex_diffuse )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);


	glActiveTexture(GL_TEXTURE1);
	glUniform1i(NormalTextureID, 1);
	tex_normal;
	glGenTextures(1, &tex_normal);

	tex_normal = SOIL_load_OGL_texture
	(
		normal,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if (0 == tex_normal)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	glBindTexture(GL_TEXTURE_2D, tex_normal);
	


	//glActiveTextureARB(GL_TEXTURE0);
	
	//glClientActiveTexture(GL_TEXTURE0);
	//glEnable(GL_TEXTURE_2D);
	
	//glUniform1i(glGetUniformLocation(p, "myTexture"), 0);
	
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glDisable(GL_TEXTURE_2D);


	
	//glClientActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0 + 1);
	
	//glUniform1i(glGetUniformLocation(p, "bump"), 1);
	//glUniform1i(NormalTextureID,1);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glDisable(GL_TEXTURE_2D);

	/*glUseProgram(p);
	// Bind Textures using texture units
	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(p, "myTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);
	
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(p, "bump"), 1);
	glBindTexture(GL_TEXTURE_2D, tex_normal);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0, 0.0);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0, 0.0);
	*/

}



void loadCubeMap() 
{

	// load 6 images into a new OpenGL cube map, forcing RGB

		
	tex_cube = SOIL_load_OGL_cubemap
	(
		"Textures/criminal-impact_rt.tga",
		"Textures/criminal-impact_lf.tga",
		"Textures/criminal-impact_up.tga",
		"Textures/criminal-impact_dn2.tga",
		"Textures/criminal-impact_bk.tga",		
		"Textures/criminal-impact_ft.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);

	if( 0 == tex_cube )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}
	 glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cube);
	 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
}


// load skybox textures
void loadSkyboxTextures() {
    skybox[1] = SOIL_load_OGL_texture                                           // skybox front
        (
        "Textures/criminal-impact_ft.tga",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
    //glBindTexture(GL_TEXTURE_2D, skybox[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
    skybox[0] = SOIL_load_OGL_texture                                           // skybox right
        (
        "Textures/criminal-impact_rt.tga",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
   //glBindTexture(GL_TEXTURE_2D, skybox[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
    skybox[2] = SOIL_load_OGL_texture                                           // skybox left
        (
       "Textures/criminal-impact_lf.tga",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
    //glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
    skybox[3] = SOIL_load_OGL_texture                                           // skybox back
        (
        "Textures/criminal-impact_bk.tga",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
    //glBindTexture(GL_TEXTURE_2D, skybox[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	skybox[4] = SOIL_load_OGL_texture                                           // skybox back
        (
        "Textures/criminal-impact_up.tga",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
    //glBindTexture(GL_TEXTURE_2D, skybox[4]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	skybox[5] = SOIL_load_OGL_texture                                           // skybox back
        (
        "Textures/criminal-impact_dn.tga",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
    //glBindTexture(GL_TEXTURE_2D, skybox[5]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);        // clamp edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 2);
} 


// display skybox
void display_skybox() {

	char *skyvs = NULL,*skyfs = NULL;
	skyv = glCreateShader(GL_VERTEX_SHADER);
	skyf = glCreateShader(GL_FRAGMENT_SHADER);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	skyvs = textFileRead("Shaders/texture - Copy.vert");
	skyfs = textFileRead("Shaders/texture - Copy.frag");
	const char * skyff = skyfs;
	const char * skyvv = skyvs;

	glShaderSource(skyv, 1, &skyvv, NULL);
	glShaderSource(skyf, 1, &skyff, NULL);
	
	free(skyvs);free(skyfs);

	glCompileShader(skyv);
	glCompileShader(skyf);
	
	skyp = glCreateProgram();
	glAttachShader(skyp,skyf);
	glAttachShader(skyp,skyv);

	glLinkProgram(skyp);

	glUseProgram(skyp);

	// Enable/Disable features
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST); // skybox should be drawn behind anything else
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
 
    // Just in case we set all vertices to white.
    glColor4f(1,1,1,1);

    glDisable(GL_DEPTH_TEST);

	int width = 8;
    int height = 8;
    int length = 8;

    //start in this coordinates
    int x = -3;
    int y = -3;
    int z = -3;

   // Render the front quad
	//glActiveTexture(GL_TEXTURE0 );
    glBindTexture(GL_TEXTURE_2D, skybox[0]);
    glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z + length);
    glEnd();
 
    // Render the left quad
	//glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, skybox[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,  z + length);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z); 
    glEnd();
	//skyLoc = glGetUniformLocation(skyp, "Skybox1");
	//glUniform1i(skyLoc, 1);
 
    // Render the back quad
	//glActiveTexture(GL_TEXTURE0 + 2 );
    glBindTexture(GL_TEXTURE_2D, skybox[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,  z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
     glEnd();
	// skyLoc = glGetUniformLocation(skyp, "Skybox2");
	// glUniform1i(skyLoc, 2);
 
    // Render the right quad
	//glActiveTexture(GL_TEXTURE0 + 3 );
    glBindTexture(GL_TEXTURE_2D, skybox[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,  z + length);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glEnd();
	//skyLoc = glGetUniformLocation(skyp, "Skybox3");
	//glUniform1i(skyLoc, 3);
 
    // Render the top quad
	//glActiveTexture(GL_TEXTURE0 + 4 );
    glBindTexture(GL_TEXTURE_2D, skybox[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glEnd();
	//skyLoc = glGetUniformLocation(skyp, "Skybox4");
	//glUniform1i(skyLoc, 4);
 
    // Render the bottom quad
	//glActiveTexture(GL_TEXTURE0 + 5 );
    glBindTexture(GL_TEXTURE_2D, skybox[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y,  z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y,  z + length);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,  z + length); 
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,  z);
    glEnd();
	//skyLoc = glGetUniformLocation(skyp, "Skybox5");
	//glUniform1i(skyLoc, 5);



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
}

//Create shader error log
void shaderError(GLuint shader)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);


	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(shader); // Don't leak the shader.
		return;
	}
}

/***************************************************************
 ** display callback function **
 - get's called each time application needs to redraw itself
 - most opengl work is done through this function
 ***************************************************************/

void moveLight() {

	//printf(" Light! ");

	glPushMatrix();

	glTranslatef(5, 5, 5);
	glRotated(lightAngle, 0.0f, 1.0f, 0.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	//glTranslatef(-5,-5,-5);

	glPopMatrix();
}


void display()
{
	
	glClear(
		GL_COLOR_BUFFER_BIT  //clear the frame buffer (set it to background color)
		| GL_DEPTH_BUFFER_BIT //clear the depth buffer for z-buffer hidden surface removal 
		);
		

	//////////////////******* insert your openGL drawing code here ****** ///////////
	//display_skybox();

	glPushMatrix(); // save current modelview matrix (mostly saves camera transform)
	glScalef(scaleFactor, scaleFactor, scaleFactor);
	glTranslated(xPos,yPos,zPos);
	glRotated(rotAngle, 0, 1, 0); //rotate by rotAngle about y-axis
	glRotated(rotAngleVert, 1, 0, 0);
		
	glEnable(GL_COLOR_MATERIAL);	//instead of specifying material properties
	glColor3f(red, green, blue);			//  we will use glColor to set the diffuse color
	//LoadGLTextures("Textures/bricks.bmp");
	   
	//draw model
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertexArray);
	glNormalPointer(GL_FLOAT, 0, normalArray);

	glClientActiveTexture(GL_TEXTURE0_ARB);
	glTexCoordPointer(2, GL_FLOAT, 0, uvArray);

	//glVertexAttribPointer(tangentArray, 3, GL_FLOAT, 0 );	
	//glMultiTexCoord3fv(GL_TEXTURE1,&tangentArray);
	//glEnableVertexAttribArrayARB(6); // use 7 for binormals
	//glVertexAttribPointerARB(6, 3, GL_FLOAT, GL_FALSE, 0, tangentArray);
	
	GLint locTangent	=	glGetAttribLocation(p, "Tangent");
	glEnableVertexAttribArray(locTangent);
	glVertexAttribPointerARB(locTangent, 3, GL_FLOAT, GL_FALSE, 0, tangentArray);
	glBindAttribLocationARB(p, locTangent, "Tangent");
	
	GLint locNormal = glGetAttribLocation(p, "Normal");
	glEnableVertexAttribArray(locNormal);
	glVertexAttribPointerARB(locNormal, 3, GL_FLOAT, GL_FALSE, 0, normalArray);
	glBindAttribLocationARB(p, locNormal, "Normal");

	GLint locTexcoords = glGetAttribLocation(p, "Texcoords");
	glEnableVertexAttribArray(locTexcoords);
	glVertexAttribPointerARB(locTexcoords, 2, GL_FLOAT, GL_FALSE, 0, uvArray);
	glBindAttribLocationARB(p, locTexcoords, "Texcoords");

	glLinkProgramARB(p);

	// Bind Textures using texture units

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(p, "diffuseTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(p, "normalTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, tex_normal);


	//glDrawArrays(GL_QUADS, 0, numVerts);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	
	//glutSolidTeapot(1);
	//GLUquadric *quad;
	//quad = gluNewQuadric();
	//gluSphere(quad, 1, 100, 20);

	////////////////////////////////////////////////////////////////

	glPopMatrix(); // retrieve modelview matrix
	glFlush(); //force OpenGL to render now;


	glutSwapBuffers(); // when using double buffering, need to swap front and back buffer

}



/*************************************************************
 ** keyboard callback function **

 *************************************************************/

void keyboard(unsigned char k, int x, int y)
{


	switch (k)
	{
	case 'a':
		rotAngle+=5; //changes a global variable ... this is later picked up by display
		break;
	case 'd':
		rotAngle-=5;  
		break;
	case 'w':
		rotAngleVert+=5; 
		break;
	case 's':
		rotAngleVert-=5;  
		break;
	case 'r':
		if (red < 1) {
			red+=.1;
		}
		break;
	case 't':
		if (red > 0) {
			red-=.1; 
		}
		break;
	case 'g':
		if (green < 1) {
			green+=.1;
		}
		break;
	case 'h':
		if (green > 0) {
			green-=.1;
		}
		break;
	case 'b':
		if (blue < 1) {
			blue+=.1;  
		}
		break;
	case 'n':
		if (blue > 0) {
			blue-=.1; 
		}
		break;

	case 'q':
		moveLight();
		lightAngle += 10;
		break;

	}



	glutPostRedisplay(); //tell glut that the display callback should be called after this
}




/*************************************************************
 ** main ***
 where everything begins

*************************************************************/

void main()
{
	//glEnable(GL_TEXTURE_2D);
	

	glutInitDisplayMode(	// initialize GLUT
			GLUT_DOUBLE		// request memory for 2 frame buffers for animation
			| GLUT_DEPTH	// request memory for z-buffer
			| GLUT_RGB		// set RGB color mode
			); 

	glutInitWindowSize(500, 500);
	glutCreateWindow("GLUT Example 02");	// create a window
		
	loadSkyboxTextures();

	glutDisplayFunc(display);				// set the display callback

	glutKeyboardFunc(keyboard);
	//glutIdleFunc(moveLight);

	init();
	glewInit();

	char *vs = NULL, *fs = NULL;
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vs = textFileRead("Shaders/litSphereNew.vert");
	fs = textFileRead("Shaders/litSphereNew.frag"); //magicPhong
	//vs = textFileRead("Shaders/normalMapFinal.vert");
	//fs = textFileRead("Shaders/normalMapFinal.frag");
	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs); free(fs);

	glCompileShader(v);
	shaderError(v);

	glCompileShader(f);
	shaderError(f);

	p = glCreateProgram();
	glAttachShader(p, f);
	glAttachShader(p, v);

	
	
	loadModel("Models/Rabbit.obj");
	//loadModel("Models/sphere.obj");
	//loadModel("Models/UmbreonHighPoly.obj");
	
	//createVBOs();
	scaleFactor = 4;
	rotAngle = -120;
	//xPos = -0.2;
	//yPos = 0.0;
	//zPos = 0.0;
	glLinkProgram(p);
	glUseProgram(p);
	
	loadWithNormalMap("Textures/green.PNG", "Textures/attrib1.png"); //("Textures/lava.jpg", "Textures/lava_normal.jpg");
	//loadCubeMap();
	//LoadGLTextures("Textures/green2.png");
	


	glutMainLoop();	// now let GLUT take care of everything


}