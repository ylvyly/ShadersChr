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
//#include <VertexTriangleAdjacency.h>



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
GLuint background;

GLuint depthFBO;
GLuint depthRenderBuffer;
GLuint tex_depth;

//swiftless.com
GLuint fbo; // The frame buffer object  
GLuint fbo_depth; // The depth buffer for the frame buffer object  
GLuint fbo_texture; // The texture object to write our frame buffer object to  
int window_width = 500; // The width of our window  
int window_height = 500; // The height of our window  
float rotation_degree = 0.0f; // The angle of rotation in degrees for our teapot  

//init depth buffer
void initFrameBufferDepthBuffer() {
	glGenRenderbuffersEXT(1, &fbo_depth); // Generate one render buffer and store the ID in fbo_depth 
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, fbo_depth); // Bind the fbo_depth render buffer  
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, window_width, window_height); // Set the render buffer storage to be a depth component, with a width and height of the window  
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Set the render buffer of this buffer to the depth buffer  
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); // Unbind the render buffer  
}
//init texture
void initFrameBufferTexture(void) {
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE3);
	GLint DepthTextureID = glGetUniformLocation(p, "depthBuffer");
	glUniform1i(DepthTextureID, 3);
	glGenTextures(1, &fbo_texture); // Generate one texture  
	glBindTexture(GL_TEXTURE_2D, fbo_texture); // Bind the texture fbo_texture  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window_width, window_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL); // Create a standard texture with the width and height of our window  

	// Setup the basic texture parameters  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Unbind the texture  
	glBindTexture(GL_TEXTURE_2D, 0);
}
//call init funcs
void initFrameBuffer(void) {
	initFrameBufferDepthBuffer(); // Initialize our frame buffer depth buffer  
	initFrameBufferTexture(); // Initialize our frame buffer texture  
	
	glGenFramebuffersEXT(1, &fbo); // Generate one frame buffer and store the ID in fbo  
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer  
	
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, fbo_texture, 0); // Attach the texture fbo_texture to the color buffer in our frame buffer   
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, fbo_depth); // Attach the depth buffer fbo_depth to our frame buffer

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); // Check that status of our generated frame buffer  
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT) // If the frame buffer does not report back as complete  
	{
		printf( "Couldn't create frame buffer"); // Make sure you include <iostream>  
	}
	switch (status)
	{
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			std::cout << "OpenGL framebuffer format not supported. " << std::endl;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			std::cout << "OpenGL framebuffer missing attachment." << std::endl;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "OpenGL framebuffer incomplete attachment." << std::endl;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			std::cout << "OpenGL framebuffer attached images must have same dimensions." << std::endl;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			std::cout << "OpenGL framebuffer attached images must have same format." << std::endl;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			std::cout << "OpenGL framebuffer missing draw buffer." << std::endl;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			std::cout << "OpenGL framebuffer missing read buffer." << std::endl;
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			std::cout << "Framebuffer complete" << std::endl;
		default:
			std::cout << "Status: ";
			std::cout << status << std::endl;
	}
	//std::cout << status << std::endl; // Output an error to the console  
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our frame buffer  

}


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

			/*
			aiFace *faceT = //mesh->mFaces[i];
			Assimp::VertexTriangleAdjacency vtAdjacency(faceT, i, i, true);
			vtAdjacency.GetAdjacentTriangles(i);
			
			unsigned int* test = vtAdjacency.GetAdjacentTriangles(i);
			*/

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
	//swiftless.com framebuffer init
	glEnable(GL_TEXTURE_2D); // Enable texturing so we can bind our frame buffer texture  
	glEnable(GL_DEPTH_TEST); // Enable depth testing  
	initFrameBuffer(); // Create our frame buffer object  


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



void loadWithNormalMap(const char *diffuse, const char *normal, const char *depth)
{


	GLint DiffuseTextureID = glGetUniformLocation(p, "diffuseTexture");
	GLint NormalTextureID = glGetUniformLocation(p, "normalTexture");

	GLint DepthTextureID = glGetUniformLocation(p, "depthTexture");

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


	glActiveTexture(GL_TEXTURE2);
	glUniform1i(DepthTextureID, 2);
	tex_depth;
	glGenTextures(1, &tex_depth);

	tex_depth = SOIL_load_OGL_texture
	(
		depth,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if (0 == tex_depth)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	glBindTexture(GL_TEXTURE_2D, tex_depth);
	
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

	/*
	char *skyvs = NULL,*skyfs = NULL;
	skyv = glCreateShader(GL_VERTEX_SHADER);
	skyf = glCreateShader(GL_FRAGMENT_SHADER);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	skyvs = textFileRead("Shaders/texture.vert");
	skyfs = textFileRead("Shaders/texture.frag");
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
	*/
	
	/*
	GLint BackgroundTextureID = glGetUniformLocation(p, "depthBuffer");
	glActiveTexture(GL_TEXTURE3);
	glUniform1i(BackgroundTextureID, 3);
	background;
	glGenTextures(1, &background);

	background = SOIL_load_OGL_texture
	(
		"Textures/grass.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if (0 == background)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	glBindTexture(GL_TEXTURE_2D, background);
	*/
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

	/*
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
 
 // Enable/Disable features
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST); // skybox should be drawn behind anything else
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	int width = 8;
	int height = 8;
	int length = 8;

	//start in this coordinates
	int x = -3;
	int y = -3;
	int z = -3;

    // Render the back quad
	
	//glActiveTexture(GL_TEXTURE0 + 2 );

	*/

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(p, "normalTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, tex_normal);
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,  z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
     glEnd();
	// skyLoc = glGetUniformLocation(skyp, "Skybox2");
	// glUniform1i(skyLoc, 2);
 
	 /*
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
	*/
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



void renderTeapotScene(void) {

	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(p, "depthBuffer"), 3);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // Bind our frame buffer for rendering  
	glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT); // Push our glEnable and glViewport states  
	glViewport(0, 0, window_width, window_height); // Set the size of the frame buffer view port  

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Set the clear colour  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the depth and colour buffers  

	glLoadIdentity();  // Reset the modelview matrix  

	glTranslatef(0.0f, 0.0f, -5.0f); // Translate back 5 units  

	glRotatef(rotation_degree, 1.0f, 1.0f, 0.0f); // Rotate according to our rotation_degree value  

	glutSolidTeapot(1.0f); // Render a teapot

	glPopAttrib(); // Restore our glEnable and glViewport states  
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // Unbind our texture  

	rotation_degree += 0.5f;
	if (rotation_degree > 360.0f)
		rotation_degree = 0.0f;
}

void renderSkybox() {
	//glLoadIdentity(); 
	//glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST); // skybox should be drawn behind anything else
	//glDisable(GL_LIGHTING);
	
	//glActiveTexture(GL_TEXTURE2);
	//glUniform1i(glGetUniformLocation(p, "depthTexture"), 2);
	//glBindTexture(GL_TEXTURE_2D, tex_depth);

	// Just in case we set all vertices to white.
	//glColor4f(1, 1, 1, 1);
	int width = 8;
	int height = 8;
	int length = 8;
	//start in this coordinates
	int x = -3;
	int y = -3;
	int z = -3;
	
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
}


void renderModel() {

	glPushMatrix(); // save current modelview matrix (mostly saves camera transform)
	glScalef(scaleFactor, scaleFactor, scaleFactor);  //rescale model
	glTranslated(xPos, yPos, zPos);	//reposition model
	glRotated(rotAngle, 0, 1, 0); //rotate by about y-axis
	glRotated(rotAngleVert, 1, 0, 0); //rotate by about x-axis

	glEnable(GL_COLOR_MATERIAL);	//instead of specifying material properties
	glColor3f(red, green, blue);	

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertexArray);
	glNormalPointer(GL_FLOAT, 0, normalArray);

	glClientActiveTexture(GL_TEXTURE0_ARB);
	glTexCoordPointer(2, GL_FLOAT, 0, uvArray);

	GLint locTangent = glGetAttribLocation(p, "Tangent");
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
	//loadFrameBuffer(500, 500);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(p, "diffuseTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(p, "normalTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, tex_normal);

	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(p, "depthTexture"), 2);
	glBindTexture(GL_TEXTURE_2D, tex_depth);

	/*
	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(p, "depthBuffer"), 3);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	*/

	//glDrawArrays(GL_QUADS, 0, numVerts);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//draw teapot
		//glutSolidTeapot(1);
	
	//draw sphere
		//GLUquadric *quad;
		//quad = gluNewQuadric();
		//gluSphere(quad, 1, 100, 20);

}


void display()
{
	
	///////////////////////////////////////////////////////////////
	//  framebuffer 
	/*
	renderTeapotScene();
	
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the colour buffer (more buffers later on)  
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations  

	glTranslatef(0.0f, 0.0f, -2.0f);

	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(p, "depthBuffer"), 3);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);

	//glutSolidTeapot(1.0f); // Render a teapot
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner  

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner  

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner  

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner  
	
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glutSwapBuffers();

	*/
	///////////////////////////////////////////////////////////////
	

	glClear(
		GL_COLOR_BUFFER_BIT  //clear the frame buffer (set it to background color)
		| GL_DEPTH_BUFFER_BIT //clear the depth buffer for z-buffer hidden surface removal 
		);
		

	//////////////////******* insert your openGL drawing code here ****** ///////////
	//display_skybox();
	//renderSkybox();
	renderModel();
	
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


void loadSkyboxTex(const char *backgroundTex) {


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
		
	//loadSkyboxTextures();
	glewInit();
	init();

	glutDisplayFunc(display);				// set the display callback
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(moveLight);


	
	
	//loadFrameBuffer();

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
	
	loadWithNormalMap("Textures/sphereTest.jpg", "Textures/grass.png", "Textures/grass_normal.png"); //("Textures/lava.jpg", "Textures/lava_normal.jpg");
	//loadSkyboxTex("Textures/lava.jpg");
	//loadCubeMap();
	//LoadGLTextures("Textures/green2.png");
	


	glutMainLoop();	// now let GLUT take care of everything


}