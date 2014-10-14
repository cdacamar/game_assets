#include <iostream>

#include <entity_handlers/entity_manager.hpp>
#include <mpl/has_member.hpp>

struct foo {
  int i = 10;
  int id = 0;
  
  void update() { i += 20; }
  void draw(std::ostream& dc) const { dc<<"foo: "<<id<<"value: "<<i<<'\n'; }
  bool is_alive() const {
    if (i > 100) std::cout<<"foo: "<<id<<" died! at:"<<i<<'\n';
    return i < 100;
  }
};

struct bar {
  std::string s = "I'M AN OBJECT!";
  bar() = default;
  bar(std::string str): s{std::move(str)} { }

  void update() { }
  void draw(std::ostream& dc) const { dc<<s<<'\n'; }
  bool is_alive() const { return false; } // die instantly
};


struct foobar {
  void update() { }
  void draw(std::ostream& dc){ dc<<"foobar!\n"; }
  bool is_alive() const { return true; }

  template <typename T>
  void draw(T&) const { }
};

int main() {
  game_utils::entity_manager<foo, bar, foobar> mgr;
  mgr.create<foo>();
  mgr.create<foo>();
  mgr.get_all<foo>().back().i = 50; // set the last one
  mgr.get_all<foo>().back().id = 1; // set the last one
  mgr.create<bar>();
  mgr.create<bar>("something awesome");
  mgr.create<foobar>();
  mgr.for_each_in_group<foo>(
    [](auto& e) {
      std::cout<<std::boolalpha<<e.is_alive()<<'\n';
    }
  );

  // emulate 20 frames
  for (int i = 0;i != 20;++i) {
    mgr.update();
    mgr.draw(std::cout);
    mgr.refresh();
  }
}
