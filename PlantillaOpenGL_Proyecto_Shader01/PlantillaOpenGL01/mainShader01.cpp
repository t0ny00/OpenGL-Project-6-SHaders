// Cubica

#include <stdlib.h>
#include <conio.h>

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "glsl.h"
#include "glm.h"

// assimp include files. These three are usually needed.
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// the global Assimp scene object
const aiScene* scene = NULL;
GLuint scene_list = 0;
aiVector3D scene_min, scene_max, scene_center;

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


using namespace std;

cwc::glShaderManager SM;
cwc::glShader *shader01;
cwc::glShader *shader02;

GLfloat calctype; 
GLfloat r; 
GLfloat freq; 
GLfloat hoff; 
GLfloat f; 

GLfloat xc; 
GLfloat yc; 
GLfloat huefreq; 
GLfloat sz; 
GLfloat escape;
GLfloat maxiter;

void ejesCoordenada() {
	
	glDisable(GL_LIGHTING);	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glEnable(GL_LIGHTING);

	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

	
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 300.0);
   glMatrixMode (GL_MODELVIEW);

}

void printValues(){
	system("cls");
	printf("Spirofield \n");
	printf("=============================== \n");
	printf("_r:  %.2f \n",r);
	printf("_hoff:  %.2f \n",hoff);
	printf("_freq:  %.2f \n",freq);
	printf("_calctype:  %.2f \n",calctype);
	printf("_f:  %.2f \n",f);

	printf("\n Mandel \n");
	printf("=============================== \n");
	printf("_xc:  %.2f \n",xc);
	printf("_yc:  %.2f \n",yc);
	printf("_huefreq:  %.2f \n",huefreq);
	printf("_sz:  %.3f \n",sz);
	printf("_escape:  %.2f \n",escape);
	printf("_maxiter:  %.2f \n",maxiter);
};

void init(){
	
   
   shader01 = SM.loadfromFile("mandel.vert","mandel.frag"); // load (and compile, link) from file
  		  if (shader01==0) 
			  std::cout << "Error Loading, compiling or linking shader\n";

   shader02 = SM.loadfromFile("spirofield.vert","spirofield.frag"); // load (and compile, link) from file
  		  if (shader02==0) 
			  std::cout << "Error Loading, compiling or linking shader\n";



	r = 10;
	hoff = 0;
	freq = 1;
	calctype = 0;
	f = 1;

	xc = 0.5; 
	yc = 0.5; 
	huefreq = 1; 
	sz = 4; 
	escape = 256;
	maxiter = 20;

	printValues();
}

void cargar_shader(int idx) {


	// Plano Derecho Mandel
	if (idx == 0){	
			if (shader01) shader01->begin();
			shader01->setUniform1f("_xc",xc);
			shader01->setUniform1f("_yc",yc);
			shader01->setUniform1f("_huefreq",huefreq);
			shader01->setUniform1f("_sz",sz);
			shader01->setUniform1f("_escape",escape);
			shader01->setUniform1f("_maxiter",maxiter);
			//Colocar aqui los parametros Uniform

	}

	// Plano Izquierdo SpiroField
	if (idx == 1){		
		   if (shader02) shader02->begin();

		   shader02->setUniform1f("_r",r);
		   shader02->setUniform1f("_hoff",hoff);
		   shader02->setUniform1f("_freq",freq);
		   shader02->setUniform1f("_calctype",calctype);
		   shader02->setUniform1f("_f",f);
		   //Colocar aqui los parametros Uniform
	}


}



void fin_shader(int idx) {


	// Material Piso
	if (idx == 0){	
			if (shader01) shader01->end();
	}

	// Material Columna
	if (idx == 1){		
		   if (shader02) shader02->end();
	}


}

void Keyboard(unsigned char key, int x, int y)
{


  switch (key)
  {
	case 'q':
		calctype = 0.0;
		break;
	case 'a':
		calctype = 1.0;
		break;
	case 'z':
		calctype = 2.0;
		break;
	case 'w':
		r += 1;
		break;
	case 'e':
		if (r > 1.1) r -= 1;
		break;
	case 's':
		freq += 0.05;
		break;
	case 'd':
		if (freq > 0.05) freq -= 0.05;
		break;
	case 'x':
		hoff += 0.1;
		break;
	case 'c':
		if (hoff > 0.009) hoff -= 0.1;
		break;
	case 'r':
		f += 0.05;
		break;
	case 't':
		if (f > 0.04) f -= 0.05;
		break;
	case 'y':
		xc += 0.05;
		yc += 0.05;
		break;
	case 'u':
		if (xc > 0.04) xc -= 0.05;
		if (yc > 0.04) yc -= 0.05;
		break;
	case 'n':
		sz += 0.001;
		break;
	case 'm':
		if (sz >= 0.001) sz -= 0.001;
		break;
	case 'i':
		huefreq += 0.05;
		break;
	case 'o':
		if (huefreq >= 0.05) huefreq -= 0.05;
		break;
	case 'f':
		escape += 12;
		break;
	case 'g':
		if (escape >= 12) escape -= 12;
		break;
	case 'v':
		maxiter += 12;
		break;
	case 'b':
		if (maxiter >= 12) maxiter -= 12;
		break;
	case '1':
		r = 6.5;
		hoff = 1.9;
		freq = 0.75;
		calctype = 1;
		f = 1;

		xc = 0.5; 
		yc = 0.5; 
		huefreq = 1; 
		sz = 4; 
		escape = 256;
		maxiter = 20;
		break;
	case '2':
		r = 13;
		hoff = 1.05;
		freq = 0.8;
		calctype = 1;
		f = 0.5;

		xc = 0.5; 
		yc = 0.5; 
		huefreq = 1; 
		sz = 4; 
		escape = 256;
		maxiter = 20;
		break;
	case '3':
		r = 2;
		hoff = 0;
		freq = 0.25;
		calctype = 0;
		f = 1;

		xc = 0.5; 
		yc = 0.5; 
		huefreq = 1; 
		sz = 4; 
		escape = 256;
		maxiter = 20;
		break;
	case '4':
		r = 11;
		hoff = 0.8;
		freq = 0.55;
		calctype = 2;
		f = 1;

		xc = 0.5; 
		yc = 0.5; 
		huefreq = 1; 
		sz = 4; 
		escape = 256;
		maxiter = 20;
		break;
	case '5':
		r = 11;
		hoff = 0.8;
		freq = 0.55;
		calctype = 2;
		f = 1;

		xc = 0.39; 
		yc = 0.25; 
		huefreq = 0.36; 
		sz = 0.05; 
		escape = 512;
		maxiter = 120;
		break;
	case '6':
		r = 11;
		hoff = 0.8;
		freq = 0.55;
		calctype = 2;
		f = 1;

		xc = 0.39; 
		yc = 0.27; 
		huefreq = 2.4; 
		sz = 0.01; 
		escape = 512;
		maxiter = 256;
		break;
	default:
		break;
  }
  scene_list = 0;
  printValues();
  glutPostRedisplay();
}

void recursive_render (const aiScene *sc, const aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);
	
	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;
			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				
				if (mesh->HasTextureCoords(0)) 
					glTexCoord2f(mesh->mTextureCoords[0][index].x, 1-mesh->mTextureCoords[0][index].y);
				
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {		
		cargar_shader(n);
		recursive_render(sc, nd->mChildren[n]);
		fin_shader(n);
	}

	glPopMatrix();
}

void render(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (0.0, 0.0, 70.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

			
	glPushMatrix();



	// Codigo para el mesh	
	glEnable(GL_NORMALIZE);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 0.0, 0.0);
	glScalef(1.0, 1.0, 1.0);
	if(scene_list == 0) {
	    scene_list = glGenLists(1);
	    glNewList(scene_list, GL_COMPILE);
            // now begin at the root node of the imported data and traverse
            // the scenegraph by multiplying subsequent local transforms
            // together on GL's matrix stack.
	    recursive_render(scene, scene->mRootNode);
	    glEndList();
	}
	glCallList(scene_list);
	
	glPopMatrix();
	
	
	

	
	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);
	glutSwapBuffers();
}

void animacion(int value) {
	
	glutTimerFunc(10,animacion,1);
    glutPostRedisplay();
	
}

void get_bounding_box_for_node (const aiNode* nd, 	aiVector3D* min, 	aiVector3D* max, 	aiMatrix4x4* trafo){
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

void get_bounding_box (aiVector3D* min, aiVector3D* max){
	aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);
	
	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene->mRootNode,min,max,&trafo);
}

int loadasset (const char* path){
	// we are taking one of the postprocessing presets to avoid
	// spelling out 20+ single postprocessing flags here.
	scene = aiImportFile(path,aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene) {
		get_bounding_box(&scene_min,&scene_max);
		scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
		scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
		scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
		return 0;
	}
	return 1;
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutCreateWindow("Test Opengl");


	// Codigo para cargar la geometria usando ASSIMP

	aiLogStream stream;
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);

	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"assimp_log.txt");
	aiAttachLogStream(&stream);

	// the model name can be specified on the command line. If none
	// is specified, we try to locate one of the more expressive test 
	// models from the repository (/models-nonbsd may be missing in 
	// some distributions so we need a fallback from /models!).
	
	
	loadasset( "planos.obj");



	init ();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc (Keyboard);
	
	glutMainLoop();
	return 0;

}
