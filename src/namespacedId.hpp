/******************************************************************************
* File: namespacedId.hpp
* Author: Tony Skeps
* Date: July 1, 2022
******************************************************************************/

#include <string>

#ifndef NAMESPACED_ID_HPP
#define NAMESPACED_ID_HPP

namespace Game {

    struct NamespacedId {
        std::string namespaceName;
        std::string id;

        NamespacedId(std::string namespaceName, std::string id);
        NamespacedId(std::string id);
        NamespacedId();
    };

    bool operator== (const NamespacedId lhs, const NamespacedId rhs);
    bool operator!= (const NamespacedId lhs, const NamespacedId rhs);

    struct NamespacedIdHash {
        size_t operator() (const NamespacedId&) const;
    };

}

#endif
