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
				QHBoxLayout* layoutHorz2 = new QHBoxLayout();
				layoutVert2->addLayout(layoutHorz2);

				{
					m_startButton = new QPushButton();
					m_startButton->setText("Start");
					connect(
						m_startButton, &QPushButton::clicked,
						this, &QtEvolutionView::onStartClicked
					);
					layoutHorz2->addWidget(m_startButton);
				}
				{
					m_stopButton = new QPushButton();
					m_stopButton->setText("Stop");
					connect(
						m_stopButton, &QPushButton::clicked,
						this, &QtEvolutionView::onStopClicked
					);
					layoutHorz2->addWidget(m_stopButton);
				}
				{
					m_resetButton = new QPushButton();
					m_resetButton->setText("Reset");
					connect(
						m_resetButton, &QPushButton::clicked,
						this, &QtEvolutionView::onResetClicked
					);
					layoutHorz2->addWidget(m_resetButton);
				}
				{
					m_continueButton = new QPushButton();
					m_continueButton->setText("Continue");
					connect(
						m_continueButton, &QPushButton::clicked,
						this, &QtEvolutionView::onContinueClicked
					);
					layoutHorz2->addWidget(m_continueButton);
				}
			}
		}
		{
			layoutHorz1->addStretch();
		}
	}
	updateButtonVisibility();
}

QtEvolutionView::~QtEvolutionView()
{
	stopEvolution();
}

void QtEvolutionView::onStartClicked(bool checked)
{
	setupEvolutionEnvironment();
	startEvolution();
	updateButtonVisibility();
}

void QtEvolutionView::onStopClicked(bool checked)
{
	stopEvolution();
	updateButtonVisibility();
}

void QtEvolutionView::onResetClicked(bool checked)
{
	m_evolutionEnvironment.reset();
	updateButtonVisibility();
}

void QtEvolutionView::onContinueClicked(bool checked)
{
	startEvolution();
	updateButtonVisibility();
}

void QtEvolutionView::updateButtonVisibility()
{
	if (m_evolutionEnvironment)
	{
		if (m_evolutionRunning)
		{
			m_startButton->setVisible(false);
			m_stopButton->setVisible(true);
			m_resetButton->setVisible(false);
			m_continueButton->setVisible(false);
		}
		else
		{
			m_startButton->setVisible(false);
			m_stopButton->setVisible(false);
			m_resetButton->setVisible(true);
			m_continueButton->setVisible(true);
		}
	}
	else
	{
		m_startButton->setVisible(true);
		m_stopButton->setVisible(false);
		m_resetButton->setVisible(false);
		m_continueButton->setVisible(false);
	}
}

void QtEvolutionView::setupEvolutionEnvironment()
{
	const int populationSize = 50;
	const int offspringSize = 10;

	m_statusLabel->setText("Initializing population...");

	m_evolutionEnvironment = std::make_shared<NeuroEvolutionEnvironment>(populationSize, offspringSize, 0.3f);

	for (int i = 0; i < populationSize; i++)
	{
		m_evolutionEnvironment->addGenotype(createRandomNeuralNetworkGenotype(20));
	}

	m_statusLabel->setText("Population initialized.");
}

void QtEvolutionView::startEvolution()
{
	if (!m_evolutionRunning)
	{
		m_evolutionRunning = true;

		m_evolutionThread = std::make_shared<std::thread>(
			[&]()
			{
				while (m_evolutionRunning)
				{
					m_evolutionEnvironment->processGeneration();
					m_statusLabel->setText(
						QString("Running evolution in generation %1 with highest fitness %2.")
						.arg(m_evolutionEnvironment->getGenerationCount())
						.arg(m_evolutionEnvironment->getHighestFitness())
					);
				}
			}
		);
	}
}

void QtEvolutionView::stopEvolution()
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
