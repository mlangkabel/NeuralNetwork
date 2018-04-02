#ifndef QT_POPULATION_LIST_ITEM_H
#define QT_POPULATION_LIST_ITEM_H

#include <QListWidgetItem>

#include "NeuralNetworkGenotype.h"

class QtPopulationListItem 
	: public QListWidgetItem
{
public:
	QtPopulationListItem(float highestFitness, std::vector<NeuralNetworkGenotype> population);
	
	std::vector<NeuralNetworkGenotype> getPopulation() const;

private:
	std::vector<NeuralNetworkGenotype> m_population;
};

#endif // QT_POPULATION_LIST_ITEM_H
