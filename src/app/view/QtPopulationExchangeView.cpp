#include "view/QtPopulationExchangeView.h"

#include <QBarCategoryAxis>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QChartView>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>

#include "NeuroEvolutionEnvironment.h"
#include "utility/Histogram.h"
#include "utility/utilityRandom.h"

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
			QHBoxLayout* layoutHorz1 = new QHBoxLayout();
			layoutVert2->addLayout(layoutHorz1);
			{
				QLabel* label = new QLabel();
				label->setText("Evolution Module:");
				layoutHorz1->addWidget(label);
			}
			{
				m_evolutionIdLabel = new QLabel();
				layoutHorz1->addWidget(m_evolutionIdLabel);
			}
		}
		{
			QHBoxLayout* layoutHorz1 = new QHBoxLayout();
			layoutVert2->addLayout(layoutHorz1);
			{
				QLabel* label = new QLabel();
				label->setText("Highest Fitness:");
				layoutHorz1->addWidget(label);
			}
			{
				m_fitnessLabel = new QLabel();
				layoutHorz1->addWidget(m_fitnessLabel);
			}
		}

		layoutVert2->addStretch();
	}
}

void QtPopulationExchangeView::addPopulation(
	int evolutionViewId, float highestFitness, std::vector<NeuralNetworkGenotype> population)
{
	m_evolutionIdLabel->setText(QString::number(evolutionViewId));
	m_fitnessLabel->setText(QString::number(highestFitness));
	m_population = population;
}

std::vector<NeuralNetworkGenotype> QtPopulationExchangeView::getPopulation() const
{
	return m_population;
}
