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

struct make_uppercase : std::unary_function<std::string, std::string>
{
  std::string operator ()(const std::string input)
  {
    std::string string = input;
    std::transform(string.begin(), string.end(), string.begin(), toupper);
    return string;
  };
};

int main (int argc, const char * argv[])
{
  typedef lens_comp<Person::dog, Dog::toy>     person_dog_toy;
  typedef lens_comp<Person::dog, Dog::toy, Toy::name> person_dog_toy_name;
  typedef lens_map<person_dog_toy_name,make_uppercase> transform_toy;
  
  const Dog tucker("tucky", Toy("squeaky"));
  const Person jon("jon", tucker);
  const Person jon2 = person_dog_toy_name(jon).set("fuzzy");
  const Person jon3 = transform_toy(jon).set("asdfasdf");
  
  std::cout << "rev 1: " << jon.show() << std::endl;
  std::cout << "rev 2: " << jon2.show() << std::endl;
  std::cout << "rev 3: " << jon3.show() << std::endl;
  std::cout << tucker.show() << std::endl;
  
  return 0;
}

