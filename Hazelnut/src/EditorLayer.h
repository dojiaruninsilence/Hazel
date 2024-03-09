//#pragma once
//
//#include "Hazel.h"
//
//namespace Hazel {
//
//	class EditorLayer : public Layer
//	{
//	public:
//		EditorLayer();
//		virtual ~EditorLayer() = default;
//
//		virtual void OnAttach() override;
//		virtual void OnDetach() override;
//
//		void OnUpdate(Timestep ts) override;
//		virtual void OnImGuiRender() override;
//		void OnEvent(Event& e) override;
//	private:
//		OrthographicCameraController m_CameraController;
//
//		// Temp
//		Ref<VertexArray> m_SquareVA;
//		Ref<Shader> m_FlatColorShader;
//		Ref<Framebuffer> m_Framebuffer;
//
//		Ref<Texture2D> m_CheckerboardTexture;
//
//		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
//
//		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
//	};
//
//}

#pragma once

#include "Hazel.h"
#include "Panels/SceneHierarchyPanel.h"

#include "Hazel/Renderer/EditorCamera.h"

namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		// open save dialogs start
		bool OnKeyPressed(KeyPressedEvent& e); 

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
		// open save dialogs end
	private:
		Hazel::OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		Ref<Texture2D> m_CheckerboardTexture;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2]; //  prep fb for mouse pick

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GizmoType = -1;  // trans gizmos

		// panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}