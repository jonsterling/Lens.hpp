//
//  main.cpp
//  Lenses
//
//  Created by Jonathan Sterling on 7/29/11.
//  Copyright 2011 Jonathan Sterling. All rights reserved.
//

#include <iostream>
#include "lens.hpp"
#include "demo_types.hpp"
#include "show.hpp"
#include <functional>
#include <algorithm>

int main (int argc, const char * argv[])
{
  typedef lens_comp<Person::dog, Dog::toy> person_dog_toy;
  typedef lens_comp<Person::dog, Dog::toy, Toy::name> person_dog_toy_name;
  
  const Dog tucker("tucky", Toy("squeaky"));
  const Person jon("jon", tucker);
  const Person jon2 = person_dog_toy_name(jon).set("fuzzy");
  
  std::cout << "rev 1: " << jon.show() << std::endl;
  std::cout << "rev 2: " << jon2.show() << std::endl;
  
  return 0;
}

