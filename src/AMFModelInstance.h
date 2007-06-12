#pragma once
#include "AMFModel.h"

class AMFModelInstance {
public:
	AMFModelInstance(const string modelName, const Vector3f& position, const Vector3f& color, const int height);
	void draw() const;
	
	Vector3f position;

private:
	Vector3f color;
	int height;
	const AMFModel *model;
};
