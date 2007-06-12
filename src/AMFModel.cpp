#include <iostream>
#include <fstream>
#include <sstream>

#include <Tiki/tiki.h>
#include <Tiki/drawables/console.h>

#include "debug.h"
#include "AMFModel.h"

extern ConsoleText *ct;

AMFModel::AMFModel(void) {
	isValid = false;
	displayLists = NULL;
}

AMFModel::~AMFModel(void) {
	if(displayLists) {
		if(isValid) {
			glDeleteLists(*displayLists, 1);
		}
		delete displayLists;
	}
}

void AMFModel::setFilename(const string& modelFilename) {
	filename = "models/" + modelFilename;
}

void AMFModel::load() {
	ifstream infile;
	infile.open(filename.c_str());
	if(infile.is_open()) {
		
		string buf;
		infile >> buf;
		
		if((buf.length() == 4) && (buf.substr(0, 3) == "AMF")) {
			version = buf[3] - '0';

			//total faces
			infile >> buf;
			totalFaces = atoi(buf.c_str());

			//total animations
			infile >> buf;
			totalAnimations = atoi(buf.c_str());

			//texture name
			infile >> buf;
			//skin = new Texture("models/" + buf, false);

			animations.resize(totalAnimations);

			//animations
			for(int ii=0; ii<totalAnimations; ii++) {
				animations[ii].read(infile, totalFaces);
			}

		} else {
			debug("Failed loading AMF model '%s'. Invalid model file.", filename.c_str());
		}

		infile.close();

		displayLists = new GLuint;
		displayLists[0] = glGenLists(1);

		glNewList(displayLists[0], GL_COMPILE);
			for(int ii=0; ii<totalFaces; ii++) {
				for(int jj=0; jj<3; jj++) {
					glNormal3f(
						animations[0].frames[0].faces[ii].vertices[jj].normal.x,
						animations[0].frames[0].faces[ii].vertices[jj].normal.z,
						animations[0].frames[0].faces[ii].vertices[jj].normal.y
						);
					glVertex3f(
						animations[0].frames[0].faces[ii].vertices[jj].position.x*8.0f,
						animations[0].frames[0].faces[ii].vertices[jj].position.y*8.0f - animations[0].frames[0].faces[ii].vertices[jj].position.z*14.0f,
						animations[0].frames[0].faces[ii].vertices[jj].position.z/4.0f
						);
				}
			}

		glEndList();

		isValid = true;
	} else {
		debug("Failed loading AMF model '%s'. File could not be opened for reading.", filename.c_str());
	}
}

int AMFModel::getAnimationIndex(const string& animationName) const {
	for(int ii=0; ii<totalAnimations; ii++) {
		if(!animations[ii].name.compare(animationName)) {
			return ii;
		}
	}
	return 0;
}

void AMFModel::draw(const Vector3f& position, const int animation, const Vector3f& color, const unsigned char height) const {
	if(!isValid) return;

	Vector3f &windowSize = ct->getSize();
	float xs = windowSize.x / 60.0f;
	float ys = windowSize.y / 25.0f;




	//static double st = 0.0;

	glLightfv(GL_LIGHT0, GL_DIFFUSE, (GLfloat *)&color);

	//v.x = 1.0f;
	//v.y = 0.0f;

	//glColor3fv((GLfloat *)&v);

	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, (GLfloat *)&Vector3f(sin(st),cos(st),0));
	//st += 0.00001;
	

	Vector3f pos = ct->getPosition() - (windowSize * 0.5f);
 
	//glColor3f
	glTranslatef(position.x * xs + pos.x, (position.y + 1.0f) * ys + pos.y - (float)height, 0.1f * (float)height);
	//glRotatef(st, 1.0f, 1.0f, 1.0f);

	glBegin(GL_TRIANGLES);


	glCallList(displayLists[0]);
	glEnd();
	

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 1.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);*/

	
	//glDisable(GL_LIGHTING);

	//glBegin(GL_TRIANGLES);
	/*glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(300.0f, 0.0f, 50.0f);
	glVertex3f(300.0f, 300.0f, 50.0f);*/
	//glEnd();

	/*int anim0Frame = animationTime0 / 1000;
	int anim1Frame = animationTime1 / 1000;

	float weight = float(animationTime0 % 1000) / 1000.0f;

	anim0Frame %= animations[animationIndex0].totalFrames;
	anim1Frame %= animations[animationIndex1].totalFrames;

	if(anim0Frame == anim1Frame) {
		weight = 0.0f;
	}

	int index = 0;

	if(weight == 0.0f) {
		memcpy(vb, animations[animationIndex0].frames[anim0Frame].frameVertexData, totalFaces * sizeof(CUSTOMVERTEX) * 3);
	} else {
		memcpy(vb, animations[animationIndex0].frames[anim0Frame].frameVertexData, totalFaces * sizeof(CUSTOMVERTEX) * 3);

		CUSTOMVERTEX *vb1 = animations[animationIndex0].frames[anim0Frame].frameVertexData;
		CUSTOMVERTEX *vb2 = animations[animationIndex1].frames[anim1Frame].frameVertexData;
		
		for(int ii=0; ii<totalFaces * 3; ii++) {
			vb[ii].position += weight * (vb2[ii].position - vb[ii].position);

			vb[ii].normal += weight * (vb2[ii].normal - vb[ii].normal);
			vb[ii].tangent += weight * (vb2[ii].tangent - vb[ii].tangent);
			vb[ii].binormal += weight * (vb2[ii].binormal - vb[ii].binormal);
		}
	}*/
}

Texture *AMFModel::getSkin() const {
	return skin;
}

int AMFModel::getAnimationFrames(int animationIndex) const {
	return animations[animationIndex].totalFrames;
}

int AMFModel::getTotalVertices() const {
	return totalFaces * 3;
}

///

void AMFModel::AMFAnimation::read(ifstream& infile, const int totalFaces) {
	string buf;
	
	infile >> name;
	
	infile >> buf;
	totalFrames = atoi(buf.c_str());

	frames.resize(totalFrames);

	for(int ii=0; ii<totalFrames; ii++) {
		frames[ii].read(infile, totalFaces);
	}
}

AMFModel::AMFFrame::AMFFrame() {
	//frameVertexData = NULL;
}
AMFModel::AMFFrame::~AMFFrame() {
	//SAFE_DELETE_ARRAY(frameVertexData);
}

void AMFModel::AMFFrame::read(ifstream& infile, const int totalFaces) {
	faces.resize(totalFaces);
	for(int ii=0; ii<totalFaces; ii++) {
		faces[ii].read(infile);
	}

	//SAFE_DELETE_ARRAY(frameVertexData);
	
	int totalVertices = totalFaces * 3;
	/*frameVertexData = new CUSTOMVERTEX[totalVertices];

	int vertNum = 0;
	foreach(AMFFace& face, faces) {
		for(int ii=0; ii<3; ii++) {
			frameVertexData[vertNum].position = face.vertices[ii].position;
			frameVertexData[vertNum].binormal = face.vertices[ii].binormal;
			frameVertexData[vertNum].normal = face.vertices[ii].normal;
			frameVertexData[vertNum].tangent = face.vertices[ii].tangent;

			frameVertexData[vertNum].tu = face.texCoords[ii].u;
			frameVertexData[vertNum].tv = face.texCoords[ii].v;

			vertNum++;
		}
	}

	faces.clear();
	*/
}

void AMFModel::AMFFace::read(ifstream& infile) {
	for(int ii=0; ii<3; ii++) {
		vertices[ii].read(infile);
	}
	for(int ii=0; ii<3; ii++) {
		texCoords[ii].read(infile);
	}
}

void AMFModel::AMFVertex::read(ifstream& infile) {
	string buf;

	//position
	infile >> buf;
	position.x = (float)atof(buf.c_str());
	infile >> buf;
	position.y = -(float)atof(buf.c_str());
	infile >> buf;
	position.z = (float)atof(buf.c_str());

	//normal
	infile >> buf;
	normal.x = (float)atof(buf.c_str());
	infile >> buf;
	normal.y = -(float)atof(buf.c_str());
	infile >> buf;
	normal.z = (float)atof(buf.c_str());

	//tangent
	infile >> buf;
	tangent.x = (float)atof(buf.c_str());
	infile >> buf;
	tangent.y = (float)atof(buf.c_str());
	infile >> buf;
	tangent.z = (float)atof(buf.c_str());
	tangent.normalize();

	//binormal
	infile >> buf;
	binormal.x = (float)atof(buf.c_str());
	infile >> buf;
	binormal.z = (float)atof(buf.c_str());
	infile >> buf;
	binormal.y = (float)atof(buf.c_str());

	binormal = tangent.cross(normal);

	binormal.normalize();
}

void AMFModel::AMFTexCoord::read(ifstream& infile) {
	string buf;

	//position
	infile >> buf;
	u = (float)atof(buf.c_str());
	infile >> buf;
	v = -(float)atof(buf.c_str());
}
