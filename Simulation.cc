#include "Simulation.h"

Simulation::Simulation(QObject *parent) :
	QObject(parent),
	watchdog_(this),
	round_timer_(this) {

	watchdog_.setInterval(CHECK_TIME);
	round_timer_.setSingleShot(true);
	connect(&watchdog_, SIGNAL(timeout()), this, SLOT(checkActivity()));
	connect(&round_timer_, SIGNAL(timeout()), this, SLOT(endRound()));
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
	ground_ = std::shared_ptr<Objects::Ground>(GroundGenerator(TRACK_SEGMENTS, TRACK_SEGMENT_WIDTH, TRACK_SEGMENTS_DELTA).genereteNew({ -CARS_START_X, -CARS_START_Y }));
}
void Simulation::newRound(std::vector<Objects::Vehicle> vehicles) {
	clearVehicles();

	vehicles_ = vehicles;
	fitnesses_.resize(vehicles_.size(), 0.0f);

	resetTimers();
}
void Simulation::reset() {
	newGround();
}
void Simulation::stop() {
	watchdog_.stop();
	round_timer_.stop();
}
void Simulation::start() {
	watchdog_.start();
	round_timer_.start();
}
void Simulation::checkActivity() {
	bool active = false;
	for (std::size_t i = 0; i < vehicles_.size(); ++i) {
		float x_position = vehicles_[i].getPosition().x;
		if (x_position - 0.1f > fitnesses_[i]) {
			fitnesses_[i] = x_position;
			active = true;
		}
	}
	if (!active)
		endRound();
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

void Simulation::endRound() {
	emit roundEnd(fitnesses_);
}
void Simulation::resetTimers() {
	watchdog_.start(CHECK_TIME);
	round_timer_.start(MAX_ROUND_TIME);
}