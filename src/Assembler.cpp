#include <algorithm>
#include "Assembler.h"
#include "GeneralSuffixTree.h"
#include "Node.h"
#include "CompareOverlap.h"

Assembler::Assembler()
{
	left_handle = -1;
}

void Assembler::label_nodes(GeneralSuffixTree& gst) {
	std::vector<Node*> to_visit (1, gst.root);
	while (!to_visit.empty()) {
		Node* current_node = to_visit.back();
		to_visit.pop_back();

		std::vector<Node*> children;
		current_node->get_children(children);
		for (int i = 0; i < children.size(); i++) {
			if (gst.tree_string[children[i]->begin_index] == '$')
				current_node->labels.push_back(children[i]->begin_index);
		}
		to_visit.insert(to_visit.end(), children.begin(), children.end());
	}
}

void Assembler::initialise_greedy_SCS(GeneralSuffixTree& gst) {
	std::string to_match;
	for (int i = 1; i < gst.tree_string.length(); i++) {
		if (gst.tree_string[i] != '$')
			to_match += gst.tree_string[i];
		else {
			mapping.insert(std::pair<int, StringMap>(i, StringMap(i)));
			push_overlap(gst, to_match, i);
			to_match.clear();
		}
	}
}

void Assembler::push_overlap(	GeneralSuffixTree& gst, 
								std::string string, 
								int string_right) 
{
	int depth = 0;
	int deepest_overlap;
	Node* node = gst.root;
	Node* deepest_node;
	while (!string.empty()) {
		if (!node->labels.empty()) {
			deepest_overlap = depth;
			deepest_node = node;
		}
		node = node->get_char_child(gst, string[0]);
		depth += node->edge_length();
		string.erase(0, node->edge_length());
	}
	overlaps.push_back(Overlap(	deepest_node, 
								deepest_node->labels.front(),
								string_right, 
								deepest_overlap));
}

void Assembler::greedy_SCS(GeneralSuffixTree& gst) {
	initialise_greedy_SCS(gst);
	std::make_heap (overlaps.begin(), overlaps.end(), CompareOverlap());
	while (overlaps.size() > 1) {
		std::pop_heap(overlaps.begin(), overlaps.end(), CompareOverlap());
		Overlap current_overlap = overlaps.back();
		overlaps.pop_back();
		Overlap lookup_overlap = gst.lookup(current_overlap, mapping);

		if (lookup_overlap.overlap == current_overlap.overlap)
			left_handle = merge_strings(lookup_overlap);
		else {
			overlaps.push_back(lookup_overlap);
			push_heap (overlaps.begin(), overlaps.end(), CompareOverlap());
		}
	}
}

int Assembler::merge_strings(Overlap overlap)
{
		int string_left = overlap.string_left;
		int string_right = overlap.string_right;
		int string_left_leftend = mapping[string_left].left_end;
		int string_right_rightend = mapping[string_right].right_end;
		mapping[string_right].left = string_left;
		mapping[string_left].right = string_right;
		mapping[string_left_leftend].right_end = mapping[string_right].right_end;
		mapping[string_right_rightend].left_end = mapping[string_left].left_end;
		mapping[string_left].deleted = true;
		mapping[string_left].suffix_overlap = overlap.overlap;
		return string_left_leftend;
}

std::string Assembler::get_SCS(GeneralSuffixTree& gst) 
{	
	int current_string_ID = left_handle;
	std::string to_return = gst.extract_string(current_string_ID);
	while (mapping[current_string_ID].right != -1)
	{
		int overlap = mapping[current_string_ID].suffix_overlap;
		current_string_ID = mapping[current_string_ID].right;
		std::string to_add = gst.extract_string(current_string_ID).substr(overlap);
		to_return += to_add;
	}
	return to_return;
}