#include "Utils/MagnumScene.h"

namespace GDE
{
	MagnumScene::~MagnumScene()
	{
		recursiveRemove(&_scene);
	}
	void MagnumScene::recursiveRemove(Object3D* child)
	{
		while (!child->children().isEmpty())
		{
			recursiveRemove(child->children().first());
		}
		child->setParent(nullptr);
	}
}