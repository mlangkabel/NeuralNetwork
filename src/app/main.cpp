#include <QtWidgets/QApplication>

#include "window/QtMainWindow.h"
#include "window/content/QtMainContent.h"

int main(int argc, char *argv[])
{
	//if (!ClSystem::getInstance())
	//{
	//	return EXIT_FAILURE;
	//}

	QApplication application(argc, argv);
	std::shared_ptr<QtMainWindow> mainWindow = std::make_shared<QtMainWindow>();
	mainWindow->setContent(new QtMainContent());
	mainWindow->show();
	return application.exec();
}
