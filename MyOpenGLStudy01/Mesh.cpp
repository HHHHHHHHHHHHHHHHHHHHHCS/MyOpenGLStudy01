﻿#include "Mesh.h"
#include <vec3.hpp>
#include <vec2.hpp>
#include <string>

Mesh::~Mesh()
{
	//push_back 是拷贝所以 这里会被析构
	// glDeleteBuffers(1,&VBO);
	// glDeleteVertexArrays(1, &VAO);
}


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	unsigned int vertSize = sizeof(Vertex);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * vertSize, &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//顶点位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertSize, reinterpret_cast<void*>(0));

	//顶点法线
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertSize, reinterpret_cast<void*>(offsetof(Vertex,Normal)));

	//顶点纹理坐标
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertSize, reinterpret_cast<void*>(offsetof(Vertex, TexCoords)));

	//顶点切线
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertSize, reinterpret_cast<void*>(offsetof(Vertex, Tangent)));

	//顶点辅助切线
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertSize, reinterpret_cast<void*>(offsetof(Vertex, Bitangent)));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader)
{
	/*
	 * OPENGL 名字格式规范
	 * uniform sampler2D texture_diffuse1;
	 * uniform sampler2D texture_diffuse2;
	 * uniform sampler2D texture_diffuse3;
	 * uniform sampler2D texture_specular1;
	 * uniform sampler2D texture_specular2;
	 * uniform sampler2D texture_normal1;
	 * uniform sampler2D texture_normal2;
	*/
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	// unsigned int emissionNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); //在绑定之前激活相应的纹理
		//获取纹理的序号(Diffuse_TextureN 的 N )
		std::string name = textures[i].type;
		std::string number;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal")
		{
			number = std::to_string(normalNr++);
		}
		// else if (name == "texture_emission")
		// {
		// 	number = std::to_string(emissionNr++);
		// }
		shader.SetInt("material." + name.append(number), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	//绘制网格
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));

	//一经配置就将所有内容设置回默认值的良好实践。
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}
