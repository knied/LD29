//
//  Components.h
//  GameFoundation
//
//  Created by Kristof Niederholtmeyer on 28.01.14.
//  Copyright (c) 2014 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GameFoundation__Components__
#define __GameFoundation__Components__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>
#include <map>

////////////////////////////////////////////////////////////////////////////////

// for internal use only!
namespace ComponentsInternal {
    template<typename Entity, typename Tuple, typename Set, size_t C>
    struct Remover {
        enum { N = std::tuple_size<Tuple>::value };
        static void remove(Tuple& tuple, Set sets[N], Entity const& entity) {
            Remover<Entity, Tuple, Set, C-1>::remove(tuple, sets, entity);
            std::get<C>(tuple).erase(entity);
            sets[C].erase(entity);
        }
    }; // Remover
    
    template<typename Entity, typename Tuple, typename Set>
    struct Remover<Entity, Tuple, Set, 0> {
        enum { N = std::tuple_size<Tuple>::value };
        static void remove(Tuple& tuple, Set sets[N], Entity const& entity) {
            std::get<0>(tuple).erase(entity);
            sets[0].erase(entity);
        }
    }; // Remover
    
    template<typename Tuple, typename Set, size_t C, size_t ...Cs>
    struct Intersecter {
        enum { N = std::tuple_size<Tuple>::value };
        static Set intersection(Set const sets[N]) {
            Set result;
            Set tmp = ComponentsInternal::Intersecter<Tuple, Set, Cs...>::intersection(sets);
            std::set_intersection(sets[C].begin(), sets[C].end(),
                                  tmp.begin(), tmp.end(),
                                  std::inserter(result, result.end()));
            return result;
        }
    }; // Intersecter
    
    template<typename Tuple, typename Set, size_t C>
    struct Intersecter<Tuple, Set, C> {
        enum { N = std::tuple_size<Tuple>::value };
        static Set intersection(Set const sets[N]) {
            return sets[C];
        }
    }; // Intersecter
} // ComponentsInternal

////////////////////////////////////////////////////////////////////////////////

template<typename Entity, typename ...Ts>
class Components {
    typedef std::tuple<std::map<Entity, Ts>...> Tuple;
    typedef std::set<Entity> Set;
    enum { N = std::tuple_size<Tuple>::value };
    
    Tuple _components;
    Set _sets[N];
    
    Entity _next_entity = 1;
    
public:
    // creates a new entity id
    Entity create_entity() {
        return _next_entity++;
    }
    
    // adds a component entry for the given entity if not yet present
    template<size_t C>
    typename std::tuple_element<C, Tuple>::type::mapped_type& add(Entity const& entity) {
        _sets[C].insert(entity);
        return std::get<C>(_components)[entity];
    }
    
    // returns true if the given entity has the component C
    template<size_t C>
    bool has(Entity const& entity) const {
        return std::get<C>(_components).find(entity) != std::get<C>(_components).end();
    }
    
    // throws an exception if the component is not present
    template<size_t C>
    typename std::tuple_element<C, Tuple>::type::mapped_type& get(Entity const& entity) {
        return std::get<C>(_components).at(entity);
    }
    template<size_t C>
    typename std::tuple_element<C, Tuple>::type::mapped_type const& get(Entity const& entity) const {
        return std::get<C>(_components).at(entity);
    }
    
    // remove component C from the given entity
    template<size_t C>
    void remove(Entity const& entity) {
        std::get<C>(_components).erase(entity);
    }
    // remove all components from the given entity
    void remove(Entity const& entity) {
        ComponentsInternal::Remover<Entity, Tuple, Set, N-1>::remove(_components, _sets, entity);
    }
    
    // returns a set of all entities having the component C
    template<size_t C>
    Set const& component() const {
        return _sets[C];
    }
    // returns a set of all entities that at least have the components C0 to CN
    template<size_t C0, size_t C1, size_t ...Cs>
    Set intersection() const {
        Set result;
        Set tmp = ComponentsInternal::Intersecter<Tuple, Set, C1, Cs...>::intersection(_sets);
        std::set_intersection(component<C0>().begin(), component<C0>().end(),
                              tmp.begin(), tmp.end(),
                              std::inserter(result, result.end()));
        return result;
    }
}; // Components

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__GameFoundation__Components__) */

////////////////////////////////////////////////////////////////////////////////
