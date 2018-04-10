#ifndef QT_POPULATION_LIST_ITEM_H
#define QT_POPULATION_LIST_ITEM_H

#include <QListWidgetItem>

#include "neural_network/NeuralNetworkConfiguration.h"

#include "NeuralNetworkPopulation.h"

class QtPopulationListItem 
	: public QListWidgetItem
{
public:
	QtPopulationListItem(const NeuralNetworkPopulation& population);
	
	NeuralNetworkPopulation getPopulation() const;

private:
	const NeuralNetworkPopulation m_population;
};

#endif // QT_POPULATION_LIST_ITEM_H
