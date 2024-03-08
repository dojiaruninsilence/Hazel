#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include "Hazel/Scene/Components.h"

namespace Hazel {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};  // open save dialogs
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID) 
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});

		// add rem entts and comps start - idea from gpt ---------- this doesn't quite work perfectly, will work out later----------------------------------------------------------

		{
			ImGuiIO& io = ImGui::GetIO();
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 windowSize = ImGui::GetWindowSize();

			// Check if the right mouse button was clicked
			if (io.MouseClicked[1]) {
				// Check if the mouse click occurred within the window's client area
				ImVec2 mousePos = io.MousePos;
				if (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
					mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y) {
					// Open a popup window at the mouse position
					ImGui::OpenPopup("CreateEmptyEntityPopup");
				}
			}
		}

		// add rem entts and comps end - idea from gpt-------------------------------------------------------------------------------------------------------------------------------


		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		// add rem entts and comps start -------- this was the code that was replced by the gpt code --------------------------------------------------------------------------------
		//Right-click on blank space
		//if (ImGui::BeginPopupContextWindow(0, 1)/* | ImGuiPopupFlags_NoOpenOverItems*/) // in video is , false) - new imGui doesnt support, fix from video comments
		//{
		//	if (ImGui::MenuItem("Create Empty Entity"))
		//		m_Context->CreateEntity("Empty Entity");

		//	ImGui::EndPopup();
		//}
		// add rem entts and comps end

		// add rem entts and comps start - idea from gpt ---------- this doesn't quite work perfectly, will work out later----------------------------------------------------------

		if (ImGui::BeginPopup("CreateEmptyEntityPopup")) {
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");
			ImGui::EndPopup();
		}
		
		// add rem entts and comps end - idea from gpt ------------------------------------------------------------------------------------------------------------------------------

		ImGui::End();

		ImGui::Begin("Properties");

		/*if (m_SelectionContext)
			DrawComponents(m_SelectionContext);*/ //add rem entts and comps start---<----
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);

			/*if (ImGui::Button("Add Component")) //  hazel nut ui shine up delete - at 30 42 in the video
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}*/ //----------------------------------------------------------------------------------------------------------------------------------
		}
		// add rem entts and comps end

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth; // hazel nut ui shine up 
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		// add rem entts and comps start
		// call entity to be destroyed
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}
		// add rem entts and comps end


		if (opened)
		{
			ImGuiTreeNodeFlags flags = /*((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | */ImGuiTreeNodeFlags_OpenOnArrow; // // hazel nut ui shine up - i removed, dont know why it was here need to check
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth; // hazel nut ui shine up 
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(entity), flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		// add rem entts and comps start
		// actually destroy the entity
		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
		// add rem entts and comps end
	}
	// trans comp ui start -- had been working before this point, video is at 19:39
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		// hazel nut ui shine up start
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		// hazel nut ui shine up end
		
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// x - red colored
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.25f, 1.0f });
		ImGui::PushFont(boldFont); // hazel nut ui shine up
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont(); // hazel nut ui shine up
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// y - green colored
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.35f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.8f, 0.4f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.4f, 1.0f });
		ImGui::PushFont(boldFont); // hazel nut ui shine up
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont(); // hazel nut ui shine up
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		// z - blue colored
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.2f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.25f, 0.2f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont); // hazel nut ui shine up
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont(); // hazel nut ui shine up
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f);
		ImGui::PopItemWidth();
		
		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	// trans comp ui end

	// hazel nut ui shine up start - look at 18:26 in vid for ref-----
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}
	// hazel nut ui shine up end

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) 
			{
				tag = std::string(buffer);
			}
		}

		// hazel nut ui shine up start----------------------------
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component")) 
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		// hazel nut ui shine up end------------------------------

		//const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap; // add rem entt and comps add and changed all below flags to this --------- hazel nut ui shine up delete

		/*if (entity.HasComponent<TransformComponent>())  ---------------------------------------------trans comp ui delete
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
			auto& transform = entity.GetComponent<TransformComponent>().Transform;
			ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
			
			ImGui::TreePop();
			}
		}*/ 

		/*// trans comp ui start - new changes and the video is at 28:06 (had been working before these changes) --------- hazel nut ui shine up delete
		if (entity.HasComponent<TransformComponent>())
		{
			//if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))// add rem entt and comps start
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
			
			if (open)// add rem entt and comps end
			{
				auto& tc = entity.GetComponent<TransformComponent>();

				DrawVec3Control("Translation", tc.Translation);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", tc.Scale);

				ImGui::TreePop();
			}
		}
		//trans comp ui end*/ // hazel nut ui shine up delete 21:02 in the video

		/*if (entity.HasComponent<CameraComponent>())   //-------------------------------------------------- hazel nut ui shine up delete
		{
			//if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))// add rem entt and comps
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))// add rem entt and comps
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &verticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

					float orthoNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetPerspectiveNearClip(orthoNear);

					float orthoFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetPerspectiveFarClip(orthoFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);

				}

				ImGui::TreePop();
			}
		}*/ // hazel nut ui shine up delete - at 20 23 in the video.

		// hazel nut ui shine up start - camera comp start at 19:46-------------transform comp at 20:52---------------------------------------
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &verticalFov))
					camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

				float orthoNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetPerspectiveNearClip(orthoNear);

				float orthoFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetPerspectiveFarClip(orthoFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);

			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
		});
		// hazel nut ui shine up end------------------------------------------------------------------------------------

		/*if (entity.HasComponent<SpriteRendererComponent>()) //------------------------------------------hazel nut ui shine up delete------
		{
			//if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))// add rem entt and comps start
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)// add rem entt and comps end
			{
				auto& src = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
				ImGui::TreePop();
			}

			// add rem entt and comps start
			if (removeComponent)
				entity.RemoveComponent<SpriteRendererComponent>();
			// add rem entt and comps end
		}*///--------------------------------------------------------------------------------------------------------------------
	}
}