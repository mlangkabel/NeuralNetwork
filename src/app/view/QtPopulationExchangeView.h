#ifndef QT_POPULATION_EXCHANGE_VIEW_H
#define QT_POPULATION_EXCHANGE_VIEW_H

#include <QLabel>
#include <QWidget>

#include "NeuralNetworkGenotype.h"

class QtPopulationExchangeView 
	: public QWidget
{
	Q_OBJECT

public:
	QtPopulationExchangeView(QWidget* parent = 0);
	
	void addPopulation(int evolutionViewId, float highestFitness, std::vector<NeuralNetworkGenotype> population);
	std::vector<NeuralNetworkGenotype> getPopulation() const;

private:
	std::vector<NeuralNetworkGenotype> m_population;

	QLabel* m_evolutionIdLabel;
	QLabel* m_fitnessLabel;
};

#endif // QT_POPULATION_EXCHANGE_VIEW_H
