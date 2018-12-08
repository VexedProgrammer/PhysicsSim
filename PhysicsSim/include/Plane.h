#pragma once
#include "Transform.h"
#include "Shader.h"
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <qquaternion.h>
#include <QtCore/QCoreApplication>
namespace Engine
{
	namespace Graphics
	{
		class Plane : public Transform, protected QOpenGLFunctions, public QObject
		{
		private:
			QVector3D m_Vertices[8] = {
				QVector3D(-0.5f, 0.5f, 0),	QVector3D(0.0f, 0.0f, -1.0f),
				QVector3D(-0.5f, -0.5f, 0),	QVector3D(0.0f, 0.0f, -1.0f),
				QVector3D(0.5f, 0.5f, 0),	QVector3D(0.0f, 0.0f, -1.0f),
				QVector3D(0.5f, -0.5f, 0),	QVector3D(0.0f, 0.0f, -1.0f)
			};
			QVector3D m_Normals[4] = {
				QVector3D(0.0f, 0.0f, -1.0f),
				QVector3D(0.0f, 0.0f, -1.0f),
				QVector3D(0.0f, 0.0f, -1.0f),
				QVector3D(0.0f, 0.0f, -1.0f)
			};

			GLuint m_Indices[6] = {
				0, 1, 2,
				1, 3, 2
			};

			QVector3D m_Normal = QVector3D(0, 0, -1); //Default normal
			QVector3D m_Colour;

			QOpenGLBuffer m_VBO;
			QOpenGLBuffer m_IBO;
			QOpenGLBuffer m_NBO;
			Shader* m_Shader;

			GLuint m_PID;
			GLuint m_VID;
			GLuint m_MID;
			GLuint m_ColourID;
		public:
			Plane(QVector3D colour, QWindow* window) : m_Colour(colour)
			{
				//window->inil
				initializeOpenGLFunctions();
				m_Shader = new Shader(window);

				m_VBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
				m_VBO.create();
				m_IBO = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
				m_IBO.create();


				m_VBO.bind();
				m_VBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
				m_VBO.allocate(m_Vertices, 8 * sizeof(QVector3D));
				m_IBO.bind();
				m_IBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
				m_IBO.allocate(m_Indices, 6 * sizeof(GLuint));
				m_VBO.release();
				m_IBO.release();

				m_PID = m_Shader->getUniLoc("P");
				m_VID = m_Shader->getUniLoc("V");
				m_MID = m_Shader->getUniLoc("M");
				m_ColourID = m_Shader->getUniLoc("Colour");

			};
			~Plane()
			{
				m_VBO.destroy();
				m_IBO.destroy();

				delete m_Shader;
			}

			const void Render(QMatrix4x4 P, QMatrix4x4 V)
			{
				m_VBO.bind();
				m_IBO.bind();
				//m_NBO.bind();
				m_Shader->Bind();

				QMatrix4x4 M = getModelMatrix();

				m_Shader->setValue(m_PID, P);
				m_Shader->setValue(m_VID, V);
				m_Shader->setValue(m_MID, M);
				m_Shader->setValue(m_ColourID, QVector4D(m_Colour, 1));


				m_Shader->setAttribBuffer(0, GL_FLOAT, (void*)0, 3, 2*sizeof(QVector3D));
				m_Shader->enableAttribArr(0);

				m_Shader->setAttribBuffer(1, GL_FLOAT, (void*)(1 * sizeof(QVector3D)), 3, 2*sizeof(QVector3D));
				m_Shader->enableAttribArr(1);
				

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

				m_Shader->Unbind();

				m_VBO.release();
				m_IBO.release();
				//m_NBO.release();
			}

			const QVector3D GetNormal() 
			{ 
				QMatrix4x4 rotationMatrix;
				rotationMatrix.rotate(EularAngles.x(), QVector3D(1, 0, 0));
				rotationMatrix.rotate(EularAngles.y(), QVector3D(0, 1, 0));
				rotationMatrix.rotate(EularAngles.z(), QVector3D(0, 0, 1));
				m_Normal = m_Normal * rotationMatrix;
				return m_Normal;
			}

		};
	}
}