I'm going to assume that you know what Lenses are. In the future, I may
define them here, just too lazy at the moment. The point of this readme
is to explain how I implemented Lenses in C++.

## How to generalize member-access

The first problem to solve was generalizing member-access in a
statically composable manner. C++ allows you to declare a *pointer to a
member* of a class, which becomes extremely useful. If the following
code declares a pointer `dog_toy` to the a `Dog`'s `toy`:

````cpp
Toy Dog::*dog_toy = &Dog::toy;
````

Then the type of `dog_toy` is `Toy Dog::*`. The other key bit of
information is that a non-type template parameter can be a pointer to a
member. Hence, the following will generalize a structure over a
member pointer `member_ptr`:

````cpp
template <class A, class B, A B::*member_ptr>
````


## Using Lenses

The most basic, raw way to use lenses in your classes is to provide a
typedef to the correct instantiation of the `lens` template. Later, you
can make new lenses by composing existing ones, or by mapping over them.
This could look like:

~~~~cpp
class Toy
{
	std::string _name;
  typedef lens<Toy,std::string,&Toy::_name> name;

public:
  Toy(std::string n) : _name(n) {}

  const std::string& get_name() const
  {
    return Toy::name(*this).get();
  }

  const Toy set_name(const std::string& n)
  {
    return Toy::name(*this).set(n);
  }
};


class Dog
{
  std::string _name;
  Toy _toy;
  typedef lens<Dog,std::string,&Dog::_name> name;
	typedef lens<Dog,Toy,&Dog::_toy> toy;

public:
  Dog(std::string n, Toy t) : _name(n), _toy(t) {}
  
  const std::string& get_name() const
  {
    return Dog::name(*this).get();
  }

  const Dog set_name(const std::string& n)
  {
    return Dog::name(*this).set(n);
  }

  const Toy& get_toy() const
  {
    return Dog::toy(*this).get();
  }

  const Dog set_toy(const Toy& t) const
  {
    return Dog::toy(*this).set(t);
  }
};

~~~~

This is rather too irritating to do frequently, though. With the help of
the C preprocessor, however, we can automate this nicely:


~~~~cpp
class Toy
{
  std::string val(name);

public:
  Toy(std::string n) : _name(n) {}  
};

class Dog
{
  std::string val(name);
  Toy val(toy);

public:
  Dog(std::string n, Toy t) : _name(n), _toy(t) {}
}
~~~~
