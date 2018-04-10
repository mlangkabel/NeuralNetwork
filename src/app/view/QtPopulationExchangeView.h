#ifndef QT_POPULATION_EXCHANGE_VIEW_H
#define QT_POPULATION_EXCHANGE_VIEW_H

#include <QListWidget>
#include <QWidget>

#include "neural_network/NeuralNetworkConfiguration.h"

struct NeuralNetworkPopulation;

class QtPopulationExchangeView 
	: public QWidget
{
	Q_OBJECT

public:
	QtPopulationExchangeView(QWidget* parent = 0);
	virtual ~QtPopulationExchangeView();
	
	void addPopulation(int evolutionViewId, const NeuralNetworkPopulation& population);
	NeuralNetworkPopulation getPopulation() const;

private:
	void onRemoveSelectedClicked(bool checked);
	void onSaveAllClicked(bool checked);
	void save(const std::string& filePath);
	void load(const std::string& filePath);

	QListWidget* m_populationList;
};

#endif // QT_POPULATION_EXCHANGE_VIEW_H
