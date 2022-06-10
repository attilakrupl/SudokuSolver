#include "stdafx.h"

RiddleSolver::RiddleSolver(int dimension, int adjacencySize /*! = 0 */)
	: m_dimension(dimension)
{
	for (int i = 1; i <= m_dimension; ++i)
	{
		m_basePossibilities.emplace_back(i);
	}

	m_adjacencySize = adjacencySize ? adjacencySize : static_cast<int>(std::sqrt(dimension));
	if (m_dimension % m_adjacencySize)
	{
		throw std::exception("Invalid constructor arguments for RiddleSolver");
	}

	for (int x = 0; x < m_dimension; ++x)
	{
		std::vector<Field> f;
		for (int y = 0; y < m_dimension; ++y)
		{
			f.push_back(Field(m_basePossibilities));
		}
		m_fields.push_back(f);
	}
}

void RiddleSolver::initWithRows(std::vector<std::vector<int>> initValues)
{
	for (int y = 0; y < initValues.size(); ++y)
	{
		for (int x = 0; x < initValues[y].size(); ++x)
		{
			if (initValues[y][x])
			{
				m_fields[x][y].value = initValues[y][x];
			}
		}
	}

	determineAdjacency();
}

std::vector<std::vector<RiddleSolver::Field>> RiddleSolver::solve()
{
	bool allSolved = false;
	while (!allSolved)
	{
		calculatePossibilities();
		writeValues();
		allSolved = checkState();
	}

	return m_fields;
}

void RiddleSolver::initWithFields(std::vector<std::vector<Field>> initValues)
{
	m_fields = initValues;

	determineAdjacency();
}

void RiddleSolver::calculatePossibilities()
{
	for (auto& column : m_fields)
	{
		for (auto& field : column)
		{
			field.possibilities.erase(std::remove_if(field.possibilities.begin(), field.possibilities.end(), [&](int possibility) -> bool {
				for (const auto [x, y] : field.toatlAdjacencyList)
				{
					if (m_fields[x][y].value.value_or(-1) == possibility)
					{
						return true;
					}
				}
				return false; }), field.possibilities.end());
		}
	}
}

void RiddleSolver::writeValues()
{
	for (auto& column : m_fields)
	{
		for (auto& field : column)
		{
			if (field.possibilities.size() == 1)
			{
				field.value = *field.possibilities.begin();
				field.possibilities.clear();
				return;
			}

			for (const auto possibility : field.possibilities)
			{
				for (const auto& listem : { field.rowAdjacencyList, field.columnAdjacencyList, field.boxAdjacencyList })
				{
					auto finding = find_if(listem.begin(), listem.end(), [&](const std::pair<int, int>& coord) {
						return std::any_of(m_fields[coord.first][coord.second].possibilities.cbegin(), m_fields[coord.first][coord.second].possibilities.cend(), [&](const int poss) {
							return poss == possibility; });

										   });
					if (finding == listem.end())
					{
						field.value = possibility;
						field.possibilities.clear();
						return;
					}
				}
			}
		}
	}

	auto [x, y, values] = findLowestPossibilityField();
	for (auto value : values)
	{
		RiddleSolver rs(m_dimension, m_adjacencySize);
		auto fieldCopy = m_fields;
		fieldCopy[x][y].value = value;
		fieldCopy[x][y].possibilities.clear();
		rs.initWithFields(fieldCopy);
		auto solved = rs.solve();
		if (rs.checkSum())
		{
			m_fields = solved;
			break;
		}
	}

	if (!checkSum())
	{
		throw std::exception("Got Stuck, probably calculation mistake!");
	}

}

bool RiddleSolver::checkState()
{
	for (const auto& column : m_fields)
	{
		for (const auto& field : column)
		{
			if (!field.value)
			{
				return false;
			}
		}
	}

	return true;
}

bool RiddleSolver::checkSum()
{
	std::vector<int> columnSums(m_dimension);
	std::vector<int> rowSums(m_dimension);

	for (int x = 0; x < m_dimension; ++x)
	{
		for (int y = 0; y < m_dimension; ++y)
		{
			if (m_fields[x][y].value)
			{
				columnSums.at(x) += m_fields[x][y].value.value();
				rowSums.at(y) += m_fields[x][y].value.value();
			}
		}
	}

	auto summer = [&]() -> int {
		int sum = 0;
		for (int i = 0; i < m_dimension; ++i, sum += i) {}
		return sum;
	};

	for (const auto& container : { columnSums, rowSums })
	{
		if (any_of(columnSums.cbegin(), columnSums.cend(), [&](const int sum) {
			if (sum != summer())
			{
				return true;
			}
			return false; }))
		{
			return false;
		}
	}

	return true;
}

std::tuple<int, int, std::vector<int>> RiddleSolver::findLowestPossibilityField()
{
	auto lowestPossibilityNumber = m_dimension;
	auto lowestX = -1;
	auto lowestY = -1;
	std::vector<int> leastPossibility;

	for (int y = 0; y < m_dimension; ++y)
	{
		for (int x = 0; x < m_dimension; ++x)
		{
			auto currentPossibilities = m_fields[x][y].possibilities;
			if ((currentPossibilities.size() != 0) && (currentPossibilities.size() < lowestPossibilityNumber))
			{
				lowestPossibilityNumber = currentPossibilities.size();
				lowestX = x;
				lowestY = y;
				leastPossibility = currentPossibilities;
			}
		}
	}
	return { lowestX,lowestY,leastPossibility };
}

void RiddleSolver::printRiddle(std::vector<std::vector<Field>> fields)
{
	const auto dimension = fields.size();
	for (int y = 0; y < dimension; ++y)
	{
		for (int x = 0; x < dimension; ++x)
		{
			const auto valOpt = fields[x][y].value;
			if (valOpt)
			{
				std::cout << valOpt.value() << " ";
			}
			else
			{
				std::cout << "x ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void RiddleSolver::determineAdjacency()
{
	for (int x = 0; x < m_dimension; ++x)
	{
		for (int y = 0; y < m_dimension; ++y)
		{
			for (int xa = 0; xa < m_dimension; ++xa)
			{
				if (x != xa)
				{
					m_fields[x][y].toatlAdjacencyList.insert(std::make_pair(xa, y));
					m_fields[x][y].rowAdjacencyList.insert(std::make_pair(xa, y));
				}
			}

			for (int ya = 0; ya < m_dimension; ++ya)
			{
				if (y != ya)
				{
					m_fields[x][y].toatlAdjacencyList.insert(std::make_pair(x, ya));
					m_fields[x][y].columnAdjacencyList.insert(std::make_pair(x, ya));
				}
			}

			for (int xa = x / m_adjacencySize * m_adjacencySize; xa < (x / m_adjacencySize * m_adjacencySize) + m_adjacencySize; ++xa)
			{
				for (int ya = y / m_adjacencySize * m_adjacencySize; ya < (y / m_adjacencySize * m_adjacencySize) + m_adjacencySize; ++ya)
				{
					if (!((x == xa) && (y == ya)))
					{
						m_fields[x][y].toatlAdjacencyList.insert(std::make_pair(xa, ya));
						m_fields[x][y].boxAdjacencyList.insert(std::make_pair(xa, ya));
					}
				}
			}
			//printAdjacency(x,y);
		}
	}
}

void RiddleSolver::printAdjacency(int ax, int ay)
{
	for (int y = 0; y < m_dimension; ++y)
	{
		for (int x = 0; x < m_dimension; ++x)
		{
			if (x == ax && y == ay)
			{
				std::cout << "E ";
			}
			else if (m_fields[ax][ay].toatlAdjacencyList.contains({ x,y }))
			{
				std::cout << "A ";
			}
			else
			{
				std::cout << "X ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

