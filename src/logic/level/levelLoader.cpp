/******************************************************************************
* File: levelLoader.cpp
* Author: Tony Skeps
* Date: April 22, 2023
******************************************************************************/

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../../universalData.hpp"

#include "levelLoader.hpp"

namespace Game::Logic::Level {

    // Format:

    // 4 byte magic
    // PLCv where v is the version
    // refer to loadLevel(void*) for the rest

    LevelData* loadLevel(std::string fileName) {
        std::ifstream file(fileName, std::ifstream::binary);
        if (file) {
            file.seekg (0, file.end);
            unsigned int length = file.tellg();
            file.seekg (0, file.beg);

            char* buffer = new char [length];

            file.read(buffer, length);

            LevelData* result = nullptr;

            if (buffer[0] == 'P' && buffer[1] == 'L' && buffer[2] == 'C') {
                result = loadLevel(buffer + 4);
            }

            file.close();
            delete[] buffer;
            return result;

        } else {
            return nullptr;
        }
    }

    // Format:
    
    // 4 byte offset to board id
    // 4 byte offset to board data
    // 16 floats representing the transformation
    // unsigned short noting how many preplaced symbols there are
    // unsigned short noting how many placeable symbols there are
    // {unsigned short cell; 4 byte offset to symbol id; unsigned char color; 4 byte offset to symbolData} []
    // {unsigned char count; 4 byte offset to symbol id; unsigned char color; 4 byte offset to symbolData; 2 floats representing initial position; 2 floats representing initial direction; 4 floats representing recursive transformation;} []
    // random assorted information for ids and data

    template <typename T> T readData(void*& data) {
        T result = *(T*)data;
        data = data + sizeof(T);
        return result;
    }

    LevelData* loadLevel(void* data) {
        void* trueData = data;
        LevelData* result = new LevelData();
        
        unsigned int idOffset = readData<unsigned int>(trueData);
        unsigned int dataOffset = readData<unsigned int>(trueData);

        NamespacedId boardId((char*)(data + idOffset));
        Boards::Board* board = boardFactory.createBoard(boardId, data + dataOffset);

        GL::mat4 transform = readData<GL::mat4>(trueData);

        unsigned short numPreplacedSymbols = readData<unsigned short>(trueData);
        unsigned short numPlaceableSymbols = readData<unsigned short>(trueData);

        PreplacedSymbol* preplacedSymbols = new PreplacedSymbol[numPreplacedSymbols];
        for (unsigned short i = 0; i < numPreplacedSymbols; i++) {
            PreplacedSymbol& preplacedSymbol = preplacedSymbols[i];
            preplacedSymbol.cell = readData<unsigned short>(trueData);
            
            idOffset = readData<unsigned int>(trueData);
            Boards::CellColor color = readData<Boards::CellColor>(trueData);
            bool showSatisfaction = readData<bool>(trueData);
            dataOffset = readData<unsigned int>(trueData);

            NamespacedId symbolId((char*)(data + idOffset));
            preplacedSymbol.symbol = symbolFactory.createSymbol(symbolId, color, showSatisfaction, data + dataOffset);
        }

        PlaceableSymbol* placeableSymbols = new PlaceableSymbol[numPlaceableSymbols];
        for (unsigned short i = 0; i < numPlaceableSymbols; i++) {
            PlaceableSymbol& placeableSymbol = placeableSymbols[i];
            placeableSymbol.count = readData<unsigned char>(trueData);
            
            idOffset = readData<unsigned int>(trueData);
            Boards::CellColor color = readData<Boards::CellColor>(trueData);
            bool showSatisfaction = readData<bool>(trueData);
            dataOffset = readData<unsigned int>(trueData);

            NamespacedId symbolId((char*)(data + idOffset));
            placeableSymbol.symbol = symbolFactory.createPlaceableSymbol(symbolId, color, showSatisfaction, data + dataOffset);

            placeableSymbol.initialOffset = readData<GL::vec2>(trueData);
            placeableSymbol.initialDirection = readData<GL::vec2>(trueData);
            placeableSymbol.recursiveTransformation = readData<GL::mat2>(trueData);
        }
        
        result->board = board;
        result->transform = transform;
        result->numPreplacedSymbols = numPreplacedSymbols;
        result->numPlaceableSymbols = numPlaceableSymbols;
        result->preplacedSymbols = preplacedSymbols;
        result->placeableSymbols = placeableSymbols;

        return result;
    }

    void saveLevel(std::string fileName, LevelData* data) {
        std::ofstream output(fileName, std::ofstream::binary);

        char magic[] {
            'P',
            'L',
            'C',
            0
        };
        output.write(magic, 4);

        std::unordered_map<NamespacedId, unsigned int, NamespacedIdHash> idList;

        unsigned int baseDataSize = sizeof(unsigned int) * 2 + sizeof(GL::mat4) + sizeof(unsigned short) * 2 + (sizeof(unsigned short) + sizeof(unsigned int) * 2 + sizeof(Boards::CellColor)) * data->numPreplacedSymbols + (sizeof(unsigned char) + sizeof(unsigned int) * 2 + sizeof(Boards::CellColor) + sizeof(GL::vec2) * 2 + sizeof(GL::mat2)) * data->numPlaceableSymbols;

        unsigned int idSize = baseDataSize;

        idList[data->board->getRendererId()] = 0;
        unsigned int otherDataSize = boardFactory.getDataSize(data->board);

        // Collect ids

        for (unsigned int i = 0; i < data->numPreplacedSymbols; i++) {
            idList[data->preplacedSymbols[i].symbol->getRendererId()] = 0;
            otherDataSize += symbolFactory.getDataSize(data->preplacedSymbols[i].symbol);
        }

        for (unsigned int i = 0; i < data->numPlaceableSymbols; i++) {
            idList[data->placeableSymbols[i].symbol->getRendererId()] = 0;
            otherDataSize += symbolFactory.getDataSize(data->placeableSymbols[i].symbol);
        }

        // Assign positions to the ids

        for (auto iter = idList.begin(); iter != idList.end(); iter++) {
            iter->second = idSize;
            idSize += iter->first.namespaceName.size() + 2 + iter->first.id.size() + 1;
        }

        // Write the data
        unsigned int entries[1 + data->numPreplacedSymbols + data->numPlaceableSymbols];
        char otherData[otherDataSize];
        
        boardFactory.writeData(otherData, data->board);
        unsigned int currentOffset = boardFactory.getDataSize(data->board);
        if (currentOffset > 0) {
            entries[0] = idSize;
        } else {
            entries[0] = 0;
        }

        for (unsigned int i = 0; i < data->numPreplacedSymbols; i++) {
            //ababcd
            //abababcd
            symbolFactory.writeData(otherData + currentOffset, data->preplacedSymbols[i].symbol);

            unsigned int returnSpot = 0;
            unsigned int currentByte = 0;
            bool hasFoundReturnSpot = false;

            unsigned int symbolSize = symbolFactory.getDataSize(data->preplacedSymbols[i].symbol);

            if (symbolSize > 0) {
                for (unsigned int j = 0; j < currentOffset + symbolSize; j++) {
                    if (!hasFoundReturnSpot) {
                        returnSpot = j;
                        if (currentByte > 0 && otherData[j] == otherData[currentOffset]) {
                            hasFoundReturnSpot = true;
                        }
                    }
                    if (otherData[j] != otherData[currentOffset + currentByte]) {
                        j = returnSpot;
                        if (hasFoundReturnSpot) {
                            currentByte = 1;
                            hasFoundReturnSpot = false;
                        } else {
                            currentByte = 0;
                        }
                    } else {
                        currentByte++;
                        if (currentByte >= symbolSize) {
                            j++;
                            entries[i + 1] = j - symbolSize + idSize;
                            if (j > currentOffset) currentOffset = j;
                            break;
                        }
                    }
                }
            } else {
                entries[i + 1] = 0;
            }

        }

        for (unsigned int i = 0; i < data->numPlaceableSymbols; i++) {
            //ababcd
            //abababcd
            symbolFactory.writePlaceableData(otherData + currentOffset, data->placeableSymbols[i].symbol);

            unsigned int returnSpot = 0;
            unsigned int currentByte = 0;
            bool hasFoundReturnSpot = false;

            unsigned int symbolSize = symbolFactory.getPlaceableDataSize(data->placeableSymbols[i].symbol);

            if (symbolSize > 0) {
                for (unsigned int j = 0; j < currentOffset + symbolSize; j++) {
                    if (!hasFoundReturnSpot) {
                        returnSpot = j;
                        if (currentByte > 0 && otherData[j] == otherData[currentOffset]) {
                            hasFoundReturnSpot = true;
                        }
                    }
                    if (otherData[j] != otherData[currentOffset + currentByte]) {
                        j = returnSpot;
                        if (hasFoundReturnSpot) {
                            currentByte = 1;
                            hasFoundReturnSpot = false;
                        } else {
                            currentByte = 0;
                        }
                    } else {
                        currentByte++;
                        if (currentByte >= symbolSize) {
                            j++;
                            entries[i + 1] = j - symbolSize + idSize;
                            if (j > currentOffset) currentOffset = j;
                            break;
                        }
                    }
                }
            } else {
                entries[i + 1] = 0;
            }

        }


        // Write to file
        output.write((char*)&idList[data->board->getRendererId()], sizeof(unsigned int));
        output.write((char*)(entries + 0), sizeof(unsigned int));

        output.write((char*)&(data->transform), sizeof(GL::mat4));

        output.write((char*)&(data->numPreplacedSymbols), sizeof(unsigned short));
        output.write((char*)&(data->numPlaceableSymbols), sizeof(unsigned short));

        for (unsigned int i = 0; i < data->numPreplacedSymbols; i++) {
            output.write((char*)&(data->preplacedSymbols[i].cell), sizeof(unsigned short));

            Boards::CellColor color = data->preplacedSymbols[i].symbol->getColor();
            output.write((char*)&idList[data->preplacedSymbols[i].symbol->getRendererId()], sizeof(unsigned int));
            output.write((char*)&color, sizeof(Boards::CellColor));
            output.write((char*)&(data->preplacedSymbols[i].symbol->showSatisfaction), sizeof(bool));
            output.write((char*)(entries + 1 + i), sizeof(unsigned int));
        }

        for (unsigned int i = 0; i < data->numPlaceableSymbols; i++) {
            output.write((char*)&(data->placeableSymbols[i].count), sizeof(unsigned char));

            Boards::CellColor color = data->placeableSymbols[i].symbol->getColor();
            output.write((char*)&idList[data->placeableSymbols[i].symbol->getRendererId()], sizeof(unsigned int));
            output.write((char*)&color, sizeof(Boards::CellColor));
            output.write((char*)&(data->placeableSymbols[i].symbol->showSatisfaction), sizeof(bool));
            output.write((char*)(entries + 1 + data->numPreplacedSymbols + i), sizeof(unsigned int));
            output.write((char*)&(data->placeableSymbols[i].initialOffset), sizeof(GL::vec2));
            output.write((char*)&(data->placeableSymbols[i].initialDirection), sizeof(GL::vec2));
            output.write((char*)&(data->placeableSymbols[i].recursiveTransformation), sizeof(GL::mat2));
        }

        for (auto iter = idList.begin(); iter != idList.end(); iter++) {
            std::string outputString = iter->first.namespaceName + "::" + iter->first.id;
            output.write(outputString.c_str(), outputString.length() + 1);
        }

        output.write(otherData, currentOffset);

        output.close();

    }

}
