//
//  main.cpp
//  Lenses
//
//  Created by Jonathan Sterling on 7/29/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "lens.hpp"
#include <functional>

struct Toy
{
  std::string name;
};

struct Dog
{
  std::string name;
  Toy toy;
};

struct Person
{
  std::string name;
  Dog dog;
  
  friend std::ostream& operator<< (std::ostream& o, Person const& person);
};

std::ostream& operator<< (std::ostream& o, Person const& person)
{
  return o << person.name << " has a dog named " << person.dog.name << " whose favorite toy is " << person.dog.toy.name << ".";
}


int main (int argc, const char * argv[])
{
  typedef lens<Person,Dog,&Person::dog>       person_dog;
  typedef lens<Dog,Toy,&Dog::toy>             dog_toy;
  typedef lens<Toy,std::string,&Toy::name>    toy_name;
  typedef lens_comp<person_dog, dog_toy>      person_dog_toy;
  typedef lens_comp<person_dog_toy, toy_name> person_dog_toy_name;
  
  const Dog tucker = { "tucky", { "squeaky" } };
  const Person jon = { "jon", tucker };
  const Person jon2 = person_dog_toy_name(jon).set("fuzzy");
  
  std::cout << "rev 1: " << jon << std::endl;
  std::cout << "rev 2: " << jon2 << std::endl;
  
  return 0;
}

