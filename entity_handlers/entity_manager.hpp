#pragma once

#include <algorithm>
#include <vector>
#include <tuple>

#include <mpl/has_member.hpp>
#include <mpl/tuple_utilities.hpp>

GENERATE_HAS_MEMBER_FN(update)
GENERATE_HAS_MEMBER_FN(is_alive)
GENERATE_HAS_MEMBER_FN(draw)

namespace game_utils
{

template <typename... EntityTypes>
class entity_manager {
public:
  using entity_lists_t = std::tuple<std::vector<EntityTypes>...>;

  template <typename EntityType, typename... EntityArgs>
  void create(EntityArgs&&... entity_args) {
    static_assert( mpl::tuple_contains<std::vector<EntityType>, entity_lists_t>::value, "entity must be part of the types entity_manager manages" );

    // instantiate the entity
    std::get<std::vector<EntityType>>(grouped_entities_).emplace_back(EntityType(std::forward<EntityArgs>(entity_args)...));
  }

  void refresh() {
    static_assert( mpl::and_bools<mpl::has_member_fn_is_alive<const EntityTypes, bool()>::value...>::value,
        "all entities must have bool (E::is_alive)()const" );
    mpl::for_each_tuple_element(grouped_entities_,
        [](auto& entity_list) {
          entity_list.erase(std::remove_if(
              std::begin(entity_list), std::end(entity_list),
              [](auto& e)->bool{ return !e.is_alive(); }
            ),
            std::end(entity_list)
          );
        }
    );
  }

  void clear() {
    mpl::for_each_tuple_element(grouped_entities_,
        [](auto& entity_list) { entity_list.clear(); }
    );
  }

  template <typename EntityType>
  auto& get_all() {
    static_assert( mpl::tuple_contains<std::vector<EntityType>, entity_lists_t>::value,
        "entity must be part of the types entity_manager manages" );
    return std::get<std::vector<EntityType>>(grouped_entities_);
  }

  void update() {
    static_assert( mpl::and_bools<mpl::has_member_fn_update<EntityTypes, void()>::value...>::value,
        "all entities must have void (E::update)()" );
    mpl::for_each_tuple_element(grouped_entities_,
        [](auto& entity_list) {
          for (auto& e : entity_list) e.update();
        }
    );
  }

  template <typename Context>
  void draw(Context& dc) const {
    static_assert( mpl::and_bools<mpl::has_member_fn_draw<const EntityTypes, void(Context&)>::value...>::value,
        "entities must have either: void (E::draw)(Context&) const OR template <typename T> void (E::draw)(Context&) const" );
    mpl::for_each_tuple_element(grouped_entities_,
        [&](auto& entity_list)mutable{
          for (auto& e : entity_list) e.draw(dc);
        }
    );
  }

  template <typename EntityType, typename Func>
  void for_each_in_group(Func&& f) {
    for (auto& e : get_all<EntityType>()) f(e);
  }

private:
  static_assert( mpl::all_unique_tuple<entity_lists_t>::value, "entity_lists_t must be all unique entity types" );
  entity_lists_t grouped_entities_;
};

}
