#ifndef QT_POPULATION_LIST_ITEM_H
#define QT_POPULATION_LIST_ITEM_H

#include <QListWidgetItem>

#include "NeuralNetworkConfiguration.h"

class QtPopulationListItem 
	: public QListWidgetItem
{
public:
	QtPopulationListItem(float highestFitness, std::vector<NeuralNetworkConfiguration> population);
	
	std::vector<NeuralNetworkConfiguration> getPopulation() const;

private:
	std::vector<NeuralNetworkConfiguration> m_population;
};

#endif // QT_POPULATION_LIST_ITEM_H
