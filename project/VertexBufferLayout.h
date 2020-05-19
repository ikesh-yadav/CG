#pragma once
#include <vector>

#include "common.h"
#include<GL/glew.h>


struct VertexBufferLayoutElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;


	static unsigned int GetSizeofType( unsigned int type) {
		switch (type) {
			case GL_FLOAT:return sizeof(GLfloat);
			case GL_UNSIGNED_INT:return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:return sizeof(GLbyte);
	
		}
		ASSERT(false);
		return(0);
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferLayoutElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() :m_Stride(0) {};
	
	template<typename T>
	void Push(unsigned int count) {
		std::cout <<T<< ":Type not allowed in vertex buffer layout";
		exit(EXIT_FAILURE);
	}

	template<>
	void Push<float>(unsigned int count) {
		unsigned int type = GL_FLOAT;
		m_Elements.push_back({type, count, GL_FALSE});
		m_Stride += count * VertexBufferLayoutElement::GetSizeofType(type);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		unsigned int type = GL_UNSIGNED_INT;
		m_Elements.push_back({ type, count, GL_FALSE });
		m_Stride += count * VertexBufferLayoutElement::GetSizeofType(type);
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		unsigned int type = GL_UNSIGNED_BYTE;
		m_Elements.push_back({ type, count, GL_TRUE });
		m_Stride += count * VertexBufferLayoutElement::GetSizeofType(type);
	}

	inline unsigned int GetStride() const { return m_Stride; }

	inline const std::vector<VertexBufferLayoutElement> GetElements() const { return m_Elements; }
};