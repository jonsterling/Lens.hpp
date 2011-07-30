//
//  lens.hpp
//  Lenses
//
//  Created by Jonathan Sterling on 7/29/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Lenses_lens_hpp
#define Lenses_lens_hpp

template <class T, class R, R T::*member>
class lens
{
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
class lens_comp
{
  const typename OutsideLens::object_type & t;
  
public:
  typedef typename OutsideLens::object_type object_type;
  
  lens_comp(const typename OutsideLens::object_type& t_) : t(t_) {}
  
  typename InsideLens::member_type get()
  {
    auto _this = OutsideLens(t);
    auto get_this = _this.get();
    auto get_that = InsideLens(get_this);
    return get_that.get();
  }
  
  const typename OutsideLens::object_type set(typename InsideLens::member_type a)
  {
    auto _this = OutsideLens(t);
    
    auto get_this = _this.get();
    auto mod_that = InsideLens(get_this).set(a);
    return _this.set(mod_that);
  }
};

template <class OutsideLens, class InsideLens, class CenterLens>
using lens_comp3 = lens_comp<lens_comp<OutsideLens, InsideLens>, CenterLens>;

#endif
