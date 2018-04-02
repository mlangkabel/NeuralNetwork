#include "view/QtPopulationExchangeView.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>

#include "element/QtPopulationListItem.h"

QtPopulationExchangeView::QtPopulationExchangeView(QWidget* parent)
	: QWidget(parent)
{
	QVBoxLayout* layoutVert1 = new QVBoxLayout();
	setLayout(layoutVert1);

	{
		QGroupBox* group = new QGroupBox();
		group->setTitle("Population Exchange Module");
		layoutVert1->addWidget(group);

		QVBoxLayout* layoutVert2 = new QVBoxLayout();
		group->setLayout(layoutVert2);

		{
			m_populationList = new QListWidget();
			layoutVert2->addWidget(m_populationList);
		}
		{
			QHBoxLayout* layoutHorz1 = new QHBoxLayout();
			layoutVert2->addLayout(layoutHorz1);

			{
				QPushButton* button = new QPushButton();
				button->setText("Remove Selected");
				connect(
					button, &QPushButton::clicked,
					this, &QtPopulationExchangeView::onRemoveSelectedClicked
				);
				layoutHorz1->addWidget(button);
			}
		}

		layoutVert2->addStretch();
	}
}

void QtPopulationExchangeView::addPopulation(
	int evolutionViewId, float highestFitness, std::vector<NeuralNetworkGenotype> population)
{
	{
		QtPopulationListItem* listItem = new QtPopulationListItem(highestFitness, population);
		m_populationList->addItem(listItem);
	}
}

std::vector<NeuralNetworkGenotype> QtPopulationExchangeView::getPopulation() const
{
	std::vector<NeuralNetworkGenotype> population;
	if (!m_populationList->selectedItems().empty())
	{
		QtPopulationListItem* listItem = dynamic_cast<QtPopulationListItem*>(m_populationList->selectedItems().front());
		if (listItem)
		{
			population = listItem->getPopulation();
		}
	}

	return population;
}

void QtPopulationExchangeView::onRemoveSelectedClicked(bool checked)
{
	for (QListWidgetItem* listItem : m_populationList->selectedItems())
	{
		m_populationList->takeItem(m_populationList->row(listItem));
	}
}
