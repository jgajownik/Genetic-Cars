/*!
 * @authors Jakub Gajownik, Rafał Galczak
 * @date 14.12.17
 *
 * \brief MainWindow setups view and application window structure.
 *        As well as being in charge of interacting with user.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "WorldWidget.h"
#include "SimulationView.h"
#include "StatisticView.h"
#include "Simulation.h"
#include "Loop.h"
#include "include\ObjectsFactory.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QGroupBox>
#include <QtCharts>
#include <QDebug>
#include <QFileDialog>
#include <QTimerEvent>

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
    const int WINDOW_WIDTH = 500;
    const int WINDOW_HEIGHT = 500;

	MainWindow(QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = 0);
	MainWindow(const MainWindow&) = delete;
	MainWindow& operator=(const MainWindow&) = delete;
private slots:
	void mutationRateChanged();
	void mutationSizeChanged();
	void saveToFile();
	void loadFromFile();
	void resetSimulation();
	void pauseSimulation(bool paused);
	void carsNumberChanged();
private:
	void setupSimulationInterface();
	void setupFileInterface();
	void setupAlgorithmInterface();

	Physics::Loop loop_;
	Simulation simulation_;

	//UI
	StatisticView statistic_view_;
	SimulationView simulation_view_;
	QWidget sidebar_widget_;

	//Simulation interface
	QGroupBox simulation_group_;
	QPushButton reset_button_;
	QPushButton pause_button_;
	QToolButton speed_increase_button_;
	QToolButton speed_decrease_button_;
	QLabel speed_label_;
	QLabel cars_count_label_;
	QLineEdit cars_count_edit_;

	//File interface
	QGroupBox file_group_;
	QPushButton load_button_;
	QPushButton save_button_;

	//Genetic algorithm
	QGroupBox algorithm_group_;
	QLabel mutation_size_label_;
	QLineEdit mutation_size_edit_;
	QLabel mutation_rate_label_;
	QLineEdit mutation_rate_edit_;

	//Chart
	QChart chart_;
	QLineSeries series;
	QLineSeries series2;
};

#endif