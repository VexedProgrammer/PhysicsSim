#include <QtGui/qguiapplication.h>
#include "PhysicsSimulator.h"
int main(int argc, char *argv[])
{
	QGuiApplication  a(argc, argv);

	Engine::PhysicsSimulator* window = new Engine::PhysicsSimulator();
	QSurfaceFormat format;
	format.setSamples(16);
	window->setFormat(format);
	window->resize(640, 480);
	window->show();
	
	window->setAnimating(true);

	return a.exec();
}
