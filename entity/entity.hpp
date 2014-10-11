// defines a basic type erased entity interface
#pragma once

#include <memory>
#include <type_traits>

namespace game_utils
{

class entity_t {
public:
  template <typename T>
  entity_t(T&& obj): self_{std::make_unique<model_t<T>>(std::forward<T>(obj))} { }
  // default copy/move operations

  friend void draw(const entity_t& e);
  friend void update(entity_t& e);

  // observing functions
  friend bool is_alive(const entity_t& e);

  // getters
  template <typename T>
  friend const T& entity_cast(const entity_t& e);
  template <typename T>
  friend T& entity_cast(entity_t& e);

private:
  struct concept_t {
    virtual ~concept_t() = default;

    virtual void draw() const     = 0;
    virtual void update()         = 0;
    virtual bool is_alive() const = 0;
  };
  template <typename T>
  struct model_t : concept_t {
    model_t(T obj): entity_{std::move(obj)} { }
    void draw() const override {
      entity_.draw();
    }
    void update() override {
      entity_.update();
    }
    bool is_alive() const override {
      return entity_.is_alive();
    }

    T entity_;
  };
  std::unique_ptr<concept_t> self_; // make a non-const unique_ptr so we can modify the contents
                                    // ie. not thread safe, need perhaps a different model or representation...
};

template <typename T>
const T& entity_cast(const entity_t& e) { return static_cast<const entity_t::model_t<T>*>(e.self_.get())->entity_; }

template <typename T>
T& entity_cast(entity_t& e) { return const_cast<T&>(entity_cast<T>(const_cast<const entity_t&>(e))); }

void draw(const entity_t& e)     { e.self_->draw(); }
void update(entity_t& e)         { e.self_->update(); }
bool is_alive(const entity_t& e) { return e.self_->is_alive(); }

}
