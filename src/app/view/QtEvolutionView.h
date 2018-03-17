#ifndef QT_EVOLUTION_VIEW_H
#define QT_EVOLUTION_VIEW_H

#include <memory>
#include <thread>

#include <QLabel>
#include <QWidget>

class QtEvolutionView 
	: public QWidget
{
	Q_OBJECT

public:
	QtEvolutionView(QWidget* parent = 0);

private:
	void onStartEvolutionClicked(bool checked);
	void onStopEvolutionClicked(bool checked);

	bool m_evolutionRunning;
	std::shared_ptr<std::thread> m_evolutionThread;

	QLabel* m_statusLabel;
};

#endif // QT_EVOLUTION_VIEW_H
