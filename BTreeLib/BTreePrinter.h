#pragma once

#include <string>
#include <vector>
#include <functional>

#include "../BTreeLib/BTree.h"
#include "../BTreeLib/BTree.cpp" // To fix template class separation

#include "../BTreeLib/BTreeNode.h"
#include "../BTreeLib/BTreeNode.cpp" // To fix template class separation

template<typename T, typename Compare = std::less<T>>
class BTreePrinter
{
	struct NodeInfo
	{
		std::string text;
		unsigned text_pos, text_end;  // half-open range
	};

	typedef std::vector<NodeInfo> LevelInfo;

	std::vector<LevelInfo> levels;

	std::string node_text(int const keys[], unsigned key_count);

	void before_traversal()
	{
		levels.resize(0);
		levels.reserve(10);   // far beyond anything that could usefully be printed
	}

	void visit(BTreeNode<T, Compare> const *node, unsigned level, unsigned child_index)
	{
		if (level >= levels.size())
			levels.resize(level + 1);

		LevelInfo &level_info = levels[level];
		NodeInfo info;

		info.text_pos = 0;
		if (!level_info.empty())  // one blank between nodes, one extra blank if left-most child
			info.text_pos = level_info.back().text_end + (child_index == 0 ? 2 : 1);

		info.text = node_text(node->keys, unsigned(node->n));

		if (node->leaf)
		{
			info.text_end = info.text_pos + unsigned(info.text.length());
		}
		else // non-leaf -> do all children so that .text_end for the right-most child becomes known
		{
			for (unsigned i = 0, e = unsigned(node->n); i <= e; ++i)  // one more pointer than there are keys
				visit(node->C[i], level + 1, i);

			info.text_end = levels[level + 1].back().text_end;
		}

		levels[level].push_back(info);
	}

	std::string BTreePrinter::node_text(int const keys[], unsigned key_count)
	{
		std::ostringstream os;
		char const *sep = "";

		os << "[";
		for (unsigned i = 0; i < key_count; ++i, sep = " ")
			os << sep << keys[i];
		os << "]";

		return os.str();
	}

	void print_blanks(unsigned n)
	{
		while (n--)
			std::cout << ' ';
	}

	void after_traversal()
	{
		for (std::size_t l = 0, level_count = levels.size(); ; )
		{
			auto const &level = levels[l];
			unsigned prev_end = 0;

			for (auto const &node : level)
			{
				unsigned total = node.text_end - node.text_pos;
				unsigned slack = total - unsigned(node.text.length());
				unsigned blanks_before = node.text_pos - prev_end;

				print_blanks(blanks_before + slack / 2);
				std::cout << node.text;

				if (&node == &level.back())
					break;

				print_blanks(slack - slack / 2);

				prev_end += blanks_before + total;
			}

			if (++l == level_count)
				break;

			std::cout << "\n\n";
		}

		std::cout << "\n";
	}

public:
	BTreePrinter(Compare& _cmp = Compare()) {
		cmp = _cmp;
	}
	void print(BTree const &tree)
	{
		before_traversal();
		visit(tree.root);
		after_traversal();
	}
};

