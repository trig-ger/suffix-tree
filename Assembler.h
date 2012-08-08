#pragma once
#include <vector>

#include "GeneralSuffixTree.h"
#include "Overlap.h"
#include <set>

class Assembler
{
public:
	Assembler(int min) : min_overlap(min), max_overlap(0) {}
	void compute_overlaps(GeneralSuffixTree&);
	void label_nodes(GeneralSuffixTree&);
	void label_node(GeneralSuffixTree&, Node*);
	void add_overlap(int, int, short int);
	std::vector<Overlap*> overlaps;
	void counting_sort();
	std::set <Overlap*> merge_overlaps(const GeneralSuffixTree&);
	void print_overlaps(const GeneralSuffixTree&);
	int min_overlap;
	int max_overlap;
};

