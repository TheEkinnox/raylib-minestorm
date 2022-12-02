#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

#include "collide.h"


Vector2 GetVector(Vector2 point1, Vector2 point2)
{
    return (Vector2) { point2.x - point1.x, point2.y - point1.y };
}

Vector2 GetLeftNormal(Vector2 vector)
{
    return (Vector2) { -vector.y, vector.x };
}

MinMax GetMinMax(Vector2 const* points, size_t pointsCount, Vector2 normal)
{
    MinMax minMax = (MinMax) {0};

    if (points == NULL || pointsCount == 1)
        return minMax;

    for (size_t i = 0; i < pointsCount; i++)
    {
        float dotProduct = points[i].x * normal.x + points[i].y * normal.y;

        if (i == 0 || dotProduct < minMax.min)
            minMax.min = dotProduct;

        if (i == 0 || dotProduct > minMax.max)
            minMax.max = dotProduct;
    }

    return minMax;
}

Vector2* MergeVector2Arrays(Vector2 const* array1, size_t array1Size,
    Vector2 const* array2, size_t array2Size)
{
    if (array1 == NULL || array2 == NULL
        || array1Size == 0 || array2Size == 0)
        return NULL;

    size_t    totalSize = array1Size + array2Size;
    Vector2*  result = malloc(totalSize * sizeof(Vector2));

    for (size_t i = 0; i < totalSize; i++)
        result[i] = i < array1Size ? array1[i] : array2[i - array1Size];

    return result;
}

bool CheckCollisionPolygons(Vector2 const* poly1Pts, size_t poly1PtsCount,
    Vector2 const* poly2Pts, size_t poly2PtsCount)
{
    if (poly1Pts == NULL || poly1PtsCount == 0 || poly2Pts == NULL || poly2PtsCount == 0)
        return false;

    size_t    allPtsSize = poly1PtsCount + poly2PtsCount;
    Vector2*  allPts = MergeVector2Arrays(poly1Pts, poly1PtsCount, poly2Pts, poly2PtsCount);

    MinMax poly1MinMax;
    MinMax poly2MinMax;

    for (size_t i = 0; i < allPtsSize; i++)
    {
        size_t pt2Index;
        if (i < poly1PtsCount)
            pt2Index = (i + 1) % poly1PtsCount;
        else
            pt2Index = Wrap(i + 1, poly1PtsCount, allPtsSize);

        Vector2 normal = GetLeftNormal(GetVector(allPts[i], allPts[pt2Index]));

        poly1MinMax = GetMinMax(poly1Pts, poly1PtsCount, normal);
        poly2MinMax = GetMinMax(poly2Pts, poly2PtsCount, normal);

        if (poly1MinMax.max < poly2MinMax.min
            || poly1MinMax.min > poly2MinMax.max)
        {
            free(allPts);
            return false;
        }
    }

    free(allPts);

    return true;
}

bool CheckCollisionCirclePolygon(Vector2 circleCenter, float circleRadius,
    Vector2 const* polyPts, size_t polyPtsCount)
{
    if (polyPts == NULL || polyPtsCount == 0)
        return false;

    MinMax polyMinMax;

    for (size_t i = 0; i < polyPtsCount; i++)
    {
        size_t pt2Index;
        pt2Index = (i + 1) % polyPtsCount;

        Vector2 normal = GetLeftNormal(GetVector(polyPts[i], polyPts[pt2Index]));
        float circleDotProd = circleCenter.x * normal.x + circleCenter.y * normal.y;

        polyMinMax = GetMinMax(polyPts, polyPtsCount, normal);

        if (circleDotProd + circleRadius < polyMinMax.min
            || circleDotProd - circleRadius > polyMinMax.max)
            return false;
    }

    return true;
}

bool CheckColCircles(Vector2 c1Center, float c1Radius, Vector2 c2Center, float c2Radius)
{
   return Vector2DistanceSqr(c1Center, c2Center) <= (c1Radius + c2Radius) * (c1Radius + c2Radius);
}