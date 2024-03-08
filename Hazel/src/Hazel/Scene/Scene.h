#pragma once

#include "entt.hpp"


#include "Hazel/Core/Timestep.h"

namespace Hazel {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity); // add rem entts and comps first line video at 4:12

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		// add rem entts and comps
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		// add rem entts and end
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer; // save and load scenes 14 48
		friend class SceneHierarchyPanel;
	};

}