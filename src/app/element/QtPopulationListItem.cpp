#include "element/QtPopulationListItem.h"

QtPopulationListItem::QtPopulationListItem(const NeuralNetworkPopulation& population)
	: QListWidgetItem("Max Fitness: " + QString::number(population.fitness) + ", Hidden Node Count: " + QString::number(population.hiddenNodeCount))
	, m_population(population)
{
}

NeuralNetworkPopulation QtPopulationListItem::getPopulation() const
{
	return m_population;
}
