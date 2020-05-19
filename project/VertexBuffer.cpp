#pragma once

#include <Gl/glew.h>

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &m_RenderrerID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderrerID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
};

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_RenderrerID);
};

void VertexBuffer::Bind()  const {
	glBindBuffer(GL_ARRAY_BUFFER, m_RenderrerID);
};
void VertexBuffer::UnBind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
};