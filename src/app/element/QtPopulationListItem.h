#ifndef QT_POPULATION_LIST_ITEM_H
#define QT_POPULATION_LIST_ITEM_H

#include <QListWidgetItem>

#include "NeuralNetworkSpecification.h"

class QtPopulationListItem 
	: public QListWidgetItem
{
public:
	QtPopulationListItem(float highestFitness, std::vector<NeuralNetworkSpecification> population);
	
	std::vector<NeuralNetworkSpecification> getPopulation() const;

private:
	std::vector<NeuralNetworkSpecification> m_population;
};

#endif // QT_POPULATION_LIST_ITEM_H
