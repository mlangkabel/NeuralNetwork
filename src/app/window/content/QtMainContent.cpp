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
	const int evolutionSlotCount = 5;
	QVBoxLayout* layoutVert1 = new QVBoxLayout();
	parent->setLayout(layoutVert1);

	for (int i = 0; i < evolutionSlotCount; i++)
	{
		layoutVert1->addWidget(new QtEvolutionView());
	}
}
