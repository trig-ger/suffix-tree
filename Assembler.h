#pragma once
#include <string>
#include <vector>
#include <map>
#include "Overlap.h"
#include "Suffix.h"
#include "StringMap.h"

class GeneralSuffixTree;
class Assembler
{
public:
	Assembler();
	std::vector<Overlap> overlaps;
	std::map<int, StringMap> mapping; 
	int left_handle;

	void label_nodes(GeneralSuffixTree&);
	void initialise(GeneralSuffixTree&);
	void push_overlap(GeneralSuffixTree& gst, std::string, int);
	void greedy_SCS(GeneralSuffixTree&);
};

