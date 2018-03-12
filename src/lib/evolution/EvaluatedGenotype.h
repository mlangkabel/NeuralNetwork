#ifndef EVALUATED_GENOTYPE_H
#define EVALUATED_GENOTYPE_H

template <typename GenotypeType>
struct EvaluatedGenotype
{
	EvaluatedGenotype() {}
	EvaluatedGenotype(GenotypeType genotype, float fitness) : genotype(genotype), fitness(fitness) {}

	bool operator==(const EvaluatedGenotype& rhs)
	{
		return this == &rhs || this->genotype == rhs.genotype;
	}

	GenotypeType genotype;
	float fitness;
};

#endif // EVALUATED_GENOTYPE_H
