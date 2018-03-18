#include "view/QtEvolutionView.h"

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
#include "utility/QtThreadedFunctor.h"
#include "utility/utilityRandom.h"

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
				QHBoxLayout* layoutHorz2 = new QHBoxLayout();
				layoutVert2->addLayout(layoutHorz2);

				{
					QLabel* label = new QLabel();
					label->setText("Status:");
					layoutHorz2->addWidget(label);
				}
				layoutHorz2->addStretch();
				{
					m_statusLabel = new QLabel();
					m_statusLabel->setText("All systems ready.");
					layoutHorz2->addWidget(m_statusLabel);
				}
			}
			{
				QHBoxLayout* layoutHorz2 = new QHBoxLayout();
				layoutVert2->addLayout(layoutHorz2);

				{
					QLabel* label = new QLabel();
					label->setText("Generation:");
					layoutHorz2->addWidget(label);
				}
				layoutHorz2->addStretch();
				{
					m_generationLabel = new QLabel();
					m_generationLabel->setText("-");
					layoutHorz2->addWidget(m_generationLabel);
				}
			}
			{
				QHBoxLayout* layoutHorz2 = new QHBoxLayout();
				layoutVert2->addLayout(layoutHorz2);

				{
					QLabel* label = new QLabel();
					label->setText("Maximum Fitness:");
					layoutHorz2->addWidget(label);
				}
				layoutHorz2->addStretch();
				{
					m_fitnessLabel = new QLabel();
					m_fitnessLabel->setText("-");
					layoutHorz2->addWidget(m_fitnessLabel);
				}
			}
			{
				QHBoxLayout* layoutHorz2 = new QHBoxLayout();
				layoutVert2->addLayout(layoutHorz2);

				{
					QLabel* label = new QLabel();
					label->setText("Hidden Node Count:");
					layoutHorz2->addWidget(label);
				}
				layoutHorz2->addStretch();
				{
					m_hiddenNodeCountBox = new QSpinBox();
					m_hiddenNodeCountBox->setMinimum(1);
					m_hiddenNodeCountBox->setMaximum(100);
					layoutHorz2->addWidget(m_hiddenNodeCountBox);
				}
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
		{
			QtCharts::QChart* chart = new QtCharts::QChart();
			chart->setAnimationOptions(QtCharts::QChart::NoAnimation);
			chart->legend()->hide();

			m_chartView = new QtCharts::QChartView(chart);
			m_chartView->setRenderHint(QPainter::Antialiasing);
			m_chartView->setFixedWidth(800);
			m_chartView->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding));
			layoutHorz1->addWidget(m_chartView);
		}
	}

	connect(
		this, &QtEvolutionView::requestUiUpdate,
		this, &QtEvolutionView::updateUi
	);
	
	updateButtonVisibility();
}

QtEvolutionView::~QtEvolutionView()
{
	stopEvolution();
}

void QtEvolutionView::updateUi()
{
	m_statusLabel->setText("Running evolution...");
	m_generationLabel->setText(QString::number(m_evolutionEnvironment->getGenerationCount()));
	m_fitnessLabel->setText(QString::number(m_evolutionEnvironment->getHighestFitness()));
	updateHistogram();
}

void QtEvolutionView::onStartClicked(bool checked)
{
	m_hiddenNodeCountBox->setEnabled(false);
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
	m_hiddenNodeCountBox->setEnabled(true);
	m_generationLabel->setText("-");
	m_fitnessLabel->setText("-");

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
	const int offspringSize = 20;

	m_statusLabel->setText("Initializing population...");

	m_evolutionEnvironment = std::make_shared<NeuroEvolutionEnvironment>(populationSize, offspringSize, 0.3f);

	for (int i = 0; i < populationSize; i++)
	{
		m_evolutionEnvironment->addGenotype(createRandomNeuralNetworkGenotype(m_hiddenNodeCountBox->value()));
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

					emit requestUiUpdate();
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

void QtEvolutionView::updateHistogram()
{
	if (m_evolutionEnvironment)
	{
		Histogram historgram = m_evolutionEnvironment->getFitnessHistogram(0.1f);

		QStringList categories;
		QtCharts::QBarSet* barSet = new QtCharts::QBarSet("");

		for (const Histogram::Bin& bin : historgram.getSortedBins(0.0f, 1.0f))
		{
			categories << QString::number(bin.meanValue, 'g', 2);
			*barSet << bin.count;
		}

		QtCharts::QChart* chart = m_chartView->chart();
		chart->removeAllSeries();


		QtCharts::QBarSeries* barSeries = new QtCharts::QBarSeries();
		barSeries->append(barSet);

		chart->addSeries(barSeries);

		QtCharts::QBarCategoryAxis* axis = new QtCharts::QBarCategoryAxis();
		axis->append(categories);

		chart->createDefaultAxes();
		chart->setAxisX(axis, barSeries);
	}
}
