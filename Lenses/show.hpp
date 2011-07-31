//
//  show.hpp
//  Lenses
//
//  Created by Jonathan Sterling on 7/31/11.
//  Copyright 2011 Jonathan Sterling. All rights reserved.
//

#ifndef Lenses_show_hpp
#define Lenses_show_hpp

#include <string>
#include <typeinfo>
#include <cxxabi.h>


#define derive_show(lenses...)\
  private: typedef Show<Self,lenses> show_t;\
  public: std::string show() const { return show_t(*this).show(); }


template<class T, class ...Lenses>
struct Show;

template<class T, class Head, class... Tail>
class Show<T, Head, Tail...> : private Show<T, Tail...>
{
  typedef Show<T,Tail...> super;
public:
  Show(const T& v) : Show<T,Tail...>(v) {}
  
  std::string show() const
  {
    std::string type_name = abi::__cxa_demangle(typeid(T).name(),0,0,0);
    auto tail = super::_show(super(this->value));
    auto head = super::_show(Head(this->value).get());
    return type_name + " (" + head + ", " + tail + ")";
  }
};

template <class T, class Head>
struct Show<T,Head>
{
  Show(const T& v) : value(v) {}
  
  std::string show() const
  {
    return _show(Head(value).get());
  }
  
protected:
  const T& value;

  template <class A>
  static std::string _show(const A& a)
  {
    return a.show();
  }
  
  static std::string _show(const std::string& a)
  {
    return a;
  };
  
};

#endif
