#include "hzpch.h"
#include "Scene.h"

#include "Components.h"
#include "Hazel/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include "Entity.h"

namespace Hazel {

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		// add rem entts and comps start 

		// add rem entts and comps end
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	// add rem entts and comps start 
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
	// add rem entts and comps end

	void Scene::OnUpdateRuntime(Timestep ts) // edit cam first line vid at 10 23 -- was - void Scene::OnUpdate(Timestep ts) - before being changed
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// todo: move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		//// Render 2D               ------------trans comp ui delete
		//Camera* mainCamera = nullptr;
		//glm::mat4* cameraTransform = nullptr;
		//{
		//	auto view = m_Registry.view<TransformComponent, CameraComponent>();
		//	for (auto entity : view)
		//	{
		//		auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

		//		if (camera.Primary)
		//		{
		//			mainCamera = &camera.Camera;
		//			cameraTransform = &transform.Transform;
		//			break;
		//		}
		//	}
		//}


		// trans comp ui start
		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}
		// trans comp ui end

		if (mainCamera)
		{
			// Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform); -trans comp ui delete
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				//Renderer2D::DrawQuad(transform, sprite.Color);  -trans comp ui delete
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color); // trans comp ui
			}

			Renderer2D::EndScene();
		}

	}

	// edit cam start-------------------------------------------
	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
		}

		Renderer2D::EndScene();
	}
	// edit cam end  -------------------------------------------

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}

	// trans gizmos start
	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}
	// trans gizmos end

	// add rem entts and comps start
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
	// add rem entts and comps end

}