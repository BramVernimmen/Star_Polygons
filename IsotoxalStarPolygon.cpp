#include "pch.h"
#include "IsotoxalStarPolygon.h"
#include <iostream>

IsotoxalStarPolygon::IsotoxalStarPolygon(const Point2f& centerPoint)
	: m_CenterPoint{ centerPoint }
	, m_NumberOfVertices{ 7 }
	, m_Alpha{ 24.f * float(M_PI) / 180.f }
	, m_Radius{ 200.f } //250.f
	, m_AngleOffset{ 36.f * float(M_PI) / 180.f }
{
	CalculateVerticesPositions();
}

void IsotoxalStarPolygon::Draw() const
{
	//utils::FillPolygon(m_VerticesPositions);
	//utils::DrawPolygon(m_VerticesPositions, true, 5.f);
	size_t amountOfTriangles{ m_VerticesPositions.size()};
	for (size_t index{0}; index < amountOfTriangles; ++index)
	{
		size_t nextIndex{ index + 1 };
		if (nextIndex == amountOfTriangles )
		{
			nextIndex = 0;
		}
		utils::FillTriangle(m_VerticesPositions[index], m_CenterPoint, m_VerticesPositions[nextIndex]);
	}

	//utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
	//utils::FillEllipse(m_CenterPoint, m_Radius, m_Radius);
	//utils::SetColor(Color4f{ 0.f,1.f,0.f,1.f });
	//utils::FillEllipse(m_CenterPoint, 168.204f, 168.204f);
	//
	//utils::SetColor(Color4f{ 0.f,0.f,0.f,1.f });
	//
	//for (int i{0}; i < m_VerticesPositions.size(); ++i)
	//{
	//	utils::FillEllipse(m_VerticesPositions[i], 25.f, 25.f);
	//}
}

int IsotoxalStarPolygon::GetNumberOfVertices() const
{
	return m_NumberOfVertices;
}

float IsotoxalStarPolygon::GetAlpha() const
{
	return m_Alpha * 180.f / float(M_PI);
}

float IsotoxalStarPolygon::GetAngleOffset() const
{
	return m_AngleOffset * 180.f / float(M_PI);
}

void IsotoxalStarPolygon::ChangeNumberOfVertices(int newNumberOfVertices)
{
	m_NumberOfVertices = newNumberOfVertices;
	CalculateVerticesPositions();
}

void IsotoxalStarPolygon::ChangeAlpha(float newAlpha)
{
	m_Alpha = newAlpha * float(M_PI) / 180.f;
	CalculateVerticesPositions();
}

void IsotoxalStarPolygon::ChangeAngleOffset(float newAngleOffset)
{
	m_AngleOffset = newAngleOffset * float(M_PI) / 180.f;
	CalculateVerticesPositions();
}

void IsotoxalStarPolygon::CalculateVerticesPositions()
{
	std::vector<Point2f> tempOuterVertices;
	std::vector<Point2f> tempInnerVertices;

	m_AngleIncrement = (360.f / m_NumberOfVertices) * float(M_PI) / 180.f;
	m_VerticesPositions.clear();

	// first calculate the outer vertices
	for (int index{0}; index < m_NumberOfVertices; ++index)
	{
		tempOuterVertices.push_back(Point2f{ m_Radius * cosf(m_AngleIncrement * index + m_AngleOffset) + m_CenterPoint.x, m_Radius * sinf(m_AngleIncrement * index + m_AngleOffset) + m_CenterPoint.y });
	}

	// calculate the inner angle -> and the inner radius
	float innerAngle{ float(M_PI) - m_AngleIncrement / 2.f - m_Alpha / 2.f };
	float innerRadius{ m_Radius * (sinf(m_Alpha / 2.f) / sinf(innerAngle)) };

	//std::cout << m_Radius << ", " << innerRadius << std::endl;
	//std::cout << m_AngleIncrement << ", " << innerAngle << std::endl;

	// calculate all inner vertices
	for (int index{ 0 }; index < m_NumberOfVertices; ++index)
	{
		tempInnerVertices.push_back(Point2f{ innerRadius * cosf(m_AngleIncrement/2.f + m_AngleIncrement * index + m_AngleOffset) + m_CenterPoint.x, innerRadius * sinf(m_AngleIncrement / 2.f + m_AngleIncrement * index + m_AngleOffset) + m_CenterPoint.y });
	}

	// push all vertices into right order in the right vector
	for (int index{ 0 }; index < m_NumberOfVertices; ++index)
	{
		m_VerticesPositions.push_back(tempOuterVertices[index]);
		m_VerticesPositions.push_back(tempInnerVertices[index]);
	}
}
