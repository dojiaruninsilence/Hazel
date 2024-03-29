#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {

	// multi rend targ and fb refact start
	enum class FramebufferTextureFormat
	{
		None = 0,

		// color
		RGBA8,
		RED_INTEGER,    //  prep fb for mouse pick

		// depth/stencil
		DEPTH24STENCIL8,

		// defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		// todo: filtering/wrap
	};

	struct FramebufferAttachmentSpecifications
	{
		FramebufferAttachmentSpecifications() = default;
		FramebufferAttachmentSpecifications(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};
	// multi rend targ and fb refact end

	struct FramebufferSpecification
	{
		uint32_t Width, Height;
		FramebufferAttachmentSpecifications Attachments; // multi rend and fb refact
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;  //  prep fb for mouse pick

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0; // clear fb tex attach

		//virtual uint32_t GetColorAttachmentRendererID() const = 0;// multi rend and fb refact ---delete--
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};
}