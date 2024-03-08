#pragma once

#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/EditorCamera.h" //  edit cam

#include "entt.hpp"

namespace Hazel {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity); // add rem entts and comps first line video at 4:12

		void OnUpdateRuntime(Timestep ts); // edit cam - vid at 10 29 -- was - void OnUpdate(Timestep ts); - before being changed
		void OnUpdateEditor(Timestep ts, EditorCamera& camera); // edit cam 
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity(); // trans gizmos
	private:
		// add rem entts and comps
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