#pragma once
#include "TypeDef.h"

namespace GDE
{
	class MagnumScene
	{
	public:
		~MagnumScene();
		Scene3D& getScene() { return _scene; }
	private:
		Scene3D _scene;
		void recursiveRemove(Object3D* child);

	};
}