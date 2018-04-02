#include "element/QtPopulationListItem.h"

QtPopulationListItem::QtPopulationListItem(float highestFitness, std::vector<NeuralNetworkGenotype> population)
	: QListWidgetItem("Max Fitness: " + QString::number(highestFitness) + (population.empty() ? "" : (", Hidden Node Count: " + QString::number(population.front().hiddenNodeAmount))))
	, m_population(population)
{
}

std::vector<NeuralNetworkGenotype> QtPopulationListItem::getPopulation() const
{
	return m_population;
}
