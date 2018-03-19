#include "window/content/QtMainContent.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include "view/QtEvolutionView.h"
#include "view/QtPopulationExchangeView.h"

QtMainContent::QtMainContent()
{
	populate(this);
}

void QtMainContent::populate(QWidget* parent)
{
	const int evolutionSlotCount = 5;

	QHBoxLayout* layoutHorz1 = new QHBoxLayout();
	parent->setLayout(layoutHorz1);

	QtPopulationExchangeView* populationExchangeView = new QtPopulationExchangeView();

	{
		QVBoxLayout* layoutVert1 = new QVBoxLayout();
		layoutHorz1->addLayout(layoutVert1);

		for (int i = 0; i < evolutionSlotCount; i++)
		{
			layoutVert1->addWidget(new QtEvolutionView(i + 1, populationExchangeView));
		}
	}
	{
		layoutHorz1->addWidget(populationExchangeView);
	}
}
