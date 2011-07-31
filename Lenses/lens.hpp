//
//  lens.hpp
//  Lenses
//
//  Created by Jonathan Sterling on 7/29/11.
//  Copyright 2011 Jonathan Sterling. All rights reserved.
//

#ifndef Lenses_lens_hpp
#define Lenses_lens_hpp

#include "derivable.hpp"

#define val(member) _##member;\
  public:\
    typedef lens<Self, decltype(_##member), &Self::_##member> member;\
    const decltype(_##member)& get_##member() const { return Self::member(*this).get(); }\
    const Self set_##member(const decltype(_##member)& a) const { return Self::member(*this).set(a); }\
  private:


template <class T, class R, R T::*member>
class lens
{
protected:
  const T& t;
  
public:
  typedef T object_type;
  typedef R member_type;
  
  lens(const T& t_) : t(t_) {}
  
  const R& get()
  {
    return t.*member;
  }
  
  const T set(const R& v)
  {
    T t_ = t;
    t_.*member = v;
    return t_;
  }
};


template <class... Lenses>
struct lens_comp;

template <class Outside, class Inside, class Center, class... Lenses>
struct lens_comp<Outside,Inside,Center,Lenses...> : public lens_comp<Outside,lens_comp<Inside,Center>,Lenses...>
{
protected:
  typedef lens_comp<Outside,lens_comp<Inside,Center>,Lenses...> super;
public:
  lens_comp(const typename Outside::object_type& t_) : super(t_) {}
};

template <class Outside, class Inside>
struct lens_comp<Outside, Inside> : public Outside
{
public:
  lens_comp(const typename Outside::object_type& t_) : Outside(t_) {}
  typedef typename Outside::object_type object_type;
  typedef typename Inside::member_type member_type;
  
  const typename Inside::member_type& get()
  {
    auto _this = Outside(this->t);
    auto get_this = _this.get();
    auto get_that = Inside(get_this);
    return get_that.get();
  }
  
  const typename Outside::object_type set(const typename Inside::member_type& a)
  {
    auto _this = Outside(this->t);
    
    auto get_this = _this.get();
    auto mod_that = Inside(get_this).set(a);
    return _this.set(mod_that);
  }
};

#endif
