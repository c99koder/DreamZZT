#include "GraphicsLayer.h"

#include <Tiki/drawables/console.h>

using namespace Tiki;
using namespace Tiki::Hid;
using namespace std;

#include "board.h"

extern ConsoleText *ct;
extern struct world_header world;

GraphicsLayer::GraphicsLayer() {
	memset(models, 0, sizeof(AMFModelInstance *)*60*25);
}

GraphicsLayer::~GraphicsLayer() {
	clear();
}

void GraphicsLayer::clear() {
	for(int y=0; y<25; y++) {
		for(int x=0; x<60; x++) {
			if(models[x][y]) {
				delete models[x][y];
				models[x][y] = NULL;
			}
		}
	}
}

void GraphicsLayer::clear(const int x, const int y) {
	if(!world.use_3d) return;

	if(models[x][y]) {
		delete models[x][y];
		models[x][y] = NULL;
	}
}

//returns false to indicate nothing was drawn and the console should be used
bool GraphicsLayer::put(const int x, const int y, const int color, const unsigned char height, const string& modelName, const bool highlight) {
	if(!world.use_3d) return false; 

	if(models[x][y]) {
		delete models[x][y];
		models[x][y] = NULL;
	}

	if(!modelName.empty()) {
		Color cColor = ct->getConsoleColor(color);
		Tiki::Math::Vector vColor;

		if(highlight) {
			Color cHighlight(0.4f, 0.4f, 0.4f);
			cColor += cHighlight;
		}

		vColor.x = cColor.r;
		vColor.y = cColor.g;
		vColor.z = cColor.b;

		try {
			models[x][y] = new AMFModelInstance(modelName, Tiki::Math::Vector((float)x, (float)y, 0.0f), vColor, height);
			ct->putChar(x, y, 0);
		} catch(void *) {
			//the model instance was not created successfully. most likely the model could not be found.
			return false;
		}
	} else {
		return false;
	}

	return true;
}

void GraphicsLayer::draw(ObjType t) {
	if(!world.use_3d) return;

	//for(std::list<const AMFModelInstance *>::iterator iter = modelList.begin(); iter != modelList.end(); iter++) {
	//	(*iter)->draw();
	//}

	//	glPushMatrix();

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 300.0f, -100.0f);
	glMatrixMode(GL_MODELVIEW);*/

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&Vector(-.5,0.5,1,0));
	glEnable(GL_LIGHT0);

	Vector &windowSize = ct->getSize();
	int sizex = (int)windowSize.x, sizey = (int)windowSize.y;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 10.0f, -10.0f);
	glMatrixMode(GL_MODELVIEW);


	for(int y=0; y<25; y++) {
		for(int x=0; x<60; x++) {
			if(models[x][y]) {
				glPushMatrix();
				models[x][y]->draw();
				glPopMatrix();
			}
		}
	}

	glDisable(GL_LIGHT0);
	//	glPopMatrix();
	glDisable(GL_LIGHTING);


//set2d();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 1.0f, -1.0f);
	glMatrixMode(GL_MODELVIEW);
}
