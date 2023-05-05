#include "pch.h"
#include "RegularStarPolygon.h"

RegularStarPolygon::RegularStarPolygon(const Point2f& centerPoint)
	: m_CenterPoint{centerPoint}
	, m_NumberOfVertices{7}
	, m_Density{3}
	, m_Radius{250.f}
	, m_LineThickness{ 5.f }
	, m_AngleOffset{0.f}
{
	m_VerticesRadius = m_LineThickness / 2.f - 1.f;
	CalculateVerticesPositions();
}

void RegularStarPolygon::Draw() const
{
	int currentPoint{ 0 };
	while (currentPoint != m_Density + 1)
	{

		int nextPoint{ currentPoint + m_Density };
		if (nextPoint >= m_NumberOfVertices)
		{
			nextPoint = nextPoint - m_NumberOfVertices;
		}
		for (int index{ 0 }; index < m_NumberOfVertices; ++index)
		{
			utils::DrawLine(m_VerticesPositions[currentPoint], m_VerticesPositions[nextPoint], m_LineThickness);
			currentPoint = nextPoint;
			if (nextPoint + m_Density >= m_NumberOfVertices)
			{
				nextPoint = (nextPoint + m_Density) - m_NumberOfVertices;
			}
			else
			{
				nextPoint += m_Density;
			}
			utils::FillEllipse(m_VerticesPositions[index], m_VerticesRadius, m_VerticesRadius);

		}
		++currentPoint;
	}
}

int RegularStarPolygon::GetNumberOfVertices() const
{
	return m_NumberOfVertices;
}

int RegularStarPolygon::GetDensity() const
{
	return m_Density;
}

float RegularStarPolygon::GetAngleOffset() const
{
	return m_AngleOffset * 180.f / float(M_PI) ;
}

void RegularStarPolygon::ChangeNumberOfVertices(int newNumberOfVertices)
{
	m_NumberOfVertices = newNumberOfVertices;
	CalculateVerticesPositions();
}

void RegularStarPolygon::ChangeDensity(int newDensity)
{
	if (newDensity == 0)
	{
		newDensity = 1;
	}
	m_Density = newDensity;
}

void RegularStarPolygon::ChangeAngleOffset(float newAngleOffset)
{
	m_AngleOffset = newAngleOffset * float(M_PI) / 180.f;
	CalculateVerticesPositions();

}

void RegularStarPolygon::CalculateVerticesPositions()
{
	m_AngleIncrement = (360.f / m_NumberOfVertices) * float(M_PI) / 180.f;
	m_VerticesPositions.clear();
	for (int index{0}; index< m_NumberOfVertices; ++index)
	{
		Point2f tempValue;
		tempValue.x = m_Radius * cosf(m_AngleIncrement * index + m_AngleOffset) + m_CenterPoint.x;
		tempValue.y = m_Radius * sinf(m_AngleIncrement * index + m_AngleOffset) + m_CenterPoint.y;
		m_VerticesPositions.push_back(tempValue);
	}
}
