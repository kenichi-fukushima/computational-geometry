#pragma once

template<class Container, class Key>
bool ContainsKey(const Container& c, const Key& k) {
  return c.find(k) != c.end();
}

template<class Container, class Key>
void RemoveKey(Container* c, const Key& k) {
  auto iter = c->find(k);
  if (iter != c->end()) {
    c->erase(iter);
  }
}

template<template<typename...> class Map,
         typename Key,
         typename Value,
         typename ... MapParams>
const Value& FindOrDie(const Map<Key, Value, MapParams...>& m, const Key& k) {
  auto iter = m.find(k);
  return iter->second;
}         
