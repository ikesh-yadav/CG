#include "Renderer.h"

void Renderer::Clear() const
{

}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind();
	va.Bind();
	ib.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawWithoutIB(const VertexArray& va, const Shader& shader, int count) const {
	shader.Bind();
	va.Bind();
	glDrawArrays(GL_TRIANGLES, 0, count);
}