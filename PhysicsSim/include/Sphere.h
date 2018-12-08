#pragma once
#include "Transform.h"
#include "Shader.h"
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <vector>
#include <Qt3DExtras/QSphereMesh>

#define PI 3.14159265358979323846264338327950288
namespace Engine
{
	namespace Graphics
	{
		class Sphere : public Transform, protected QOpenGLFunctions
		{
		private:
			/*QVector3D m_Vertices[4] = {
				QVector3D(-0.5f, 0.5f, 0),
				QVector3D(-0.5f, -0.5f, 0),
				QVector3D(0.5f, 0.5f, 0),
				QVector3D(0.5f, -0.5f, 0)
			};

			GLuint m_Indices[6] = {
				0, 1, 2,
				1, 3, 2
			};*/

			std::vector<QVector3D> m_Vertices;
			std::vector<GLuint> m_Indices;
			std::vector<QVector3D> m_Normals;

			QVector3D m_Colour;

			QOpenGLBuffer m_VAO;
			QOpenGLBuffer m_IBO;
			GLuint m_IndexBuffer;

			Shader* m_Shader;

			GLuint m_PID;
			GLuint m_VID;
			GLuint m_MID;
			GLuint m_ColourID;

			Qt3DExtras::QSphereMesh *m_Sphere;
		public:
			Sphere(QVector3D colour, QWindow* window) : m_Colour(colour)
			{
				//window->inil
				initializeOpenGLFunctions();

				int r, s;

				float sectorStep = 2 * PI / 10;
				float stackStep = PI / 10;
				float sectorAngle, stackAngle;

				float x, y, z, xy;
				float nx, ny, nz, lengthInv = 1.0f / 1.f;

				for (r = 0; r <= 10; r++)
				{
					stackAngle = PI / 2 - r * stackStep;   // starting from pi/2 to -pi/2
					xy = 1 * cosf(stackAngle);             // r * cos(u)
					z = 1 * sinf(stackAngle);              // r * sin(u)

					for (s = 0; s <= 10; s++)
					{
						sectorAngle = s * sectorStep;
						// vertex position (x, y, z)
						x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
						y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
						m_Vertices.push_back(QVector3D(x, y, z));

						// vertex normal
						nx = x * lengthInv;
						ny = y * lengthInv;
						nz = z * lengthInv;

						m_Vertices.push_back(QVector3D(nx, ny, nz));
					}
				}

				int k1, k2;
				for (r = 0; r < 10; ++r) 
				{
					k1 = r * (10 + 1);     // beginning of current stack
					k2 = k1 + 10 + 1;      // beginning of next stack
					for (s = 0; s < 10; ++s, ++k1, ++k2) 
					{
						// 2 triangles per sector excluding 1st and last stacks
						if (r != 0)
						{
							m_Indices.push_back(k1);
							m_Indices.push_back(k2);
							m_Indices.push_back(k1 + 1);
						}

						if (r != (10 - 1))
						{
							m_Indices.push_back(k1 + 1);
							m_Indices.push_back(k2);
							m_Indices.push_back(k2 + 1);
						}

					}
				}


				m_Shader = new Shader(window);

				m_VAO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
				m_VAO.create();
				m_IBO = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
				m_IBO.create();

				m_VAO.bind();
				m_VAO.setUsagePattern(QOpenGLBuffer::StaticDraw);
				m_VAO.allocate(m_Vertices.data(), m_Vertices.size() * sizeof(QVector3D));
				m_IBO.bind();
				m_IBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
				m_IBO.allocate(m_Indices.data(), m_Indices.size() * sizeof(GLuint));
				m_VAO.release();
				m_IBO.release();

				m_PID = m_Shader->getUniLoc("P");
				m_VID = m_Shader->getUniLoc("V");
				m_MID = m_Shader->getUniLoc("M");
				m_ColourID = m_Shader->getUniLoc("Colour");



			};
			~Sphere()
			{
				m_VAO.destroy();
				m_IBO.destroy();

				delete m_Shader;
			}

			const void Render(QMatrix4x4 P, QMatrix4x4 V)
			{
				m_VAO.bind();
				m_IBO.bind();
				m_Shader->Bind();

				QMatrix4x4 M = getModelMatrix();

				m_Shader->setValue(m_PID, P);
				m_Shader->setValue(m_VID, V);
				m_Shader->setValue(m_MID, M);
				m_Shader->setValue(m_ColourID, QVector4D(m_Colour, 1));

				m_Shader->setAttribBuffer(0, GL_FLOAT, (void*)0, 3, 2 * sizeof(QVector3D));
				m_Shader->enableAttribArr(0);

				m_Shader->setAttribBuffer(1, GL_FLOAT, (void*)(1 * sizeof(QVector3D)), 3, 2 * sizeof(QVector3D));
				m_Shader->enableAttribArr(1);

				glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, (void*)0);

				m_Shader->Unbind();

				m_VAO.release();
				m_IBO.release();
			}

		};
	}
}