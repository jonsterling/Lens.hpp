I'm going to assume that you know what Lenses are. In the meanwhile, check out 
[these slides](http://twanvl.nl/files/lenses-talk-2011-05-17.pdf) for some 
basic theory on Lenses. The point of this readme is to explain how I 
implemented Lenses in C++.

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
can make new lenses by composing existing ones. This could look like:

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

  const Toy set_name(const std::string& n) const
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

  const Dog set_name(const std::string& n) const
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

The hiccup here, though, is that we need to be able to determine the
type of the current class in a general manner. Preferably, C++ would
have a type keyword `This` or `Self`, which would point to the current
class, but that's something we need to implement for ourselves. The way
to do this is define a small utility class, `Derivable<T>`:

~~~~cpp
template <class T> class Derivable
{
  typedef T Self;
};
~~~~

Thus, we don't need to pass `Toy` into the `val` macro, since it assumes
that the class inherits the `Derivable` trait.

###Composition

Lenses are composed by using the `lens_comp` veriadic template. For instance:

~~~~cpp
typedef lens_comp<Person::dog,Dog::toy,Toy::name> person_dog_toy_name;

const Person jon("jon", Dog("tucker", Toy("squeaky")));
const std::string str = person_dog_toy_name(jon).get(); // => "squeaky"

const Person jon2 = person_dog_toy_name(jon).set("fuzzy");
// Now, jon2 is a copy of jon, but with his the name of his Dog's Toy changed.
~~~~

This is super excellent for dealing with deep structures.

##Deriving Show

In Haskell, we can allow any datatype to be printed by deriving the
`Show` type class automatically. This looks like:

~~~~haskell
data Toy = Toy String deriving Show
data Dog = Dog String Toy deriving Show
~~~~

This seemed like an important thing to have in C++, so I've added a
similar (static) facility for doing this, using a variadic template over
lenses.

~~~~cpp
template< class T // The class of the object being "shown"
        , class ...Lenses // The object's lenses, in order of desired appearance
        >
struct Show;
~~~

This can be used in a class as follows:

~~~~cpp
class Dog
{
  std::string val(name);
  Toy val(toy);
  
  typedef Show<Dog,name,toy> show_t;

public:
  Dog(std::string n, Toy t) : _name(n), _toy(t) {}

  std::string show() const
  {
    return show_t(*this).show();
  }
};

const Toy t("squeaky");
const Dog d("Tucker", t);
std::cout << d.show() << std::endl; // => "Dog (Tucker, squeaky)"
~~~~

Of course, a macro `derive_show` has also been provided, which
simpifies derivation by a lot:

~~~~cpp
class Dog
{
  std::string val(name);
  Toy val(toy);

public:
  Dog(std::string n, Toy t) : _name(n), _toy(t) {}

  derive_show(name,toy);
};
~~~~
