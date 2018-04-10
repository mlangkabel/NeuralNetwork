#include "view/QtPopulationExchangeView.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>

#include "tinyxml/tinyxml.h"

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
			{
				QPushButton* button = new QPushButton();
				button->setText("Save all");
				connect(
					button, &QPushButton::clicked,
					this, &QtPopulationExchangeView::onSaveAllClicked
				);
				layoutHorz1->addWidget(button);
			}
		}

		layoutVert2->addStretch();
	}
	load("data/exported_populations.xml");
}

QtPopulationExchangeView::~QtPopulationExchangeView()
{
	save("data/exported_populations.xml");
}

void QtPopulationExchangeView::addPopulation(
	int evolutionViewId, const NeuralNetworkPopulation& population)
{
	{
		QtPopulationListItem* listItem = new QtPopulationListItem(population);
		m_populationList->addItem(listItem);
	}
}

NeuralNetworkPopulation QtPopulationExchangeView::getPopulation() const
{
	NeuralNetworkPopulation population;
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

void QtPopulationExchangeView::onSaveAllClicked(bool checked)
{
	save("data/exported_populations.xml");
}

void QtPopulationExchangeView::save(const std::string& filePath)
{
	TiXmlElement* elementPopulations = new TiXmlElement("populations");

	for (int i = 0; i < m_populationList->count(); i++)
	{
		QtPopulationListItem* listItem = dynamic_cast<QtPopulationListItem*>(m_populationList->item(i));
		if (listItem)
		{
			elementPopulations->LinkEndChild(neuralNetworkPopulationToXmlElement(listItem->getPopulation()));
		}
	}

	TiXmlDocument doc;
	doc.LinkEndChild(elementPopulations);
	doc.SaveFile(filePath.c_str());
}

void QtPopulationExchangeView::load(const std::string& filePath)
{
	TiXmlDocument doc(filePath.c_str());
	doc.LoadFile();
	if (doc.Error())
	{
		LOG_ERROR("Loading population from \"" + filePath + "\" failed: " + doc.ErrorDesc() + " [" + std::to_string(doc.ErrorRow()) + ":" + std::to_string(doc.ErrorCol()) + "]");
	}

	if (const TiXmlElement* elementPopulations = doc.FirstChildElement("populations"))
	{
		const std::string elementPopulationName = "population";
		const TiXmlElement* elementPopulation = elementPopulations->FirstChildElement(elementPopulationName.c_str());
		while (elementPopulation)
		{
			addPopulation(0, neuralNetworkPopulationFromXmlElement(elementPopulation));
			elementPopulation = elementPopulation->NextSiblingElement(elementPopulationName.c_str());
		}
	}
	else
	{
		LOG_ERROR("Loading population from \"" + filePath + "\" failed: Unable to find \"populations\" element.");
	}
}
