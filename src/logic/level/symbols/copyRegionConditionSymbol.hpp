/******************************************************************************
* File: copyRegionConditionSymbol.hpp
* Author: Tony Skeps
* Date: August 30, 2023
******************************************************************************/

#include "symbol.hpp"

#ifndef COPY_REGION_SYMBOL_HPP
#define COPY_REGION_SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    struct CellTree {
        unsigned int numEdges;
        CellTree* tree;
    };

    class CopyRegionConditionSymbol: public Symbol {
    public:
        CopyRegionConditionSymbol(Boards::CellColor, bool showSatisfaction, CellTree cellTree);
        virtual bool isSatisfied(const BoardState&, const Boards::Cell& cell) const;
    protected:
        virtual Symbol* cloneInternal();
    private:
        const CellTree cellTree;
        void markSatisfaction();
        std::vector<CopyRegionConditionSymbol*> satisfactionRecursionHelper(const CopyRegionConditionSymbol* symbol, std::unordered_set<const Boards::Cell*> cells, std::vector<CopyRegionConditionSymbol*> otherSymbols, const std::unordered_multimap<unsigned int, const Boards::Cell*>& cellList);
        mutable bool currentSatisfaction;
        mutable bool satisfactionKnown;
    };

    extern const SymbolFactory* copyRegionConditionFactory;

}

#endif