#include "Simulation.h"

Simulation::Simulation(QObject *parent) :
	QObject(parent),
	watchdog_(this),
	population_size_(CARS_NUMBER_ON_START) {

	watchdog_.setInterval(2000);
	connect(&watchdog_, SIGNAL(timeout()), this, SLOT(checkActivity()));
	watchdog_.start();
}
void Simulation::clearVehicles() {
	for (auto& vehicle : vehicles_) {
		vehicle.destroy();
	}
	vehicles_.clear();
	fitnesses_.clear();
}
void Simulation::newGround() {
	if (ground_ != nullptr)
	{
		Physics::ObjectsFactory::getInstance().destroyBody(*ground_.get());
	}
	ground_.reset();
	ground_ = std::shared_ptr<Objects::Ground>(GroundGenerator(TRACK_SEGMENTS, TRACK_SEGMENT_WIDTH, TRACK_SEGMENTS_DELTA).genereteNew({ 0.0f, 0.0f }));
}
void Simulation::newVehicles() {
	clearVehicles();

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> uni(0.3, 0.6);


	for (std::size_t i = 0; i < population_size_; ++i) {
		std::vector<float> points;
		for (std::size_t i = 0; i < 8; ++i) {
			points.push_back(uni(rng));
		}
		vehicles_.push_back(Objects::Vehicle(Objects::Vector2(CARS_START_X, CARS_START_Y), points, uni(rng), uni(rng)));
		fitnesses_.emplace_back(0.0f);
	}
}
void Simulation::reset() {
	newGround();
	newVehicles();
}
void Simulation::stop() {
	watchdog_.stop();
}
void Simulation::start() {
	watchdog_.start();
}
void Simulation::checkActivity() {
	bool active = false;
	for (std::size_t i = 0; i < vehicles_.size(); ++i) {
		float x_position = vehicles_[i].getPosition().x;
		if (x_position > fitnesses_[i]) {
			fitnesses_[i] = x_position;
			active = true;
		}
	}
	if (!active) {
		emit roundEnd(fitnesses_);
		newVehicles();
	}
}
void Simulation::setPopulationSize(std::size_t newSize) {
	this->population_size_ = std::min(newSize, MAXIMUM_CARS);
}
const std::vector<Objects::Vehicle> Simulation::getVehicles() const {
	return this->vehicles_;
}
const std::weak_ptr<Objects::Ground> Simulation::getGround() const {
	return this->ground_;
}
const Objects::Vehicle& Simulation::getBestVehicle() const {
	const Objects::Vehicle* best_vehicle = nullptr;

	for (const auto& vehicle : vehicles_) {
		if (best_vehicle == nullptr)
			best_vehicle = &vehicle;
		else if (best_vehicle->getPosition().x < vehicle.getPosition().x)
			best_vehicle = &vehicle;
	}

	return *best_vehicle;
}
std::size_t Simulation::getPopulationSize() const {
	return this->population_size_;
}