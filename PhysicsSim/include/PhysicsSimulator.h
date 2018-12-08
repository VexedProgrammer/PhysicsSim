#pragma once
#include "RenderWindow.h"
#include <QtGui\qopenglshaderprogram.h>
#include <QtGui\qscreen.h>
#include "Shader.h"
#include "Plane.h"
#include "Sphere.h"
namespace Engine
{
	class PhysicsSimulator : public RenderWindow
	{
	public:
		PhysicsSimulator();

		void initialize() override;
		void render() override;

	private:
		GLuint m_posAttr;
		GLuint m_colAttr;

		GLuint m_PID;
		GLuint m_VID;
		GLuint m_MID;

		Graphics::Shader* m_Shader;
		int m_frame;

		Graphics::Plane* m_FloorPlane;
		Graphics::Sphere* m_Ball;
	};
}

