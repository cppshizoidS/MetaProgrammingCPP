#include "path.hpp"

int main()
{
#define static_assert(...) static_assert(__VA_ARGS__, "")

    static_assert(map::getFromPos<Vec<6, 0>> == 0);
    static_assert(map::getFromPos<Vec<3, 4>> == 1);

    static_assert(map::get<0> == 'S');
    static_assert(map::getFromPos<Vec<0, 0>> == 'S');
    static_assert(map::getFromPos<Vec<2, 3>> == 'E');

    static_assert(map::find<'S'> == 0); // Start
    static_assert(map::find<'E'> == 3*8 + 2); // End

    static_assert(Vec<1, 2>::Index<8> == 17);
    static_assert(std::is_same<FromIndex<8, 17>, Vec<1, 2>>::value);
    static_assert(FromIndex<8, 17>::Index<8> == 17);

    using TestVec1 = VectorFromValue<int, 0, 1, 2, 3, 4, 5>;
    using TestVec2 = TestVec1::PushFront<6>;

    static_assert(TestVec1::Get<0>::Value == 0);
    static_assert(TestVec1::Get<1>::Value == 1);
    static_assert(TestVec1::Get<5>::Value == 5);
    static_assert(TestVec1::remove<2>::get<3> == 4);

    static_assert(TestVec2::Get<0>::Value == 6);
    static_assert(TestVec2::PopFront::value == 6);
    static_assert(TestVec2::PopFront::vector::Size == TestVec1::Size);

    using TestVec3 = Vector<Vec<0, 0>, Vec<1, 1>, Vec<2, 2>>;

    static_assert(std::is_same<TestVec3::get<0>, Vec<0, 0>>::value);
    static_assert(std::is_same<TestVec3::get<1>, Vec<1, 1>>::value);
    static_assert(std::is_same<TestVec3::get<2>, Vec<2, 2>>::value);
    static_assert(std::is_same<TestVec3::get<0>, Vec<2, 2>>::value == false);

    using TestVector4 = Vector<Test<0>, Test<-1>, Test<1>, Test<8>, Test<-1>>;
    using LeastNode = FindLeast<TestVector4>;
    static_assert(LeastNode::Index == 1);
    static_assert(LeastNode::Type::Cost == -1);

    using TestVector5 = Vector<Test<0>, Test<-1>, Test<1>, Test<8>, Test<-1>>;
    using LeastNode2 = FindLeast<TestVector5, Test<9>, -1, FalseCondition>;

    static_assert(LeastNode2::Index == -1);
    static_assert(std::is_same<LeastNode2::Type, Test<9>>::value);

    using astar = AStar<map>;

    using Node1 = typename astar::StartNode;
    using Node2 = Node<Vec<7, 7>::Index<8>, 0, 0>;
    using Node3 = Node<Vec<0, 1>::Index<8>, 0, 0>;
    using Node4 = Node<Vec<1, 1>::Index<8>, 0, 0>;

    static_assert(astar::template BuildNeighbors<Node1>::Type::Size == 2);
    static_assert(astar::template BuildNeighbors<Node2>::Type::Size == 2);
    static_assert(astar::template BuildNeighbors<Node3>::Type::Size == 3);
    static_assert(astar::template BuildNeighbors<Node4>::Type::Size == 4);;
}