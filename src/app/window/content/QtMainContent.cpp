#include "window/content/QtMainContent.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include "view/QtEvolutionView.h"

QtMainContent::QtMainContent()
{
	populate(this);
}

void QtMainContent::populate(QWidget* parent)
{
	QVBoxLayout* layoutVert1 = new QVBoxLayout();
	parent->setLayout(layoutVert1);

	layoutVert1->addWidget(new QtEvolutionView());
	layoutVert1->addStretch();
}
