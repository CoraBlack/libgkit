#include "gkit/graphic/opengl/VertexArray.hpp"
#include "gkit/graphic/opengl/VertexBuffer.hpp"
#include "gkit/graphic/opengl/VertexBufferLayout.hpp"
#include "gkit/math/vector4.hpp"
#include "gkit/math/matrix4.hpp"

gkit::graphic::opengl::VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_renderer_id);
}

gkit::graphic::opengl::VertexArray::~VertexArray() {
	if(m_renderer_id != 0) {
		glDeleteVertexArrays(1, &m_renderer_id);
		m_renderer_id = 0;
	}
}

gkit::graphic::opengl::VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_renderer_id(other.m_renderer_id)
    , m_attribIndex(other.m_attribIndex) {
    other.m_renderer_id = 0;
}

auto gkit::graphic::opengl::VertexArray::operator=(VertexArray&& other) noexcept -> VertexArray& {
    if (this != &other) {
        if (m_renderer_id != 0) {
            glDeleteVertexArrays(1, &m_renderer_id);
        }
        m_renderer_id = other.m_renderer_id;
        m_attribIndex = other.m_attribIndex;
        other.m_renderer_id = 0;
    }
    return *this;
}

auto gkit::graphic::opengl::VertexArray::add_buffer(const buffer::VertexBuffer& vb, const buffer::VertexBufferLayout& layout) -> void {
	bind();
	vb.bind();
	const auto& elements = layout.get_elements();
	size_t offset = 0;
	for (int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(m_attribIndex);
		//Specify the reading rules for the incoming data, the meaning of the arrays (coordinates, textures, etc.)
		glVertexAttribPointer(m_attribIndex, element.count, element.type,
			element.normalized, layout.get_stride(), (const void*)offset);
		offset += element.count * buffer::VertexBufferElement::get_size_of_type(element.type);
		m_attribIndex++;
	}

}

auto gkit::graphic::opengl::VertexArray::add_instance_buffer(const buffer::VertexBuffer& vb) -> void {
    bind();
	vb.bind();

    size_t vec4Size = sizeof(gkit::math::Vector4);

    for (uint32_t i = 0; i < 4; i++) {
        glEnableVertexAttribArray(m_attribIndex);
        glVertexAttribPointer(
			m_attribIndex,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(gkit::math::Matrix4),
            (void*)(i * vec4Size)
        );
        glVertexAttribDivisor(m_attribIndex, 1);
		m_attribIndex++;
    }
}

auto gkit::graphic::opengl::VertexArray::bind() const -> void {
	glBindVertexArray(m_renderer_id);
}

auto gkit::graphic::opengl::VertexArray::unbind() const -> void {
	glBindVertexArray(0);
}
