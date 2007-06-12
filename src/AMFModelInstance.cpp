#include "AMFModelInstance.h"
#include "ModelManager.h"

using namespace std;
using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::GL::Plxcompat;


AMFModelInstance::AMFModelInstance(const string modelName, const Vector3f& position, const Vector3f& color, const int height) {
	model = modelManager.getModel(modelName);

	if(!model) {
		throw NULL;
	}

	this->position = position;
	this->color = color;
	this->height = height;
}

void AMFModelInstance::draw() const {
	model->draw(position, 0, color, height);
}
