#pragma once

#include <optional>
#include <vector>
#include <set>
#include <list>

class RiddleSolver
{
public:
	struct Field
	{
		Field(std::vector<int> bPossibilities) :possibilities(bPossibilities) {}

		std::optional<int> value = std::nullopt;
		std::vector<int> possibilities;
		std::set<std::pair<int, int>> toatlAdjacencyList;
		std::set<std::pair<int, int>> rowAdjacencyList;
		std::set<std::pair<int, int>> columnAdjacencyList;
		std::set<std::pair<int, int>> boxAdjacencyList;
	};

public:
	RiddleSolver(int dimension, int adjacencySize = 0);

public:
	void initWithRows(std::vector<std::vector<int>> initValues);

public:
	std::vector<std::vector<Field>> solve();

public:
	static void printRiddle(std::vector<std::vector<Field>> fields);

private:
	void determineAdjacency();
	void printAdjacency(int x, int y);

	void initWithFields(std::vector<std::vector<Field>> initValues);

	void calculatePossibilities();
	void writeValues();
	bool checkState();
	bool checkSum();

	std::tuple<int, int, std::vector<int>> findLowestPossibilityField();

private:
	int m_dimension;
	int m_adjacencySize;
	std::vector<std::vector<Field>> m_fields;
	std::vector<int> m_basePossibilities;
};

