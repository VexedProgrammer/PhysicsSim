#include "RenderWindow.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

Engine::RenderWindow::RenderWindow(QWindow *parent)
	: QWindow(parent)
	, m_animating(false)
	, m_Context(0)
	, m_PaintDevice(0)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

Engine::RenderWindow::~RenderWindow()
{
	delete m_PaintDevice;
}
void Engine::RenderWindow::render(QPainter *painter)
{
	Q_UNUSED(painter);
}

void Engine::RenderWindow::initialize()
{
	//initializeOpenGLFunctions();
}

void Engine::RenderWindow::render()
{
	if (!m_PaintDevice)
		m_PaintDevice = new QOpenGLPaintDevice;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_PaintDevice->setSize(size());

	QPainter painter(m_PaintDevice);
	render(&painter);
}

void Engine::RenderWindow::renderLater()
{
	requestUpdate();
}
#include <iostream>
bool Engine::RenderWindow::event(QEvent *event)
{

	QKeyEvent * keyEvent = static_cast<QKeyEvent*>(event);
	switch (event->type()) {
	case QEvent::UpdateRequest:
		renderNow();
		return true;
	case QEvent::KeyPress:
		
		if (keyEvent->key() == Qt::Key_A)
		{
			m_CameraPos.setX(m_CameraPos.x() + 1);
		}
		if (keyEvent->key() == Qt::Key_D)
		{
			m_CameraPos.setX(m_CameraPos.x() - 1);
		}
		if (keyEvent->key() == Qt::Key_W)
		{
			m_CameraPos.setZ(m_CameraPos.z() + 1);
		}
		if (keyEvent->key() == Qt::Key_S)
		{
			m_CameraPos.setZ(m_CameraPos.z() - 1);
		}
		return true;
	default:
		return QWindow::event(event);
	}

}

void Engine::RenderWindow::exposeEvent(QExposeEvent *event)
{
	Q_UNUSED(event);

	if (isExposed())
		renderNow();
}

void Engine::RenderWindow::renderNow()
{
	if (!isExposed())
		return;

	bool needsInitialize = false;

	if (!m_Context) {
		m_Context = new QOpenGLContext(this);
		m_Context->setFormat(requestedFormat());
		m_Context->create();

		needsInitialize = true;
	}

	m_Context->makeCurrent(this);

	if (needsInitialize) {
		initializeOpenGLFunctions();
		initialize();
	}

	render();

	m_Context->swapBuffers(this);

	if (m_animating)
		renderLater();
}

void Engine::RenderWindow::setAnimating(bool animating)
{
	m_animating = animating;

	if (animating)
		renderLater();
}