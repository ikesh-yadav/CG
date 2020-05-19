#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"



class Renderer {
private:
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawWithoutIB(const VertexArray& va, const Shader& shader, int count) const;
};