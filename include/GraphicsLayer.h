#ifndef __GRAPHICSLAYER_H
#define __GRAPHICSLAYER_H

#include "Tiki/gl.h"

#include "AMFModelInstance.h"

class GraphicsLayer : public Drawable {
public:
	GraphicsLayer();
	virtual ~GraphicsLayer();

	void clear();
	void clear(const int x, const int y);
	bool put(const int x, const int y, const int color, const unsigned char height, const string& modelName, const bool highlight = false);

	virtual void draw(ObjType t);

protected:
	AMFModelInstance *models[60][25];
};

#endif