#include "ModelManager.h"

//universal static model manager
ModelManager modelManager;

ModelManager::ModelManager(void)
{
}

ModelManager::~ModelManager(void)
{
	modelList.clear();
	modelMap.clear();
}

AMFModel *ModelManager::getModel(const string& modelName)
{
	AMFModel *model;

	if(!(model = modelMap[modelName]))
	{
		model = new AMFModel;
		model->setFilename(modelName);
		model->load();
		modelList.push_back(*model);
		model = &(modelList.back());

		modelMap[modelName] = model;
	}

	return model;
}