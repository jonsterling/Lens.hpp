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

#define val(member)\
  _##member;\
  public:\
    typedef lens<Self, typeof(_##member), &Self::_##member> member;\
    typeof(_##member) get_##member() const { return Self::member(*this).get(); }\
    Self set_##member(typeof(_##member) a) const { return Self::member(*this).set(a); }\
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
  
  R get()
  {
    return t.*member;
  }
  
  const T set(R v)
  {
    T t_ = t;
    t_.*member = v;
    return t_;
  }
};


template <class OutsideLens, class InsideLens>
struct lens_comp : public OutsideLens
{
  lens_comp(const typename OutsideLens::object_type& t_) : OutsideLens(t_) {}
  
  typename InsideLens::member_type get()
  {
    auto _this = OutsideLens(this->t);
    auto get_this = _this.get();
    auto get_that = InsideLens(get_this);
    return get_that.get();
  }
  
  const typename OutsideLens::object_type set(typename InsideLens::member_type a)
  {
    auto _this = OutsideLens(this->t);
    
    auto get_this = _this.get();
    auto mod_that = InsideLens(get_this).set(a);
    return _this.set(mod_that);
  }
};

template <class Lens, class Transformer>
struct lens_map : public Lens
{
  lens_map(const typename Lens::object_type& t_) : Lens(t_) {}
  
  typename Transformer::result_type get()
  {
    return Transformer()(Lens(this->t).get());
  };
  
  typename Lens::object_type set(typename Transformer::argument_type a)
  {
    return Lens(this->t).set(Transformer()(a));
  }
};

#endif
