//
//  demo_types.hpp
//  Lenses
//
//  Created by Jonathan Sterling on 7/31/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef Lenses_demo_types_hpp
#define Lenses_demo_types_hpp

#include "lens.hpp"
#include "show.hpp"

class Toy : public Derivable<Toy>
{
  std::string val(name);
  
public:
  Toy(std::string n) : _name(n) {}
  derive_show(name);
};

class Dog : public Derivable<Dog>
{
  std::string val(name);
  Toy val(toy);
  
public:
  Dog(std::string n, Toy t) : _name(n), _toy(t) {}
  derive_show(name, toy);
};

class Person : public Derivable<Person>
{
  std::string val(name);
  Dog val(dog);
  
public:
  Person(std::string n, Dog d) : _name(n), _dog(d) {}
  derive_show(name, dog);
};

#endif
