#ifndef QT_MAIN_CONTENT_H
#define QT_MAIN_CONTENT_H

#include <memory>

#include <QWidget>

class QtMainContent : public QWidget
{
	Q_OBJECT

public:
	QtMainContent();

private:
	void populate(QWidget* parent);
};

#endif // QT_MAIN_CONTENT_H
