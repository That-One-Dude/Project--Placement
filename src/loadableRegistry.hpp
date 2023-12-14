/******************************************************************************
* File: loadableRegistry.hpp
* Author: Tony Skeps
* Date: November 7, 2022
******************************************************************************/

#include "namespacedId.hpp"

#ifndef LOADABLE_REGISTRY_HPP
#define LOADABLE_REGISTRY_HPP

namespace Game {

    struct RegistryEntry;

    class LoadableRegistry {
    public:
        LoadableRegistry(unsigned int numBuckets);
        ~LoadableRegistry();
        void* loadEntry(NamespacedId);
        void unloadEntry(NamespacedId);
        void* getEntry(NamespacedId);
        bool isLoaded(NamespacedId);
        void reset();
    protected:
        void addEntryInternal(NamespacedId, void*);
        virtual void loadEntryInternal(NamespacedId, void**) = 0;
        virtual void unloadEntryInternal(void*) = 0;
        virtual void destroyEntryInternal(void*) = 0;
    private:
        RegistryEntry* getSpecificEntry(NamespacedId);
        RegistryEntry** entries;
        unsigned int numBuckets;
    };

    using LoaderFunction = void* (*)(NamespacedId);
    using UnloaderFunction = void (*)(void*);

    class GeneralizedLoadableRegistry: public LoadableRegistry {
    public:
        GeneralizedLoadableRegistry(unsigned int numBuckets, LoaderFunction, UnloaderFunction);
        ~GeneralizedLoadableRegistry();
        void addEntry(NamespacedId);
    protected:
        virtual void loadEntryInternal(NamespacedId, void**);
        virtual void unloadEntryInternal(void*);
        virtual void destroyEntryInternal(void*);
    private:
        LoaderFunction loaderFunction;
        UnloaderFunction unloaderFunction;
    };

    class LoadableItem {
    public:
        LoadableItem();
        ~LoadableItem();
        virtual void load() = 0;
        virtual void unload() = 0;
    };

    class IndividualizedLoadableRegistry: public LoadableRegistry {
    public:
        IndividualizedLoadableRegistry(unsigned int numBuckets);
        ~IndividualizedLoadableRegistry();
        void addEntry(NamespacedId, LoadableItem*);
    protected:
        virtual void loadEntryInternal(NamespacedId, void**);
        virtual void unloadEntryInternal(void*);
        virtual void destroyEntryInternal(void*);
    };

}

#endif