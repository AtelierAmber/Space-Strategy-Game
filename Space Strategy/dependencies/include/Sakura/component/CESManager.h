#pragma once
#ifndef CESManager_h__
#define CESManager_h__

#include <vector>
#include <component/Component.h>
#include <component/Entity.h>

// TODO: IMPLEMENT ECS

namespace Sakura{

	class CESManager{
	public:

		Entity& addEntity(){

		}

	private:
		std::vector<Component*> m_allComponents;
	};

}

#endif // CESManager_h__