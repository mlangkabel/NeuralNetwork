#ifndef QT_MAIN_WINDOW_H
#define QT_MAIN_WINDOW_H

#include <memory>

#include <QAction>
#include <QtWidgets/QMainWindow>

class QtMainWindow
	: public QMainWindow
{
	Q_OBJECT

public:
	QtMainWindow();
	~QtMainWindow();

	void setContent(QWidget* widget);
};

#endif // QT_MAIN_WINDOW_H
