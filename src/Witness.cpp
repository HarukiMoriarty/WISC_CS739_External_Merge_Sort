#include "Witness.h"

WitnessPlan::WitnessPlan(char const* const name, Plan* const input)
	: Plan(name), _input(input)
{
	TRACE(false);
} // WitnessPlan::WitnessPlan

WitnessPlan::~WitnessPlan()
{
	TRACE(false);
	delete _input;
} // WitnessPlan::~WitnessPlan

Iterator* WitnessPlan::init() const
{
	TRACE(false);
	return new WitnessIterator(this);
} // WitnessPlan::init

WitnessIterator::WitnessIterator(WitnessPlan const* const plan) :
	_plan(plan), _input(plan->_input->init()),
	_rows(0), in_order(true)
{
	TRACE(false);
	for (auto& item : _parity) {
		item = 0;
	}
} // WitnessIterator::WitnessIterator

WitnessIterator::~WitnessIterator()
{
	TRACE(false);

	delete _input;

	traceprintf("%s witnessed %lu rows\n", _plan->_name, (unsigned long)(_rows));
	writeObservation();
} // WitnessIterator::~WitnessIterator

bool WitnessIterator::next(Row& row)
{
	TRACE(false);

	if (!_input->next(row))  return false;
	checkOrder(row);
	calculateParity(row);
	++_rows;
	return true;
} // WitnessIterator::next

void WitnessIterator::free(Row& row)
{
	TRACE(false);
	_input->free(row);
} // WitnessIterator::free

void WitnessIterator::calculateParity(Row& row)
{
	for (size_t i = 0; i < ROW_LENTH; i++)
	{
		_parity[i] ^= row.getData(i);
	}
} // WitnessIterator::calParity

void WitnessIterator::writeObservation()
{
	std::ofstream outfile("witness.output", std::ios_base::app);
	if (outfile.is_open()) {
		outfile << _plan->_name << " witnessed " << (in_order ? "true" : "false") << " order\n";
		outfile << "Parity " << _plan->_name << " sort: ";
		for (auto& item : _parity) {
			outfile << item << " ";
		}
		outfile << "\n";
	}
	else {
		printf("Cannot open witness output file\n");
	}
} // WitnessIterator::printParity

void WitnessIterator::checkOrder(Row& row)
{
	if (in_order)
	{
		if (row >= last_row)
		{
			last_row = row;
			return;
		}
		in_order = false;
	}
} // WitnessIterator::checkOrder
