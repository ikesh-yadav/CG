#include "Renderer.h"

void Renderer::Clear() const
{

}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind();
	va.Bind();
	ib.Bind();
	//int r;
	//glGetIntegerv(GL_CURRENT_PROGRAM, &r);
	////std::cout << r << " but the bound program is " << shader.getRendererID() <<" from renderer "<< std::endl;
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	//shader.UnBind();
	va.UnBind();
	ib.UnBind();
}

void Renderer::DrawWithoutIB(const VertexArray& va, const Shader& shader, int count) const {
	shader.Bind();
	va.Bind();
	//int r;
	//glGetIntegerv(GL_CURRENT_PROGRAM, &r);
	////std::cout << r << " but the bound program is " << shader.getRendererID() << " from renderer " << std::endl;
	glDrawArrays(GL_TRIANGLES, 0, count);
	va.UnBind();
	//shader.UnBind();
}