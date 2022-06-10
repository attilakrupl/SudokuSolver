#pragma once

#include <optional>
#include <vector>
#include <set>
#include <list>

class SudokuSolver
{
public:
    struct Field
    {
        Field( std::vector<int> aPossibilities ) : possibilities( aPossibilities )
        {
        }

        std::optional<int> value = std::nullopt;
        std::vector<int> possibilities;
        std::set<std::pair<int, int>> toatlAdjacencyList;
        std::set<std::pair<int, int>> rowAdjacencyList;
        std::set<std::pair<int, int>> columnAdjacencyList;
        std::set<std::pair<int, int>> boxAdjacencyList;
    };

public:
    SudokuSolver() = default;

public:
    void initWithRows( const std::vector<std::vector<int>>& initValues );

public:
    std::vector<std::vector<Field>> solve();

public:
    static void printSudoku( std::vector<std::vector<Field>> fields );

private:
    void determineAdjacency();
    void printAdjacency( int x, int y );

    void initWithFields( const std::vector<std::vector<Field>>& initValues );
    template<typename T>
    void initBaseValues( const std::vector<T>& values );

    void calculatePossibilities();
    void writeValues();
    bool checkState();
    bool checkSum();

    std::tuple<int, int, std::vector<int>> findLowestPossibilityField();

private:
    int m_dimension;
    int m_adjacencySize;
    std::vector<int> m_basePossibilities;

    std::vector<std::vector<Field>> m_fields;
};

template<typename T>
void SudokuSolver::initBaseValues( const std::vector<T>& initValues )
{
    m_dimension = initValues.size();
    for ( int i = 1; i <= m_dimension; ++i )
    {
        m_basePossibilities.emplace_back( i );
    }

    m_adjacencySize = static_cast<int>( std::sqrt( m_dimension ) );
    if ( m_dimension % m_adjacencySize )
    {
        throw std::exception( "Invalid number of columns/rows. Number of columns/rows should be square number (4,9,16...)" );
    }
}

