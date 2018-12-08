#include "PhysicsSimulator.h"

static const char *vertexShaderSource =
"attribute highp vec4 posAttr;\n"
"attribute lowp vec4 colAttr;\n"
"varying lowp vec4 col;\n"
"uniform highp mat4 P;\n"
"uniform highp mat4 V;\n"
"uniform highp mat4 M;\n"
"void main() {\n"
"   col = colAttr;\n"
"   gl_Position = (P * V * M) * posAttr;\n"
"}\n";

static const char *fragmentShaderSource =
"varying lowp vec4 col;\n"
"void main() {\n"
"   gl_FragColor = col;\n"
"}\n";

Engine::PhysicsSimulator::PhysicsSimulator()
	: RenderWindow()
{
	setSurfaceType(QWindow::OpenGLSurface);
}

void Engine::PhysicsSimulator::initialize()
{
	m_Shader = new Graphics::Shader(this);
	m_posAttr = m_Shader->getAtrribLoc("posAttr");
	m_colAttr = m_Shader->getAtrribLoc("colAttr");
	m_PID = m_Shader->getUniLoc("P");
	m_VID = m_Shader->getUniLoc("V");
	m_MID = m_Shader->getUniLoc("M");

	m_FloorPlane = new Graphics::Plane(QVector3D(1, 1, 0), this);
	m_FloorPlane->Position.setY(-2.5f);
	m_FloorPlane->EularAngles.setX(90);
	m_FloorPlane->Scale.setX(15);
	m_FloorPlane->Scale.setY(15);

	m_Ball = new Graphics::Sphere(QVector3D(0, 0, 1), this);
	
}

void Engine::PhysicsSimulator::render()
{
	const qreal retinaScale = devicePixelRatio();

	
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	QMatrix4x4 P;
	P.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	QMatrix4x4 V;
	V.translate(m_CameraPos);

	m_Shader->Bind();

	m_FloorPlane->Render(P, V);
	m_Ball->Render(P, V);

	++m_frame;
}