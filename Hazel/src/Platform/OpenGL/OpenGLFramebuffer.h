#pragma once

#include "Hazel/Renderer/FrameBuffer.h"

namespace Hazel {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		//virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }  // multi rend and fb refact ---delete--
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { HZ_CORE_ASSERT(index < m_ColorAttachments.size(), ""); return m_ColorAttachments[index]; }

		virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		//uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0; // multi rend and fb refact ---delete--
		FramebufferSpecification m_Specification;

		// multi rend and fb refact start ------ 
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
		// multi rend and fb refact end   ------ 
	};
}
