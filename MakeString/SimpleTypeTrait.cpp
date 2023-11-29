#include <iostream>
#include <set>
#include <vector>

#include "makeString.hpp"

struct A {
  std::string to_string() const { return "A"; }
};

struct B {
  int m_i = 0;
  std::string to_string() const { return "B{" + std::to_string(m_i) + "}"; }
};

struct C {
  std::string m_string;
  auto begin() const { return std::begin(m_string); }
  auto begin() { return std::begin(m_string); }
  auto end() const { return std::end(m_string); }
  auto end() { return std::end(m_string); }

  std::string to_string() const { return "C{\"" + m_string + "\"}"; }
};

int main() {
  A a;
  B b = {1};

  const std::vector<int> xs = {1, 2, 3};
  const std::set<float> ys = {4, 5, 6};
  const double zs[] = {7, 8, 9};

  std::cout << "a: " << makeString(a) << "; b: " << makeString(b)
            << "; pi: " << makeString(3.14) << std::endl
            << "xs: " << makeString(xs) << "; ys: " << makeString(ys)
            << "; zs: " << makeString(zs) << std::endl;

  std::cout << makeString("Hello, ") << makeString(std::string_view("world"))
            << makeString(std::string("!!!")) << std::endl;

  const std::string constHello = "const hello!";
  std::cout << makeString(constHello) << std::endl;

  std::cout << makeString("xs: ", xs, "; and float is: ", 3.14f) << std::endl;

  std::cout << makeString(C{"\t a container with its own to_string()"})
            << std::endl;
}
