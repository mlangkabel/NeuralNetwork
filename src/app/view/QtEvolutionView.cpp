#include "view/QtEvolutionView.h"

#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>

#include "NeuroEvolutionEnvironment.h"

QtEvolutionView::QtEvolutionView(QWidget* parent)
	: QWidget(parent)
	, m_evolutionRunning(false)
{
	QVBoxLayout* layoutVert1 = new QVBoxLayout();
	setLayout(layoutVert1);

	{
		QGroupBox* group = new QGroupBox();
		layoutVert1->addWidget(group);

		QHBoxLayout* layoutHorz1 = new QHBoxLayout();
		group->setLayout(layoutHorz1);

		{
			QVBoxLayout* layoutVert2 = new QVBoxLayout();
			layoutHorz1->addLayout(layoutVert2);

			{
				m_statusLabel = new QLabel();
				m_statusLabel->setText("All systems ready.");
				layoutVert2->addWidget(m_statusLabel);
			}
			{
				QPushButton* startEvolutionButton = new QPushButton();
				startEvolutionButton->setText("Start");
				connect(
					startEvolutionButton, &QPushButton::clicked,
					this, &QtEvolutionView::onStartEvolutionClicked
				);
				layoutVert2->addWidget(startEvolutionButton);
			}
			{
				QPushButton* stopEvolutionButton = new QPushButton();
				stopEvolutionButton->setText("Stop");
				connect(
					stopEvolutionButton, &QPushButton::clicked,
					this, &QtEvolutionView::onStopEvolutionClicked
				);
				layoutVert2->addWidget(stopEvolutionButton);
			}
		}
		{
			layoutHorz1->addStretch();
		}
	}
}

void QtEvolutionView::onStartEvolutionClicked(bool checked)
{
	if (!m_evolutionRunning)
	{
		m_evolutionRunning = true;

		m_evolutionThread = std::make_shared<std::thread>(
			[&]()
			{
				const int populationSize = 50;
				const int offspringSize = 10;

				NeuroEvolutionEnvironment evolutionEnvironment(populationSize, offspringSize, 0.3f);

				m_statusLabel->setText("Initializing population...");

				for (int i = 0; i < populationSize; i++)
				{
					evolutionEnvironment.addGenotype(createRandomNeuralNetworkGenotype(20));
				}

				while (m_evolutionRunning)
				{
					evolutionEnvironment.processGeneration();
					m_statusLabel->setText(
						QString("Running evolution in generation %1 with highest fitness %2.")
						.arg(evolutionEnvironment.getGenerationCount())
						.arg(evolutionEnvironment.getHighestFitness())
					);
				}
			}
		);
	}
}

void QtEvolutionView::onStopEvolutionClicked(bool checked)
{
	if (m_evolutionRunning)
	{
		m_statusLabel->setText("Stopping evolution.");

		m_evolutionRunning = false;
		m_evolutionThread->join();

		m_statusLabel->setText("Evolution stopped.");

		QTimer::singleShot(
			500, 
			[&]() 
			{
				m_statusLabel->setText("All systems ready.");
			}
		);
	}
}
