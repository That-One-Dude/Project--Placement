/******************************************************************************
* File: symbol.hpp
* Author: Tony Skeps
* Date: June 24, 2023
******************************************************************************/

#include <iostream>

#include <unordered_set>
#include <string>
#include "../boardStateClass.hpp"
#include "../boards/cell.hpp"
#include "../../../namespacedId.hpp"

#ifndef SYMBOL_HPP
#define SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    class Symbol {
    public:
        Symbol(std::unordered_set<NamespacedId, NamespacedIdHash> tags, Boards::CellColor color, bool showSatisfaction): tags(tags), color(color), showSatisfaction(showSatisfaction) {}
        ~Symbol() {}
        virtual bool isSatisfied(const BoardState&, const Boards::Cell&) const {return true;}
        virtual void onPlace(const BoardState&, const Boards::Cell&) {}
        virtual void onRemove(const BoardState&, const Boards::Cell&) {}
        // Generally for locking cells. You should only be touching the cell you're on
        virtual void onEarlyInvoke(BoardState&, const Boards::Cell&) {}
        virtual void onInvoke(BoardState&, const Boards::Cell&) {}
        // Generally for satisfaction
        virtual void onLateInvoke(BoardState&, const Boards::Cell&) {}
        virtual bool canBePlaced(const BoardState&, const Boards::Cell&) const {return true;}
        // This is called after "all changes" happen
        virtual void onUpdate(BoardState&, const Boards::Cell&, std::unordered_set<const Boards::Cell*> updatedCells) {}
        virtual void hover(const BoardState&, const Boards::Cell*) {}
        virtual void onAdjust(const BoardState&, const Boards::Cell*) {}
        const std::unordered_set<NamespacedId, NamespacedIdHash>& getTags() const {return tags;}
        Boards::CellColor getColor() const {return color;}
        NamespacedId getRendererId() const {return rendererId;}
        void setRendererId(NamespacedId rendererId) {this->rendererId = rendererId;}
        Symbol* clone() {
            Symbol* result = cloneInternal();
            result->rendererId = rendererId;
            return result;
        }
        const bool showSatisfaction;
    protected:
        virtual Symbol* cloneInternal() = 0;
    private:
        std::unordered_set<NamespacedId, NamespacedIdHash> tags;
        Boards::CellColor color;
        NamespacedId rendererId;
    };

    class SymbolFactory {
    public:
        SymbolFactory() {}
        ~SymbolFactory() {}
        virtual Symbol* produce(Boards::CellColor color, bool showSatisfaction, void*) const = 0;
        virtual Symbol* producePlaceable(Boards::CellColor color, bool showSatisfaction, void* data) const {
            return produce(color, showSatisfaction, data);
        }
        // Saving stuff
        virtual unsigned int getDataSize(Symbol* symbol) const = 0;
        virtual unsigned int getPlaceableDataSize(Symbol* symbol) const {return getDataSize(symbol);}
        virtual void writeData(void* data, Symbol* symbol) const = 0;
        virtual void writePlaceableData(void* data, Symbol* symbol) const {writeData(data, symbol);}
    };

}

#endif
