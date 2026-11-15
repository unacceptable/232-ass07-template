// Robert Jackson
// CSCI 232 Fall 2026
// Programming Assignment #7
// I declare that I am the author of this work, take full responsibility for it, and have disclosed any material external assistance.
// I used Claude (Anthropic) to write the tests and to explain the C++ concepts behind each one.
// I reviewed, tested, and verified the submitted implementation myself.

#ifndef UNITY_H
#define UNITY_H
#include "unity.h"
#endif

#include "code.hpp"
#include <cmath>
#include <iostream>
#include <numbers>

// ============================================================
// PART 1 Tests: Shapes
// ============================================================

/// Create Circle(1), Square(2), Rectangle(2, 3).
/// Verify area() is pi, 4 and 6 (compare doubles with std::fabs(a - b) < 1e-9).
void test_shape_areas(void)
{
    Circle circle(1);
    Square square(2);
    Rectangle rectangle(2, 3);

    TEST_ASSERT_TRUE_MESSAGE(std::fabs(circle.area() - std::numbers::pi) < 1e-9, "Circle(1) area should be pi");
    TEST_ASSERT_TRUE_MESSAGE(std::fabs(square.area() - 4.0) < 1e-9, "Square(2) area should be 4");
    TEST_ASSERT_TRUE_MESSAGE(std::fabs(rectangle.area() - 6.0) < 1e-9, "Rectangle(2, 3) area should be 6");
}

/// Create an array of three Shape* (a Circle, a Square, a Rectangle) with new.
/// Verify name() through the BASE pointer gives "Circle", "Square", "Rectangle".
/// Delete the shapes through the base pointers.
void test_shape_names_through_base_pointer(void)
{
    Shape *shapes[3] = {new Circle(1), new Square(2), new Rectangle(2, 3)};

    TEST_ASSERT_TRUE_MESSAGE(shapes[0]->name() == "Circle", "base pointer to a Circle should say Circle");
    TEST_ASSERT_TRUE_MESSAGE(shapes[1]->name() == "Square", "base pointer to a Square should say Square");
    TEST_ASSERT_TRUE_MESSAGE(shapes[2]->name() == "Rectangle", "base pointer to a Rectangle should say Rectangle");

    // no ShapeList here, so this test owns the shapes and must delete them
    for (Shape *shapePtr : shapes)
    {
        delete shapePtr;
    }
}

/// Create Square(1) and Square(2). Take two const Shape& to them.
/// Verify small < big is true, big < small is false, small < small is false.
void test_shape_operator_less(void)
{
    Square smallSquare(1);
    Square bigSquare(2);
    const Shape &small = smallSquare;
    const Shape &big = bigSquare;

    TEST_ASSERT_TRUE_MESSAGE(small < big, "area 1 < area 4 should be true");
    TEST_ASSERT_FALSE_MESSAGE(big < small, "area 4 < area 1 should be false");
    TEST_ASSERT_FALSE_MESSAGE(small < small, "a shape is not less than itself");
}

/// Create Square(2) and Rectangle(1, 4): both have area 4.
/// Verify that NEITHER a < b NOR b < a is true.
void test_shape_equal_area_not_less(void)
{
    Square a(2);
    Rectangle b(1, 4);

    TEST_ASSERT_TRUE_MESSAGE(!(a < b), "equal areas: a < b must be false");
    TEST_ASSERT_TRUE_MESSAGE(!(b < a), "equal areas: b < a must be false");
}

// ============================================================
// PART 2 Tests: ShapeList
// ============================================================

/// Create a ShapeList. Add new Circle(1) and new Square(2) with addShape.
/// Verify both calls return 0 and length() is 2.
/// (Do NOT delete the shapes yourself: the list owns them.)
void test_list_add_and_length(void)
{
    ShapeList list;

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, list.addShape(new Circle(1)), "adding a Circle should return 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, list.addShape(new Square(2)), "adding a Square should return 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, list.length(), "two adds should give length 2");
}

/// Create a ShapeList. Call addShape(nullptr).
/// Verify it returns -1 and length() stays 0.
void test_list_add_null_guard(void)
{
    ShapeList list;

    TEST_ASSERT_EQUAL_INT_MESSAGE(-1, list.addShape(nullptr), "addShape(nullptr) should return -1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, list.length(), "a rejected nullptr should not change the length");
}

/// Add Rectangle(2, 5) (area 10), Circle(1) (area 3.14), Square(2) (area 4).
/// Call insertionSort(). Verify nameAt(0), nameAt(1), nameAt(2) are
/// "Circle", "Square", "Rectangle" and isSorted() is true.
void test_insertionSort_orders_by_area(void)
{
    ShapeList list;
    list.addShape(new Rectangle(2, 5));
    list.addShape(new Circle(1));
    list.addShape(new Square(2));

    list.insertionSort();

    TEST_ASSERT_TRUE_MESSAGE(list.nameAt(0) == "Circle", "smallest area (pi) should be first");
    TEST_ASSERT_TRUE_MESSAGE(list.nameAt(1) == "Square", "area 4 should be in the middle");
    TEST_ASSERT_TRUE_MESSAGE(list.nameAt(2) == "Rectangle", "largest area (10) should be last");
    TEST_ASSERT_TRUE_MESSAGE(list.isSorted(), "the list should report sorted after insertionSort");
}

/// Call insertionSort() on an EMPTY list: no crash, length() is 0.
/// Then add ONE shape and sort again: length() is 1 and nameAt(0) is unchanged.
void test_insertionSort_empty_and_single(void)
{
    ShapeList list;

    list.insertionSort();
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, list.length(), "sorting an empty list should leave it empty");

    list.addShape(new Square(3));
    list.insertionSort();
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, list.length(), "sorting one shape should keep length 1");
    TEST_ASSERT_TRUE_MESSAGE(list.nameAt(0) == "Square", "the single shape should be unchanged");
}
