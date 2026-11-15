// Robert Jackson
// CSCI 232 Fall 2026
// Programming Assignment #7
// I declare that I am the author of this work, take full responsibility for it, and have disclosed any material external assistance.
// I interactively used Claude (Anthropic) to work on this assignment to explain the C++ concepts behind each one.
// I reviewed, tested, and verified the submitted implementation myself.

// ============================================================
// CSCI 232 Assignment 07 – Shapes, Polymorphism and Insertion Sort
// Student Implementation
// ============================================================

#include "code.hpp"
#include <iostream>
#include <iterator>
#include <numbers>

std::string AUTHOR_NAME       = "Robert Jackson";
std::string AUTHOR_AUTHORSHIP = "I acknowledge that I have worked on this "
"assignment independently, except where explicitly noted and referenced. "
"Any collaboration or use of external resources has been properly cited. "
"I am fully aware of the consequences of academic dishonesty and agree "
"to abide by the university's academic integrity policy.";

// ============================================================
// PART 1: Shape, Circle, Square, Rectangle
// ============================================================

/// True if THIS shape has a strictly smaller area() than other.
bool Shape::operator<(const Shape &other) const
{
    // area() is virtual, so each side runs the formula of its real class.
    // Strict <, never <=: equal areas are not "less" (the stable sort needs this).
    return area() < other.area();
}

/// Constructor with a member initializer list.
Circle::Circle(double newRadius)
    : radius(newRadius)
{
}

/// pi * radius * radius, use std::numbers::pi
double Circle::area() const
{
    return std::numbers::pi * radius * radius;
}

/// Return exactly "Circle".
std::string Circle::name() const
{
    return "Circle";
}

Square::Square(double newSide)
    : side(newSide)
{
}

/// side * side
double Square::area() const
{
    return side * side;
}

/// Return exactly "Square".
std::string Square::name() const
{
    return "Square";
}

Rectangle::Rectangle(double newWidth, double newHeight)
    : width(newWidth), height(newHeight)
{
}

/// width * height
double Rectangle::area() const
{
    return width * height;
}

/// Return exactly "Rectangle".
std::string Rectangle::name() const
{
    return "Rectangle";
}

// ============================================================
// PART 2: ShapeList
// ============================================================

/// RAII: delete EVERY shape, then clear the list.
ShapeList::~ShapeList()
{
    // The list only stores pointers; clear() alone would leak every shape.
    for (Shape *shapePtr : data)
    {
        delete shapePtr;                // virtual destructor: the whole derived object goes
    }
    data.clear();
}

/// Append at the end. Return -1 if newShapePtr is nullptr, 0 on success.
int ShapeList::addShape(Shape *newShapePtr)
{
    if (newShapePtr == nullptr)
    {
        return -1;                      // nothing to own, list unchanged
    }
    data.push_back(newShapePtr);
    return 0;
}

/// Delete the first shape and remove it from the list.
/// Return false if the list is empty, true otherwise.
bool ShapeList::removeFirst()
{
    if (data.empty())
    {
        return false;
    }
    delete data.front();                // delete the shape first...
    data.pop_front();                   // ...then drop the (now dangling) pointer
    return true;
}

/// Number of shapes in the list.
int ShapeList::length() const
{
    return static_cast<int>(data.size());
}

/// Sum of area() of all shapes (0.0 if empty).
double ShapeList::totalArea() const
{
    double sum = 0.0;
    for (const Shape *shapePtr : data)
    {
        sum += shapePtr->area();        // virtual call: no formula in ShapeList
    }
    return sum;
}

/// One line per shape: name, one space, area. Return the number of shapes.
int ShapeList::printList() const
{
    int printed = 0;
    for (const Shape *shapePtr : data)
    {
        std::cout << shapePtr->name() << " " << shapePtr->area() << std::endl;
        printed++;
    }
    return printed;
}

/// area() of the shape at index, or std::nullopt if index is out of range.
/// Hint: std::next(data.begin(), index)
std::optional<double> ShapeList::areaAt(int index) const
{
    if (index < 0 || index >= length())
    {
        return std::nullopt;
    }
    // std::list has no operator[]: walk an iterator index steps from the front.
    // The iterator points to a Shape*, so one * gives the pointer.
    const Shape *shapePtr = *std::next(data.begin(), index);
    return shapePtr->area();
}

/// name() of the shape at index, or std::nullopt if index is out of range.
std::optional<std::string> ShapeList::nameAt(int index) const
{
    if (index < 0 || index >= length())
    {
        return std::nullopt;
    }
    const Shape *shapePtr = *std::next(data.begin(), index);
    return shapePtr->name();
}

/// Your own STABLE insertion sort, smallest area first.
/// Compare shapes with operator< (dereference the pointers!).
/// Hint: for every element, walk left while the previous shape is bigger,
/// then move the element there with data.splice(...).
void ShapeList::insertionSort()
{
    if (data.size() < 2)
    {
        return;                         // empty and one-element lists are already sorted
    }

    // Everything left of currentIt is the sorted part. Start at the second element.
    auto currentIt = std::next(data.begin());
    while (currentIt != data.end())
    {
        // Remember where the next round starts BEFORE moving the current node.
        auto nextIt = std::next(currentIt);

        // Walk gapIt left while the shape before it is strictly bigger than the
        // current one. **it is the Shape itself, so operator< compares areas,
        // not addresses. Strict "bigger" keeps equal areas in order (stable).
        auto gapIt = currentIt;
        while (gapIt != data.begin() && **currentIt < **std::prev(gapIt))
        {
            --gapIt;
        }

        // Relink the node in front of gapIt: Theta(1), no shape is copied.
        if (gapIt != currentIt)
        {
            data.splice(gapIt, data, currentIt);
        }

        currentIt = nextIt;
    }
}

/// True if no shape is smaller than the one before it.
bool ShapeList::isSorted() const
{
    if (data.size() < 2)
    {
        return true;
    }

    auto previousIt = data.begin();
    for (auto currentIt = std::next(previousIt); currentIt != data.end(); ++currentIt)
    {
        if (**currentIt < **previousIt)
        {
            return false;               // a smaller shape after a bigger one
        }
        previousIt = currentIt;
    }
    return true;
}
