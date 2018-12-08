#pragma once

#include <QtGui\qopenglshaderprogram.h>
#include <QtGui/QOpenGLFunctions>
#include <QtGui\qwindow.h>

namespace Engine
{
	namespace Graphics
	{

		static const char *vertexShaderSource =
			"#version 330 core\n"
			"layout (location = 0) in vec4 posAttr;\n"
			"layout (location = 1) in vec3 normAttr;\n"
			"varying lowp vec4 col;\n"
			"varying highp vec3 norm;\n"
			"varying highp vec3 fragPos;\n"
			"uniform highp mat4 P;\n"
			"uniform highp mat4 V;\n"
			"uniform highp mat4 M;\n"
			"uniform lowp vec4 Colour;\n"
			"lowp vec3 lightDir = vec3(0, -1, 0);\n"
			"varying highp vec3 lDir;\n"
			"void main() {\n"
			"   col = Colour;\n"
			"   lDir = mat3(V)*normalize(-lightDir);\n"
			"   norm = mat3(transpose(inverse(M))) * normAttr;\n"
			"   fragPos = vec3(M * posAttr);\n"
			"   gl_Position = (P * V * M) * posAttr;\n"
			"}\n";

		static const char *fragmentShaderSource =
			"varying lowp vec4 col;\n"
			"varying highp vec3 norm;\n"
			"varying highp vec3 fragPos;\n"
			"varying highp vec3 lDir;\n"
			"lowp vec4 lightColour = vec4(1, 1, 1, 1);\n"
			"lowp vec4 ambLight = vec4(0.1, 0.1, 0.1, 1);\n"
			"void main() {\n"
			"   highp vec3 normN = normalize(norm);\n"
			"   float diff =  max(dot(normN, lDir), 0.0);\n"
			"   highp vec4 diffuse = diff * lightColour;\n"
			"   gl_FragColor = (ambLight+ diffuse)*col;\n"
			"}\n";
		class Shader : protected QOpenGLFunctions
		{
		private:
			QOpenGLShaderProgram *m_Program;
		public:
	
			Shader(QWindow* window)
			{

				m_Program = new QOpenGLShaderProgram(window);
				m_Program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
				m_Program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
				m_Program->link();
			}
			~Shader()
			{
				delete m_Program;
			}
			const void Bind() const
			{
				m_Program->bind();
			}
			const void Unbind() const
			{
				m_Program->release();
			}
			GLuint getAtrribLoc(QString attibName) const { return m_Program->attributeLocation(attibName); }
			GLuint getUniLoc(QString uniName) const { return m_Program->uniformLocation(uniName); }
			const void enableAttribArr(GLuint location) const { m_Program->enableAttributeArray(location); }
			const void setAttribBuffer(GLuint location, GLenum type, void* values, unsigned int elements, size_t size) const { m_Program->setAttributeArray(location, type, values, elements, size); }
			const void setValue(GLuint ID, QMatrix4x4 value) { m_Program->setUniformValue(ID, value); }
			const void setValue(GLuint ID, QVector4D value) { m_Program->setUniformValue(ID, value); }


		};
	}
}