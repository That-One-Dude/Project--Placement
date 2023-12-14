/******************************************************************************
* File: loadableRegistry.hpp
* Author: Tony Skeps
* Date: November 7, 2022
******************************************************************************/

#include "loadableRegistry.hpp"

namespace Game {

    struct RegistryEntry {
        RegistryEntry(NamespacedId id);
        ~RegistryEntry();
        NamespacedId id;
        void* item = nullptr;
        unsigned int numLoaders = 0;
        RegistryEntry* next = nullptr;
    };

    RegistryEntry::RegistryEntry(NamespacedId id): id(id) {}

    RegistryEntry::~RegistryEntry() {
        if (next != nullptr) {
            delete next;
        }
    }

    LoadableRegistry::LoadableRegistry(unsigned int numBuckets): entries(new RegistryEntry*[numBuckets]), numBuckets(numBuckets) {
        for (int i = 0; i < numBuckets; i++) {
            entries[i] = nullptr;
        }
    }

    LoadableRegistry::~LoadableRegistry() {
        for (int i = 0; i < numBuckets; i++) {
            delete entries[i];
        }

        delete entries;
    }

    void LoadableRegistry::addEntryInternal(NamespacedId id, void* item) {
        RegistryEntry** entry = entries + (NamespacedIdHash()(id) % numBuckets);
        RegistryEntry* newEntry = new RegistryEntry(id);
        newEntry->item = item;
        newEntry->next = *entry;
        *entry = newEntry;
    }

    /*void* LoadableRegistry::loadEntry(NamespacedId id) {
        Entry* currentEntry = getSpecificEntry(id);
        if (currentEntry != nullptr) {
            if (currentEntry->numLoaders <= 0) {
                currentEntry->item = loaderFunction(id);
            }
            currentEntry->numLoaders++;
        }
    }

    void LoadableRegistry::unloadEntry(NamespacedId id) {
        Entry* currentEntry = getSpecificEntry(id);
        if (currentEntry != nullptr) {
            if (currentEntry->numLoaders > 0) {
                currentEntry->numLoaders--;
                if (currentEntry->numLoaders == 0) {
                    unloaderFunction(currentEntry->item);
                }
            }
        }
    }*/

    void* LoadableRegistry::loadEntry(NamespacedId id) {
        RegistryEntry* currentEntry = getSpecificEntry(id);
        if (currentEntry != nullptr) {
            if (currentEntry->numLoaders <= 0) {
                loadEntryInternal(currentEntry->id, &currentEntry->item);
            }
            currentEntry->numLoaders++;
            return currentEntry->item;
        } else {
            return nullptr;
        }
    }

    void LoadableRegistry::unloadEntry(NamespacedId id) {
        RegistryEntry* currentEntry = getSpecificEntry(id);
        if (currentEntry != nullptr) {
            if (currentEntry->numLoaders > 0) {
                currentEntry->numLoaders--;
                if (currentEntry->numLoaders == 0) {
                    unloadEntryInternal(currentEntry->item);
                }
            }
        }
    }

    void* LoadableRegistry::getEntry(NamespacedId id) {
        RegistryEntry* currentEntry = getSpecificEntry(id);
        if (currentEntry != nullptr) {
            return currentEntry->item;
        } else {
            return nullptr;
        }
    }

    bool LoadableRegistry::isLoaded(NamespacedId id) {
        RegistryEntry* currentEntry = getSpecificEntry(id);
        if (currentEntry != nullptr) {
            return currentEntry->numLoaders > 0;
        } else {
            return false;
        }
    }

    RegistryEntry* LoadableRegistry::getSpecificEntry(NamespacedId id) {
        RegistryEntry* currentEntry = entries[NamespacedIdHash()(id) % numBuckets];
        while (currentEntry != nullptr) {
            if (currentEntry->id == id) {
                return currentEntry;
            } else {
                currentEntry = currentEntry->next;
            }
        }
        return nullptr;
    }

    void LoadableRegistry::reset() {
        for(int i = 0; i < numBuckets; i++) {
            for(RegistryEntry* j = entries[i]; j != nullptr; j = j->next) {
                destroyEntryInternal(j->item);
            }
            delete entries[i];
            entries[i] = nullptr;
        }
    }

    GeneralizedLoadableRegistry::GeneralizedLoadableRegistry(unsigned int numBuckets, LoaderFunction loaderFunction, UnloaderFunction unloaderFunction): LoadableRegistry(numBuckets), loaderFunction(loaderFunction), unloaderFunction(unloaderFunction) {}

    GeneralizedLoadableRegistry::~GeneralizedLoadableRegistry() {}

    void GeneralizedLoadableRegistry::addEntry(NamespacedId id) {
        addEntryInternal(id, nullptr);
    }

    void GeneralizedLoadableRegistry::loadEntryInternal(NamespacedId id, void** item) {
        *item = loaderFunction(id);
    }

    void GeneralizedLoadableRegistry::unloadEntryInternal(void* item) {
        unloaderFunction(item);
    }

    void GeneralizedLoadableRegistry::destroyEntryInternal(void* item) {
        unloaderFunction(item);
    }

    LoadableItem::LoadableItem() {}

    LoadableItem::~LoadableItem() {}

    IndividualizedLoadableRegistry::IndividualizedLoadableRegistry(unsigned int numBuckets): LoadableRegistry(numBuckets) {}

    IndividualizedLoadableRegistry::~IndividualizedLoadableRegistry() {}

    void IndividualizedLoadableRegistry::addEntry(NamespacedId id, LoadableItem* item) {
        addEntryInternal(id, item);
    }

    void IndividualizedLoadableRegistry::loadEntryInternal(NamespacedId id, void** item) {
        ((LoadableItem*)(*item))->load();
    }

    void IndividualizedLoadableRegistry::unloadEntryInternal(void* item) {
        ((LoadableItem*)(item))->unload();
    }

    void IndividualizedLoadableRegistry::destroyEntryInternal(void* item) {
        delete ((LoadableItem*)(item));
    }

}