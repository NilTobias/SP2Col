#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "Vertex.h"
#include "MyMath.h"
#include "LoadOBJ.h"

Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);

	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(0.5f,-0.5f,0.f);
	v.color = color;
	v.normal.Set(0,0,1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f,0.5f,0.f);	
	v.color = color;
	v.normal.Set(0,0,1);
	v.texCoord.Set(1, 0);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f,0.5f,0.f);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1, 1);
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f,-0.5f,0.f);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 1);
	vertex_buffer_data.push_back(v);

	std::vector <GLuint> index_buffer_data;

	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color ,float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(-0.5f,-0.5f,-0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f,-0.5f,-0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f, 0.5f,-0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f, 0.5f,-0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f,-0.5f, 0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f,-0.5f, 0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f, 0.5f, 0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f, 0.5f, 0.5f);
	v.color = color;
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;

	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = 36;
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateCircle(const std::string &meshName, Color color, unsigned numSlice,float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector <GLuint> index_buffer_data;

	float degreePerSlice =360.f/numSlice;

	for (unsigned slice = 0; slice < numSlice; ++slice)
	{
		//v0
		float theta = slice * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0,1,0);
		vertex_buffer_data.push_back(v);

		//v1
		v.pos.Set(0,0,0);
		v.color = color;
		v.normal.Set(0,1,0);
		vertex_buffer_data.push_back(v);

		//v2
		float theta2 = (slice + 1) * degreePerSlice;
		v.pos.Set(radius * cos(Math::DegreeToRadian(theta2)), 0, radius * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		v.normal.Set(0,1,0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned slice = 0; slice <numSlice; ++slice)
	{
		index_buffer_data.push_back(slice * 3 + 0);
		index_buffer_data.push_back(slice * 3 + 1);
		index_buffer_data.push_back(slice * 3 + 2);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlice,float outerR, float innerR)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector <GLuint> index_buffer_data;

	float degreePerSlice =360.f/numSlice;

	for (unsigned slice = 0; slice < numSlice; ++slice)
	{
		//v0
		float theta = slice * degreePerSlice;
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0,1,0);
		vertex_buffer_data.push_back(v);

		//v1
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal.Set(0,1,0);
		vertex_buffer_data.push_back(v);

		//v2
		float theta2 = (slice + 1) * degreePerSlice;
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta2)), 0, outerR * sin(Math::DegreeToRadian(theta2)));
		v.normal.Set(0,1,0);
		v.color = color;
		vertex_buffer_data.push_back(v);

		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta2)), 0, innerR * sin(Math::DegreeToRadian(theta2)));
		v.color = color;
		v.normal.Set(0,1,0);
		vertex_buffer_data.push_back(v);

	}

	for (unsigned slice = 0; slice <numSlice; ++slice)
	{
		index_buffer_data.push_back(slice * 4 + 0);
		index_buffer_data.push_back(slice * 4 + 1);
		index_buffer_data.push_back(slice * 4 + 2);

		index_buffer_data.push_back(slice * 4 + 3);
		index_buffer_data.push_back(slice * 4 + 2);
		index_buffer_data.push_back(slice * 4 + 1);
	}


	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

float sphereX (float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}

float sphereY (float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}

float sphereZ (float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack,unsigned numSlice,float radius)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector <GLuint> index_buffer_data;

	float degreePerStack = 180.f /numStack;
	float degreePerSlice =360.f/numSlice;

	for (unsigned stack = 0; stack < numStack + 1; ++stack)
	{
		float phi = -90 + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			float theta = slice * degreePerSlice;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi,theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta),sphereY(phi,theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);

		}
	}
	for(unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice <numSlice + 1; ++slice)
		{
			index_buffer_data.push_back(stack *(numSlice + 1) + slice);
			index_buffer_data.push_back((stack + 1) *(numSlice + 1) + slice);

		}

	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path) {
	//Read vertices, texcoords & normals from OBJ
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if(!success)
		return NULL;

	//Index the vertices, texcoords & normals properly
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	//Create the mesh and call glBindBuffer, glBufferData
	//Use triangle list and remember to set index size
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector <GLuint> index_buffer_data;

	float height = 1.f / numRow;
	float width = 1.f / numCol;
	int offset = 0;
	for(unsigned i = 0; i < numRow; ++i)
	{
		for(unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height -  i * height;
			v.pos.Set(-0.5,-0.5,0.f);
			v.texCoord.Set(u1 , v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5,-0.5,0.f);	
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5,0.5,0.f);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5, 0.5, 0.f);
			v.texCoord.Set(u1, v1 + height);

			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset +=4;
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}



