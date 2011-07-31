I'm going to assume that you know what Lenses are. In the future, I may
define them here, just too lazy at the moment. The point of this readme
is to explain how I implemented Lenses in C++.

## How to generalize member-access

The first problem to solve was generalizing member-access in a
statically composable manner. C++ allows you to declare a *pointer to a
member* of a class, which becomes extremely useful. If the following
code declares a pointer `dog_toy` to the a `Dog`'s `toy`:

~~~~cpp
Toy Dog::*dog_toy = &Dog::toy;
~~~~

Then the type of `dog_toy` is `Toy Dog::*`. The other key bit of
information is that a non-type template parameter can be a pointer to a
member. Hence, the following will generalize a structure over a
member pointer `member_ptr`:

~~~~cpp
template <class A, class B, A B::*member_ptr>
~~~~


