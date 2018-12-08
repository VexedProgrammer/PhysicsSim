#pragma once
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QKeyEvent>
#include <QtGui\qvector3d.h>
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
namespace Engine
{
	class RenderWindow : public QWindow, protected QOpenGLFunctions
	{
		Q_OBJECT
	public:
		explicit RenderWindow(QWindow *parent = 0);
		~RenderWindow();

		virtual void render(QPainter *painter);
		virtual void render();

		virtual void initialize();

		void setAnimating(bool animating);

		QVector3D m_CameraPos = QVector3D(0, 0, -5);

		public slots:
		void renderLater();
		void renderNow();

		

	protected:
		bool event(QEvent *event) override;

		void exposeEvent(QExposeEvent *event) override;

	private:
		bool m_animating;

		QOpenGLContext *m_Context;
		QOpenGLPaintDevice *m_PaintDevice;
	};
}