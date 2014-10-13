#pragma once

#include <algorithm>
#include <vector>
#include <tuple>

#include <entity/entity.hpp>

#include <mpl/tuple_utilities.hpp>

namespace game_utils
{

template <typename... EntityTypes>
class entity_manager {
public:
  using entity_lists_t = std::tuple<std::vector<EntityTypes*>...>;

  template <typename EntityType, typename... EntityArgs>
  void create(EntityArgs&&... entity_args) {
    static_assert( mpl::tuple_contains<std::vector<EntityType*>, entity_lists_t>::value, "entity must be part of the types entity_manager manages" );

    // instantiate the entity
    entities_.emplace_back(EntityType{std::forward<EntityArgs>(entity_args)...});
    // register it in the grouped entities
    std::get<std::vector<EntityType*>>(grouped_entities_).emplace_back(&entity_cast<EntityType>(entities_.back()));
  }

  void refresh() {
    mpl::for_each_tuple_element(grouped_entities_,
        [](auto& entity_list) {
          entity_list.erase(std::remove_if(
              std::begin(entity_list), std::end(entity_list),
              [](auto& e)->bool{ return !e->is_alive(); }
            ),
            std::end(entity_list)
          );
        }
    );

    entities_.erase(std::remove_if(
       std::begin(entities_), std::end(entities_),
        [](auto& e)->bool{ return !game_utils::is_alive(e); }
      ),
      std::end(entities_)
    );
  }

  void clear() {
    mpl::for_each_tuple_element(grouped_entities_,
        [](auto& entity_list) { entity_list.clear(); }
    );
    entities_.clear();
  }

  template <typename EntityType>
  auto& get_all() {
    static_assert( mpl::tuple_contains<std::vector<EntityType*>, entity_lists_t>::value, "entity must be part of the types entity_manager manages" );
    return std::get<std::vector<EntityType*>>(grouped_entities_);
  }

  void update() {
    for (auto& e : entities_) game_utils::update(e);
  }

  void draw() const {
    for (auto& e : entities_) game_utils::draw(e);
  }

  template <typename EntityType, typename Func>
  void for_each_in_group(Func&& f) {
    for (auto& e : get_all<EntityType>()) f(*e);
  }

private:
  std::vector<entity_t> entities_;

  static_assert( mpl::all_unique_tuple<entity_lists_t>::value, "entity_lists_t must be all unique entity types" );
  entity_lists_t grouped_entities_;
};

}
