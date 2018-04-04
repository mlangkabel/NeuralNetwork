#ifndef QT_POPULATION_EXCHANGE_VIEW_H
#define QT_POPULATION_EXCHANGE_VIEW_H

#include <QListWidget>
#include <QWidget>

#include "NeuralNetworkConfiguration.h"

class QtPopulationExchangeView 
	: public QWidget
{
	Q_OBJECT

public:
	QtPopulationExchangeView(QWidget* parent = 0);
	
	void addPopulation(int evolutionViewId, float highestFitness, std::vector<NeuralNetworkConfiguration> population);
	std::vector<NeuralNetworkConfiguration> getPopulation() const;

private:
	void onRemoveSelectedClicked(bool checked);

	QListWidget* m_populationList;
};

#endif // QT_POPULATION_EXCHANGE_VIEW_H
