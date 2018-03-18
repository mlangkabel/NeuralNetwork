#ifndef QT_EVOLUTION_VIEW_H
#define QT_EVOLUTION_VIEW_H

#include <memory>
#include <thread>

#include <QChartView>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QWidget>

class NeuroEvolutionEnvironment;

class QtEvolutionView 
	: public QWidget
{
	Q_OBJECT

public:
	QtEvolutionView(QWidget* parent = 0);
	~QtEvolutionView();

signals:
	void requestUiUpdate();

private slots:
	void updateUi();

private:
	void onStartClicked(bool checked);
	void onStopClicked(bool checked);
	void onResetClicked(bool checked);
	void onContinueClicked(bool checked);

	void updateButtonVisibility();

	void setupEvolutionEnvironment();
	void startEvolution();
	void stopEvolution();
	void updateHistogram();

	bool m_evolutionRunning;
	std::shared_ptr<std::thread> m_evolutionThread;
	std::shared_ptr<NeuroEvolutionEnvironment> m_evolutionEnvironment;

	QPushButton* m_startButton;
	QPushButton* m_stopButton;
	QPushButton* m_resetButton;
	QPushButton* m_continueButton;
	QLabel* m_statusLabel;
	QLabel* m_generationLabel;
	QLabel* m_fitnessLabel;
	QSpinBox* m_hiddenNodeCountBox;
	QtCharts::QChartView* m_chartView;
};

#endif // QT_EVOLUTION_VIEW_H
