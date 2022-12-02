#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct Vector2 Vector2;

typedef struct MinMax
{
    float   min;
    float   max;
} MinMax;

/// @brief get a vector from two points 
/// @param point1 The origin of the vector
/// @param point2 The target vector
/// @return The vector going from point1 to point2
Vector2 GetVector(Vector2 point1, Vector2 point2);

/// @brief get the normal of the vector
/// @param vector The vector we want the normal of
/// @return The left normal of the given vector
Vector2 GetLeftNormal(Vector2 vector);

/// @brief get the Min and Max of the vectors
/// @param points The vectors to check
/// @param pointsCount The number of points to check
/// @param normal the normal of the vector
/// @return The min and max values
MinMax GetMinMax(Vector2 const* points, size_t pointsCount, Vector2 normal);

/// @brief Merge two arrays into a new one
/// @param array1 The first array to merge
/// @param array1Size The number of elements in the first array
/// @param array2 The second array to merge
/// @param array2Size The number of elements in the second array
/// @return A new array containing the elements of both arrays
Vector2* MergeVector2Arrays(Vector2 const* array1, size_t array1Size,
    Vector2 const* array2, size_t array2Size);

/// @brief Check if two polygons are colliding 
/// @param poly1Pts the points of the first polygon
/// @param poly1PtsCount the number of points in the first polygon
/// @param poly2Pts the points of the second polygon
/// @param poly2PtsCount the number of points in the second polygon
/// @return true when the polygons are colliding, false otherwise
bool CheckCollisionPolygons(Vector2 const* poly1Pts, size_t poly1PtsCount,
    Vector2 const* poly2Pts, size_t poly2PtsCount);

/// @brief Check if a polygon and a circle are colliding 
/// @param circleCenter the center of the 1st circle
/// @param circleRadius the radius of the 1st circle
/// @param polyPts the points of the polygon
/// @param polyPtsCount the number of points in the pollygon
/// @return a boolean true when the circles are colliding
bool CheckCollisionCirclePolygon(Vector2 circleCenter, float circleRadius,
    Vector2 const* polyPts, size_t polyPtsCount);

/// @brief Check if two circles are colliding
/// @param c1Center the center of the 1st circle  
/// @param c1Radius the radius of the 1st circle
/// @param c2Center the center of the 2nd circle
/// @param c2Radius the radius of the 2nd circle
/// @return a boolean true when the circles are colliding
bool CheckColCircles(Vector2 c1Center, float c1Radius, Vector2 c2Center,
    float c2Radius);