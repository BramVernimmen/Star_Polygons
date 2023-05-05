#pragma once
#include <vector>
#include "utils.h"
class IsotoxalStarPolygon
{
public:
	explicit IsotoxalStarPolygon(const Point2f& centerPoint);
	~IsotoxalStarPolygon() = default;
	IsotoxalStarPolygon(const IsotoxalStarPolygon& other) = delete;
	IsotoxalStarPolygon& operator=(const IsotoxalStarPolygon& other) = delete;
	IsotoxalStarPolygon(IsotoxalStarPolygon&& other) = delete;
	IsotoxalStarPolygon& operator=(IsotoxalStarPolygon&& other) = delete;

	void Draw() const;

	int GetNumberOfVertices() const;
	float GetAlpha() const;
	float GetAngleOffset() const;

	void ChangeNumberOfVertices(int newNumberOfVertices);
	void ChangeAlpha(float newAlpha);
	void ChangeAngleOffset(float newAngleOffset);

private:
	void CalculateVerticesPositions();

	const Point2f m_CenterPoint;

	int m_NumberOfVertices;
	float m_Alpha;
	float m_Radius;

	float m_AngleIncrement;

	float m_AngleOffset;

	std::vector<Point2f> m_VerticesPositions;
};

