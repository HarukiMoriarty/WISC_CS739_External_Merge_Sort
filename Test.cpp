#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "Witness.h"

int main(int argc, char* argv[])
{
	TRACE(false);

	std::vector<Predicate> predicates;
	predicates.emplace_back(0, 1000);
	predicates.emplace_back(2, 5000);

	Plan* const plan =
		new WitnessPlan("output",
			new SortPlan("*** The main thing! ***",
				new WitnessPlan("input",
					new FilterPlan("half",
						new ScanPlan("source", 10), predicates
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
