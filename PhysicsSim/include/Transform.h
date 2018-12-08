#pragma once
#include <QVector3D>
#include <qmatrix4x4.h>

namespace Engine
{
	class Transform
	{
	public:
		QVector3D Position = QVector3D(0, 0, 0);
		QVector3D EularAngles = QVector3D(0, 0, 0);
		QVector3D Scale = QVector3D(1, 1, 1);

		QMatrix4x4 getModelMatrix() const
		{
			QMatrix4x4 m;
			m.translate(Position);
			m.rotate(EularAngles.x(), QVector3D(1, 0, 0));
			m.rotate(EularAngles.y(), QVector3D(0, 1, 0));
			m.rotate(EularAngles.z(), QVector3D(0, 0, 1));
			m.scale(Scale);
			return m;
		}
	};
}