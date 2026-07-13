#include "gkit/graphic/opengl/VertexArray.hpp"
#include "gkit/graphic/opengl/VertexBuffer.hpp"
#include "gkit/graphic/opengl/VertexBufferLayout.hpp"
#include "gkit/math/vector4.hpp"
#include "gkit/math/matrix4.hpp"

gkit::graphic::opengl::VertexArray::VertexArray() {
	glGenVertexArrays(1, &this->renderer_id);
}

gkit::graphic::opengl::VertexArray::~VertexArray() {
	if(this->renderer_id != 0) {
		glDeleteVertexArrays(1, &this->renderer_id);
		this->renderer_id = 0;
	}
}

gkit::graphic::opengl::VertexArray::VertexArray(VertexArray&& other) noexcept
    : renderer_id(other.renderer_id)
    , attrib_index(other.attrib_index) {
    other.renderer_id = 0;
}

auto gkit::graphic::opengl::VertexArray::operator=(VertexArray&& other) noexcept -> VertexArray& {
    if (this != &other) {
        if (this->renderer_id != 0) {
            glDeleteVertexArrays(1, &this->renderer_id);
        }
        this->renderer_id = other.renderer_id;
        this->attrib_index = other.attrib_index;
        other.renderer_id = 0;
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
		glEnableVertexAttribArray(this->attrib_index);
		//Specify the reading rules for the incoming data, the meaning of the arrays (coordinates, textures, etc.)
		glVertexAttribPointer(this->attrib_index, element.count, element.type,
			element.normalized, layout.get_stride(), (const void*)offset);
		offset += element.count * buffer::VertexBufferElement::get_size_of_type(element.type);
		this->attrib_index++;
	}

}

auto gkit::graphic::opengl::VertexArray::add_instance_buffer(const buffer::VertexBuffer& vb) -> void {
    bind();
	vb.bind();

    size_t vec4Size = sizeof(gkit::math::Vector4);

    for (uint32_t i = 0; i < 4; i++) {
        glEnableVertexAttribArray(this->attrib_index);
        glVertexAttribPointer(
			this->attrib_index,
            4,
            GL_FLOAT,
            GL_FALSE,
            sizeof(gkit::math::Matrix4),
            (void*)(i * vec4Size)
        );
        glVertexAttribDivisor(this->attrib_index, 1);
		this->attrib_index++;
    }
}

auto gkit::graphic::opengl::VertexArray::bind() const -> void {
	glBindVertexArray(this->renderer_id);
}

auto gkit::graphic::opengl::VertexArray::unbind() const -> void {
	glBindVertexArray(0);
}
