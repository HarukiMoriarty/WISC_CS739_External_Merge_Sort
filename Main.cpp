#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "Witness.h"

//TODO: Write a parser here supporting arguments (optional: --predicates=...)
void parseArgument(int argc, char* argv[], size_t& row_number, std::vector<Predicate>& predicates)
{
	assert(argc >= 4);
	assert((argc - 2) % 2 == 0);

	row_number = std::stoul(argv[1]);

	for (int i = 2; i < argc; i += 2) {
		size_t index = std::stoul(argv[i]);
		size_t value = std::stoul(argv[i + 1]);
		predicates.emplace_back(index, value);
	}
}

int main(int argc, char* argv[])
{
	TRACE(false);

	size_t row_number;
	std::vector<Predicate> predicates;
	parseArgument(argc, argv, row_number, predicates);

	Plan* const plan =
		new WitnessPlan("output",
			new SortPlan("*** The main thing! ***",
				new WitnessPlan("input",
					new FilterPlan("half",
						new ScanPlan("source", row_number), predicates
					)
				)
			)
		);

	Iterator* const it = plan->init();
	it->run();
	delete it;

	delete plan;

	return 0;
} // main