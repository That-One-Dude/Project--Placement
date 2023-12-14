/******************************************************************************
* File: namespacedId.cpp
* Author: Tony Skeps
* Date: July 1, 2022
******************************************************************************/

#include "namespacedId.hpp"

namespace Game {

    NamespacedId::NamespacedId(std::string namespaceName, std::string id): namespaceName(namespaceName), id(id) {}

    NamespacedId::NamespacedId(std::string id) {
        std::size_t pos = id.find("::");
        if (pos != std::string::npos) {
            this->namespaceName = id.substr(0, pos);
            this->id = id.substr(pos + 2);
        } else {
            this->namespaceName = "game";
            this->id = id;
        }
        
    }

    NamespacedId::NamespacedId() {}

    bool operator== (const NamespacedId lhs, const NamespacedId rhs) {
        return lhs.namespaceName == rhs.namespaceName && lhs.id == rhs.id;
    }

    bool operator!= (const NamespacedId lhs, const NamespacedId rhs) {
        return lhs.namespaceName != rhs.namespaceName || lhs.id != rhs.id;
    }

    size_t NamespacedIdHash::operator() (const NamespacedId& id) const {
        std::hash<std::string> str_hash;

        return str_hash(id.namespaceName) ^ (str_hash(id.id) << 2);
    }

}
