#pragma once
#include "AMFModel.h"

class AMFModelInstance {
public:
	AMFModelInstance(const string modelName, const Vector& position, const Vector& color, const int height);
	void draw() const;

	Vector position;

private:
	Vector color;
	int height;
	const AMFModel *model;
};
