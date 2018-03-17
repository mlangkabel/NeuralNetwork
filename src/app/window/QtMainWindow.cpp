#include "window/QtMainWindow.h"

#include <QSettings>

QtMainWindow::QtMainWindow()
{
	{
		QSettings settings("MyCompany", "DailyQuest");
		this->restoreGeometry(settings.value("geometry").toByteArray());
		this->restoreState(settings.value("windowState").toByteArray());
	}

	QWidget* centralWidget = new QWidget();
	this->setCentralWidget(centralWidget);
}

QtMainWindow::~QtMainWindow()
{
	QSettings settings("MyCompany", "DailyQuest");
	settings.setValue("geometry", this->saveGeometry());
	settings.setValue("windowState", this->saveState());
}

void QtMainWindow::setContent(QWidget* widget)
{
	this->setCentralWidget(widget);
}

