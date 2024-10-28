#include "Iterator.h"
#include "Scan.h"
#include "Filter.h"
#include "Sort.h"
#include "Witness.h"
#include "parser.h"



int main(int argc, char* argv[])
{
	TRACE(false);

	ArgumentParser parser;
	if (!parser.parse(argc, argv)) {
		return 1;
	}

	Plan* const plan =
		new WitnessPlan("output",
			new SortPlan("*** The main thing! ***",
				new WitnessPlan("input",
					new FilterPlan("half",
						new ScanPlan("source", parser.row_number, parser.value_range), parser.predicates
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