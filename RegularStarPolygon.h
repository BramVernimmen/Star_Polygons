#pragma once
#include <vector>
#include "utils.h"

class RegularStarPolygon
{
public:
	explicit RegularStarPolygon(const Point2f& centerPoint);
	~RegularStarPolygon() = default;
	RegularStarPolygon(const RegularStarPolygon& other) = delete;
	RegularStarPolygon& operator=(const RegularStarPolygon& other) = delete;
	RegularStarPolygon(RegularStarPolygon&& other) = delete;
	RegularStarPolygon& operator=(RegularStarPolygon&& other) = delete;

	void Draw() const;

	int GetNumberOfVertices() const;
	int GetDensity() const;
	float GetAngleOffset() const;

	void ChangeNumberOfVertices(int newNumberOfVertices);
	void ChangeDensity(int newDensity);
	void ChangeAngleOffset(float newAngleOffset);

private:
	void CalculateVerticesPositions();

	const Point2f m_CenterPoint;

	int m_NumberOfVertices;
	int m_Density;
	float m_Radius;

	float m_AngleIncrement;
	
	float m_LineThickness;
	float m_VerticesRadius;

	float m_AngleOffset;

	std::vector<Point2f> m_VerticesPositions;
};

