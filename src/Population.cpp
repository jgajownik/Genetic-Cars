//
// \author Rafa� Galczak
// \date 15.01.18
//

#include "include/Population.h"

#include <algorithm>

namespace Algorithm {
	
	Population::Population() :
		mutation_rate_(DEFAULT_MUTATION_RATE),
		next_population_size_(DEFAULT_GENERATION_SIZE) {

		rng_ = std::mt19937(rd_());
	}

	Population::Population(std::vector<Genotype> genotypes) : 
		Population() {
		genotypes_ = genotypes;
	}

	void Population::inflateRandom() {
		for (unsigned int i = 0; i < next_population_size_; ++i) {
			Genotype g;
			g.inflateWithRandom();
			genotypes_.emplace_back(g);
		}
	}

	std::vector<Genotype>& Population::getGenotypes() {
		return genotypes_;
	}

	void Population::setMutationRate(float rate)
	{
		if (rate > MAX_RAND_VALUE)
			rate = MAX_RAND_VALUE;
		mutation_rate_ = rate;
	}

	void Population::nextPopulation() {
		sort();
		std::vector<Genotype> new_population;

		for (int i = 0; i < elite_specimen; ++i) {
			new_population.push_back(genotypes_[i]);
		}
		for (int i = 0; i < (next_population_size_ - elite_specimen) / 2; ++i) {
			std::pair<Genotype, Genotype> children = getNewChildren();
			children.first.mutate(mutation_rate_);
			children.second.mutate(mutation_rate_);
			new_population.push_back(children.first);
			new_population.push_back(children.second);
		}

		genotypes_ = new_population;
	}

	std::pair<Genotype, Genotype> Population::getNewChildren() {
		Genotype a = getRandomParent();
		Genotype b = getRandomParent();

		return std::make_pair(a.cross(b), b.cross(a));
	}

	Genotype& Population::getRandomParent() {	
		float limit = 0.0f;
		for (const auto& g : genotypes_) {
			limit += g.fitness;
		}
		std::uniform_real_distribution<float> random(0.0f, limit);
		float rulette = random(rng_);
		for (auto& g : genotypes_) {
			if (g.fitness >= rulette) {
				return g;
			}
			rulette -= g.fitness;
		}
		throw std::runtime_error("No parent found!");
	}
	std::size_t Population::getNextGenerationSize() const {
		return next_population_size_;
	}
	float Population::getMutationRate() const {
		return mutation_rate_;
	}
	void Population::setNextGenerationSize(std::size_t population_size) {
		next_population_size_ = population_size;
	}

	void Population::sort() {
		std::sort(genotypes_.begin(), genotypes_.end(),
			[](const Genotype& a, const Genotype& b) {
				return a.fitness > b.fitness;
		});
	}
}