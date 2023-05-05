#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>

Game::Game( const Window& window ) 
	:m_Window{ window }
	, m_TextNumberOfVerticesRegularBase{"Number Of Vertices:  "}
	, m_TextNumberOfVerticesIsotoxalBase{"Number Of Vertices:  "}
	, m_TextDensityBase{"Density:  "}
	, m_TextAlphaBase{"Alpha:  "}
	, m_TextAngleOffsetRegularBase{"Angle Offset: "}
	, m_TextAngleOffsetIsotoxalBase{"Angle Offset: "}
	, m_TextBGColorBase{"BG Color: "}
	, m_TextStarColorBase{"Star Color: "}
	, m_FontPath{ "Resources/Baloo.ttf" }
	, m_TitleTextSize{50}
	, m_SettingsTextSize{40}
	, m_TextBGColorHitBox{ Rectf{12.f, 20.f, 415.f, 28.f} }
	, m_TextStarColorHitBox{ Rectf{616.f, 20.f, 415.f, 28.f} }
	, m_NumberVerticesRegularHitBox{Rectf{19.f, 210.f, 430.f, 28.f}}
	, m_DensityHitBox{Rectf{19.f, 151.f, 210.f, 28.f}}
	, m_AngleOffsetRegularHitBox{Rectf{19.f, 90.f, 320.f, 28.f}}
	, m_NumberVerticesIsotoxalHitBox{Rectf{617.f, 210.f, 430.f, 28.f}}
	, m_AlphaHitBox{Rectf{617.f, 151.f, 210.f, 28.f}}
	, m_AngleOffsetIsotoxalHitBox{Rectf{617.f, 90.f, 320.f, 28.f}}
	, m_IsChangingAnElement{false}
	, m_IsChangingBGColor{false}
	, m_IsChangingStarColor{false}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	// initialize the layout
	float titleBoxHeight{ 50.f };
	m_TitleBox = Rectf{ 0.f, m_Window.height - titleBoxHeight, m_Window.width, titleBoxHeight };
	float settingsBoxHeight{ 250.f };
	m_SettingsBox = Rectf{ 0.f, 0.f, m_Window.width, settingsBoxHeight };
	m_BoxColor = Color4f{ 0.15f, 0.15f, 0.15f, 1.f };
	float starsBoxHeight{ 600.f };	
	m_StarsBox = Rectf{ 0.f, settingsBoxHeight, m_Window.width, starsBoxHeight };


	// initialize the changable colors
	m_StarsBoxChangableColor = ChangableColor{ 216.f, 229.f, 204.f };
	m_StarsBoxColor = Color4f{ m_StarsBoxChangableColor.r / 255.f, m_StarsBoxChangableColor.g / 255.f, m_StarsBoxChangableColor.b / 255.f, 1.f };

	m_StarsChangableColor = ChangableColor{ 0.f, 0.f, 0.f };
	m_StarsColor = Color4f{ m_StarsChangableColor.r / 255.f, m_StarsChangableColor.g / 255.f, m_StarsChangableColor.b / 255.f, 1.f };


	// initialize the stars with position
	m_CenterRegularPosition = Point2f{ m_Window.width / 4.f, settingsBoxHeight + starsBoxHeight / 2.f };
	m_CenterIsotoxalPosition = Point2f{ m_Window.width / 4.f * 3.f, settingsBoxHeight + starsBoxHeight / 2.f };
	m_pRegularStarPolygon = new RegularStarPolygon(m_CenterRegularPosition);
	m_pIsotoxalStarPolygon = new IsotoxalStarPolygon(m_CenterIsotoxalPosition);

	// initialize text
	m_TextColor = Color4f{ 98 / 255.f, 146 / 255.f, 99 / 255.f, 1.f };
	m_ChangingTextColor = Color4f{ 98 / 255.f, 99 / 255.f, 146 / 255.f, 1.f };

	m_RegularStarPolygonText = new Texture("Regular Star Polygon", m_FontPath, m_TitleTextSize, m_TextColor);
	m_IsotoxalStarPolygonText = new Texture("Isotoxal Star Polygon", m_FontPath, m_TitleTextSize, m_TextColor);
	m_TextNumberOfVerticesRegular = new Texture(m_TextNumberOfVerticesRegularBase + std::to_string(m_pRegularStarPolygon->GetNumberOfVertices()), m_FontPath, m_SettingsTextSize, m_TextColor);
	m_TextDensity = new Texture(m_TextDensityBase + std::to_string(m_pRegularStarPolygon->GetDensity()), m_FontPath, m_SettingsTextSize, m_TextColor);
	m_TextAngleOffsetRegular = new Texture(m_TextAngleOffsetRegularBase + std::to_string(int(m_pRegularStarPolygon->GetAngleOffset())), m_FontPath, m_SettingsTextSize, m_TextColor);
	
	m_TextNumberOfVerticesIsotoxal = new Texture(m_TextNumberOfVerticesIsotoxalBase + std::to_string(m_pIsotoxalStarPolygon->GetNumberOfVertices()), m_FontPath, m_SettingsTextSize, m_TextColor);
	m_TextAlpha = new Texture(m_TextAlphaBase + std::to_string(int(m_pIsotoxalStarPolygon->GetAlpha())), m_FontPath, m_SettingsTextSize, m_TextColor);
	m_TextAngleOffsetIsotoxal = new Texture(m_TextAngleOffsetIsotoxalBase + std::to_string(int(m_pIsotoxalStarPolygon->GetAngleOffset())), m_FontPath, m_SettingsTextSize, m_TextColor);

	// initialize text positions
	float titleYPosition{ settingsBoxHeight + starsBoxHeight - m_TitleTextSize/4.f };
	float titleXPositionOffset{ 55.f };
	m_RegularStarPolygonPosition = Point2f{ titleXPositionOffset, titleYPosition };
	m_IsotoxalStarPolygonPosition = Point2f{ m_Window.width / 2.f + titleXPositionOffset, titleYPosition };
	m_TextNumberOfVerticesRegularPosition = Point2f{ 20.f , settingsBoxHeight - m_SettingsTextSize - m_SettingsTextSize / 2.f};
	m_TextDensityPosition = Point2f{ 20.f , m_TextNumberOfVerticesRegularPosition.y - m_SettingsTextSize - m_SettingsTextSize / 2.f };
	m_TextAngleOffsetRegularPosition = Point2f{ 20.f , m_TextDensityPosition.y - m_SettingsTextSize - m_SettingsTextSize / 2.f };

	m_TextNumberOfVerticesIsotoxalPosition = Point2f{ m_Window.width / 2.f + 20.f , settingsBoxHeight - m_SettingsTextSize - m_SettingsTextSize / 2.f};
	m_TextAlphaPosition = Point2f{ m_Window.width / 2.f + 20.f , m_TextNumberOfVerticesRegularPosition.y - m_SettingsTextSize - m_SettingsTextSize / 2.f };
	m_TextAngleOffsetIsotoxalPosition = Point2f{ m_Window.width / 2.f + 20.f , m_TextDensityPosition.y - m_SettingsTextSize - m_SettingsTextSize / 2.f };
	
	m_TextBGColorPosition = Point2f{ 20.f, 0.f };
	m_TextStarColorPosition = Point2f{ m_Window.width / 2.f + 20.f, 0.f };


	// initialize changable colors
	{
		std::string redValue{ std::to_string(int(m_StarsBoxChangableColor.r)) };
		std::string greenValue{ std::to_string(int(m_StarsBoxChangableColor.g)) };
		std::string blueValue{ std::to_string(int(m_StarsBoxChangableColor.b)) };

		while (redValue.size() < 3)
		{
			redValue = "0" + redValue;
		}
		while (greenValue.size() < 3)
		{
			greenValue = "0" + greenValue;
		}
		while (blueValue.size() < 3)
		{
			blueValue = "0" + blueValue;
		}
		//const std::string colorValue{ m_TextBGColorBase + redValue + ", " + greenValue + ", " + blueValue };
		m_TextBGColor = new Texture(m_TextBGColorBase, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_BGRedValue = new Texture(redValue, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_BGGreenValue = new Texture(greenValue, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_BGBlueValue = new Texture(blueValue, m_FontPath, m_SettingsTextSize, m_TextColor);
	}	

	{
		std::string redValue{ std::to_string(int(m_StarsChangableColor.r)) };
		std::string greenValue{ std::to_string(int(m_StarsChangableColor.g)) };
		std::string blueValue{ std::to_string(int(m_StarsChangableColor.b)) };

		while (redValue.size() < 3)
		{
			redValue = "0" + redValue;
		}
		while (greenValue.size() < 3)
		{
			greenValue = "0" + greenValue;
		}
		while (blueValue.size() < 3)
		{
			blueValue = "0" + blueValue;
		}
		m_TextStarColor = new Texture(m_TextStarColorBase, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_StarRedValue = new Texture(redValue, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_StarGreenValue = new Texture(greenValue, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_StarBlueValue = new Texture(blueValue, m_FontPath, m_SettingsTextSize, m_TextColor);
	}

}

void Game::Cleanup( )
{
	delete m_RegularStarPolygonText;
	m_RegularStarPolygonText = nullptr;

	delete m_IsotoxalStarPolygonText;
	m_IsotoxalStarPolygonText = nullptr;

	delete m_pRegularStarPolygon;
	m_pRegularStarPolygon = nullptr;

	delete m_pIsotoxalStarPolygon;
	m_pIsotoxalStarPolygon = nullptr;

	delete m_TextNumberOfVerticesRegular;
	m_TextNumberOfVerticesRegular = nullptr;

	delete m_TextDensity;
	m_TextDensity = nullptr;

	delete m_TextAngleOffsetRegular;
	m_TextAngleOffsetRegular = nullptr;
	

	delete m_TextNumberOfVerticesIsotoxal;
	m_TextNumberOfVerticesIsotoxal = nullptr;

	delete m_TextAlpha;
	m_TextAlpha = nullptr;

	delete m_TextAngleOffsetIsotoxal;
	m_TextAngleOffsetIsotoxal = nullptr;

	delete m_TextBGColor;
	m_TextBGColor = nullptr;

	delete m_BGRedValue;
	m_BGRedValue = nullptr;

	delete m_BGGreenValue;
	m_BGGreenValue = nullptr;

	delete m_BGBlueValue;
	m_BGBlueValue = nullptr;

	delete m_TextStarColor;
	m_TextStarColor = nullptr;

	delete m_StarRedValue;
	m_StarRedValue = nullptr;

	delete m_StarGreenValue;
	m_StarGreenValue = nullptr;

	delete m_StarBlueValue;
	m_StarBlueValue = nullptr;
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );

	// draw the program layout
	utils::SetColor(m_StarsBoxColor);
	utils::FillRect(m_StarsBox);
	utils::SetColor(m_BoxColor);
	utils::FillRect(m_TitleBox);
	utils::FillRect(m_SettingsBox);
	utils::DrawLine(Point2f{ m_Window.width / 2.f, 0.f }, Point2f{ m_Window.width / 2.f, m_Window.height }, 10.f);
	utils::SetColor(Color4f{ 0.f,0.f,0.f, 1.f });
	float Yoffset{ float(m_SettingsTextSize + m_SettingsTextSize / 3.f) };
	utils::DrawLine(Point2f{ 0.f, Yoffset }, Point2f{ m_Window.width, Yoffset }, 5.f);



	// draw the stars
	utils::SetColor(m_StarsColor);
	m_pRegularStarPolygon->Draw();
	m_pIsotoxalStarPolygon->Draw();
	//float radius{ 250.f };
	//utils::FillEllipse(m_CenterIsotoxalPosition, radius, radius);



	// draw text
	m_RegularStarPolygonText->Draw(m_RegularStarPolygonPosition);
	m_IsotoxalStarPolygonText->Draw(m_IsotoxalStarPolygonPosition);

	m_TextNumberOfVerticesRegular->Draw(m_TextNumberOfVerticesRegularPosition);
	m_TextDensity->Draw(m_TextDensityPosition);
	m_TextAngleOffsetRegular->Draw(m_TextAngleOffsetRegularPosition);

	m_TextNumberOfVerticesIsotoxal->Draw(m_TextNumberOfVerticesIsotoxalPosition);
	m_TextAlpha->Draw(m_TextAlphaPosition);
	m_TextAngleOffsetIsotoxal->Draw(m_TextAngleOffsetIsotoxalPosition);


	m_TextBGColor->Draw(m_TextBGColorPosition);
	m_BGRedValue->Draw(m_BGRedValuePositionIndex);
	m_BGGreenValue->Draw(m_BGGreenValuePositionIndex);
	m_BGBlueValue->Draw(m_BGBlueValuePositionIndex);


	m_TextStarColor->Draw(m_TextStarColorPosition);
	m_StarRedValue->Draw(m_StarRedValuePositionIndex);
	m_StarGreenValue->Draw(m_StarGreenValuePositionIndex);
	m_StarBlueValue->Draw(m_StarBlueValuePositionIndex);


	// draw index ptr
	if (m_IsChangingAnElement == true)
	{
		if (m_IsChangingBGColor == true)
		{
			utils::SetColor(m_ChangingTextColor);
			if (m_HasBGRedChanged == false)
			{
				utils::DrawLine(Point2f{ m_BGRedValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex , 15.f }, Point2f{ m_BGRedValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex + m_IndexWidth / 2.f, 15.f }, 5.f);
			}
			else if (m_HasBGGreenChanged == false)
			{
				utils::DrawLine(Point2f{ m_BGGreenValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex , 15.f }, Point2f{ m_BGGreenValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex + m_IndexWidth / 2.f, 15.f }, 5.f);
			}
			else if (m_HasBGBlueChanged == false)
			{
				utils::DrawLine(Point2f{ m_BGBlueValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex , 15.f }, Point2f{ m_BGBlueValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex + m_IndexWidth / 2.f, 15.f }, 5.f);
			}
		}
		else if (m_IsChangingStarColor == true)
		{
			utils::SetColor(m_ChangingTextColor);
			if (m_HasStarRedChanged == false)
			{
				utils::DrawLine(Point2f{ m_StarRedValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex , 15.f }, Point2f{ m_StarRedValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex + m_IndexWidth / 2.f, 15.f }, 5.f);
			}
			else if (m_HasStarGreenChanged == false)
			{
				utils::DrawLine(Point2f{ m_StarGreenValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex , 15.f }, Point2f{ m_StarGreenValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex + m_IndexWidth / 2.f, 15.f }, 5.f);
			}
			else if (m_HasStarBlueChanged == false)
			{
				utils::DrawLine(Point2f{ m_StarBlueValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex , 15.f }, Point2f{ m_StarBlueValuePositionIndex.x + m_IndexWidth * m_ColorChangeIndex + m_IndexWidth / 2.f, 15.f }, 5.f);
			}
		}
	}
	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_LEFT:	
		if (m_IsChangingBGColor == true || m_IsChangingStarColor == true)
		{
			m_ColorChangeIndex -= 1;
			if (m_ColorChangeIndex < 0)
			{
				m_ColorChangeIndex = 2;
			}
		}
		//std::cout << "Left arrow key released\n";
		break;
	case SDLK_RIGHT:
		if (m_IsChangingBGColor == true || m_IsChangingStarColor == true)
		{
			m_ColorChangeIndex += 1;
			if (m_ColorChangeIndex > 2)
			{
				m_ColorChangeIndex = 0;
			}
		}
		//std::cout << "`Right arrow key released\n";
		break;
	case SDLK_0:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(0);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(0);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "0";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "0";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "0";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "0";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "0";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "0";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_0:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(0);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(0);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "0";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "0";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "0";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "0";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "0";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "0";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_1:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(1);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(1);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "1";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "1";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "1";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "1";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "1";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "1";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_1:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(1);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(1);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "1";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "1";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "1";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "1";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "1";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "1";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_2:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(2);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(2);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "2";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "2";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "2";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "2";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "2";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "2";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_2:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(2);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(2);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "2";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "2";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "2";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "2";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "2";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "2";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_3:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(3);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(3);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "3";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "3";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "3";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "3";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "3";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "3";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_3:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(3);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(3);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "3";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "3";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "3";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "3";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "3";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "3";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_4:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(4);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(4);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "4";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "4";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "4";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "4";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "4";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "4";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_4:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(4);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(4);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "4";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "4";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "4";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "4";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "4";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "4";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_5:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(5);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(5);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "5";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "5";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "5";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "5";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "5";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "5";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_5:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(5);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(5);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "5";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "5";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "5";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "5";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "5";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "5";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_6:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(6);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(6);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "6";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "6";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "6";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "6";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "6";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "6";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_6:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(6);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(6);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "6";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "6";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "6";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "6";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "6";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "6";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_7:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(7);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(7);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "7";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "7";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "7";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "7";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "7";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "7";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_7:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(7);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(7);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "7";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "7";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "7";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "7";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "7";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "7";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_8:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(8);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(8);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "8";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "8";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "8";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "8";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "8";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "8";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_8:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(8);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(8);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "8";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "8";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "8";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "8";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "8";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "8";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_9:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(9);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(9);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "9";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "9";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "9";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "9";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "9";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "9";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_KP_9:
		if (m_IsChangingBGColor == true)
		{
			ChangeBGColorValue(9);
			ChangeBGColorTextColor(true);
		}
		else if (m_IsChangingStarColor == true)
		{
			ChangeStarColorValue(9);
			ChangeStarColorTextColor(true);
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_TempString += "9";
			ChangeNumberOfVerticesRegularColor(true);
		}
		else if (m_IsChangingDensity == true)
		{
			m_TempString += "9";
			ChangeDensityColor(true);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_TempString += "9";
			ChangeAngleOffsetRegularColor(true);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_TempString += "9";
			ChangeNumberOfVerticesIsotoxalColor(true);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_TempString += "9";
			ChangeAlphaColor(true);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_TempString += "9";
			ChangeAngleOffsetIsotoxalColor(true);
		}
		break;
	case SDLK_RETURN:
		if (m_IsChangingBGColor == true)
		{
			if (m_HasBGRedChanged == false)
			{
				m_HasBGRedChanged = true;
			}
			else if (m_HasBGGreenChanged == false)
			{
				m_HasBGGreenChanged = true;
			}
			else if (m_HasBGBlueChanged == false)
			{
				m_IsChangingBGColor = false;
				m_IsChangingAnElement = false;
				m_HasBGGreenChanged = false;
				m_HasBGRedChanged = false;
				ChangeBGColorTextColor(false);
			}
		}
		else if (m_IsChangingStarColor == true)
		{
			if (m_HasStarRedChanged == false)
			{
				m_HasStarRedChanged = true;
			}
			else if (m_HasStarGreenChanged == false)
			{
				m_HasStarGreenChanged = true;
			}
			else if (m_HasStarBlueChanged == false)
			{
				m_IsChangingStarColor = false;
				m_IsChangingAnElement = false;
				m_HasStarGreenChanged = false;
				m_HasStarRedChanged = false;
				ChangeStarColorTextColor(false);
			}
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingNumberOfVerticesRegular = false;
			ChangeNumberOfVerticesRegular();
			ChangeNumberOfVerticesRegularColor(false);
		}
		else if (m_IsChangingDensity == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingDensity = false;
			ChangeDensity();
			ChangeDensityColor(false);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingAngleOffsetRegular = false;
			ChangeAngleOffsetRegular();
			ChangeAngleOffsetRegularColor(false);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingNumberOfVerticesIsotoxal = false;
			ChangeNumberOfVerticesIsotoxal();
			ChangeNumberOfVerticesIsotoxalColor(false);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingAlpha = false;
			ChangeAlpha();
			ChangeAlphaColor(false);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingAngleOffsetIsotoxal = false;
			ChangeAngleOffsetIsotoxal();
			ChangeAngleOffsetIsotoxalColor(false);
		}
		break;
	case SDLK_KP_ENTER:
		if (m_IsChangingBGColor == true)
		{
			if (m_HasBGRedChanged == false)
			{
				m_HasBGRedChanged = true;
			}
			else if (m_HasBGGreenChanged == false)
			{
				m_HasBGGreenChanged = true;
			}
			else if (m_HasBGBlueChanged == false)
			{
				m_IsChangingBGColor = false;
				m_IsChangingAnElement = false;
				m_HasBGGreenChanged = false;
				m_HasBGRedChanged = false;
				ChangeBGColorTextColor(false);
			}
		}
		else if (m_IsChangingStarColor == true)
		{
			if (m_HasStarRedChanged == false)
			{
				m_HasStarRedChanged = true;
			}
			else if (m_HasStarGreenChanged == false)
			{
				m_HasStarGreenChanged = true;
			}
			else if (m_HasStarBlueChanged == false)
			{
				m_IsChangingStarColor = false;
				m_IsChangingAnElement = false;
				m_HasStarGreenChanged = false;
				m_HasStarRedChanged = false;
				ChangeStarColorTextColor(false);
			}
		}
		else if (m_IsChangingNumberOfVerticesRegular == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingNumberOfVerticesRegular = false;
			ChangeNumberOfVerticesRegular();
			ChangeNumberOfVerticesRegularColor(false);
		}
		else if (m_IsChangingDensity == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingDensity = false;
			ChangeDensity();
			ChangeDensityColor(false);
		}
		else if (m_IsChangingAngleOffsetRegular == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingAngleOffsetRegular = false;
			ChangeAngleOffsetRegular();
			ChangeAngleOffsetRegularColor(false);
		}
		else if (m_IsChangingNumberOfVerticesIsotoxal == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingNumberOfVerticesIsotoxal = false;
			ChangeNumberOfVerticesIsotoxal();
			ChangeNumberOfVerticesIsotoxalColor(false);
		}
		else if (m_IsChangingAlpha == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingAlpha = false;
			ChangeAlpha();
			ChangeAlphaColor(false);
		}
		else if (m_IsChangingAngleOffsetIsotoxal == true)
		{
			m_IsChangingAnElement = false;
			m_IsChangingAngleOffsetIsotoxal = false;
			ChangeAngleOffsetIsotoxal();
			ChangeAngleOffsetIsotoxalColor(false);
		}
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
	m_MousePos.x = float(e.x);
	m_MousePos.y = float(e.y);
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		if (m_IsChangingAnElement == false)
		{
			if (utils::IsPointInRect(m_MousePos, m_TextBGColorHitBox) == true)
			{
				m_IsChangingAnElement = true;
				m_IsChangingBGColor = true;
				ChangeBGColorTextColor(true);
			}
			else if (utils::IsPointInRect(m_MousePos, m_TextStarColorHitBox) == true)
			{
				m_IsChangingAnElement = true;
				m_IsChangingStarColor = true;
				ChangeStarColorTextColor(true);
			}
			else if (utils::IsPointInRect(m_MousePos, m_NumberVerticesRegularHitBox) == true)
			{
				m_IsChangingNumberOfVerticesRegular = true;
				m_IsChangingAnElement = true;
				ChangeNumberOfVerticesRegularColor(true);
			}
			else if (utils::IsPointInRect(m_MousePos, m_DensityHitBox) == true)
			{
				m_IsChangingDensity = true;
				m_IsChangingAnElement = true;
				ChangeDensityColor(true);
			}
			else if (utils::IsPointInRect(m_MousePos, m_AngleOffsetRegularHitBox) == true)
			{
				m_IsChangingAngleOffsetRegular = true;
				m_IsChangingAnElement = true;
				ChangeAngleOffsetRegularColor(true);
			}
			else if (utils::IsPointInRect(m_MousePos, m_NumberVerticesIsotoxalHitBox) == true)
			{
				m_IsChangingNumberOfVerticesIsotoxal = true;
				m_IsChangingAnElement = true;
				ChangeNumberOfVerticesIsotoxalColor(true);
			}
			else if (utils::IsPointInRect(m_MousePos, m_AlphaHitBox) == true)
			{
				m_IsChangingAlpha = true;
				m_IsChangingAnElement = true;
				ChangeAlphaColor(true);
			}
			else if (utils::IsPointInRect(m_MousePos, m_AngleOffsetIsotoxalHitBox) == true)
			{
				m_IsChangingAngleOffsetIsotoxal = true;
				m_IsChangingAnElement = true;
				ChangeAngleOffsetIsotoxalColor(true);
			}
		}
		else
		{
			if (utils::IsPointInRect(m_MousePos, m_TextBGColorHitBox) == false && m_IsChangingBGColor == true)
			{
				m_IsChangingAnElement = false;
				m_IsChangingBGColor = false;
				m_HasBGRedChanged = false;
				m_HasBGGreenChanged = false;
				m_HasBGBlueChanged = false;
				ChangeBGColorTextColor(false);
			}
			else if (utils::IsPointInRect(m_MousePos, m_TextStarColorHitBox) == false && m_IsChangingStarColor == true)
			{
				m_IsChangingAnElement = false;
				m_IsChangingStarColor = false;
				m_HasStarRedChanged = false;
				m_HasStarGreenChanged = false;
				m_HasStarBlueChanged = false;
				ChangeStarColorTextColor(false);
			}
			else if (utils::IsPointInRect(m_MousePos, m_NumberVerticesRegularHitBox) == false && m_IsChangingNumberOfVerticesRegular == true)
			{
				m_IsChangingNumberOfVerticesRegular = false;
				m_IsChangingAnElement = false;
				ChangeNumberOfVerticesRegularColor(false);
				m_TempString.clear();

			}
			else if (utils::IsPointInRect(m_MousePos, m_DensityHitBox) == false && m_IsChangingDensity == true)
			{
				m_IsChangingDensity = false;
				m_IsChangingAnElement = false;
				ChangeDensityColor(false);
				m_TempString.clear();
			}
			else if (utils::IsPointInRect(m_MousePos, m_AngleOffsetRegularHitBox) == false && m_IsChangingAngleOffsetRegular == true)
			{
				m_IsChangingAngleOffsetRegular = false;
				m_IsChangingAnElement = false;
				ChangeAngleOffsetRegularColor(false);
				m_TempString.clear();
			}
			else if (utils::IsPointInRect(m_MousePos, m_NumberVerticesIsotoxalHitBox) == false && m_IsChangingNumberOfVerticesIsotoxal == true)
			{
				m_IsChangingNumberOfVerticesIsotoxal = false;
				m_IsChangingAnElement = false;
				ChangeNumberOfVerticesIsotoxalColor(false);
				m_TempString.clear();

			}
			else if (utils::IsPointInRect(m_MousePos, m_AlphaHitBox) == false && m_IsChangingAlpha == true)
			{
				m_IsChangingAlpha = false;
				m_IsChangingAnElement = false;
				ChangeAlphaColor(false);
				m_TempString.clear();
			}
			else if (utils::IsPointInRect(m_MousePos, m_AngleOffsetIsotoxalHitBox) == false && m_IsChangingAngleOffsetIsotoxal == true)
			{
				m_IsChangingAngleOffsetIsotoxal = false;
				m_IsChangingAnElement = false;
				ChangeAngleOffsetIsotoxalColor(false);
				m_TempString.clear();
			}
		}
		//std::cout << " left button " << std::endl;
		break;
	case SDL_BUTTON_RIGHT:
		//std::cout << " right button " << std::endl;
		break;
	case SDL_BUTTON_MIDDLE:
		//std::cout << " middle button " << std::endl;
		break;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::ChangeBGColorValue(int value)
{
	std::string tempString{ };
	if (m_HasBGRedChanged == false)
	{
		tempString = std::to_string(int(m_StarsBoxChangableColor.r));
	}
	else if (m_HasBGGreenChanged == false)
	{
		tempString = std::to_string(int(m_StarsBoxChangableColor.g));
	}
	else if (m_HasBGBlueChanged == false)
	{
		tempString = std::to_string(int(m_StarsBoxChangableColor.b));
	}


	while (tempString.size() < 3)
	{
		tempString = "0" + tempString;
	}
	if (m_ColorChangeIndex == 0)
	{
		// can't be greather than 2
		if (value < 2)
		{
			tempString[m_ColorChangeIndex] = value + 48;
		}
		else if (value == 2)
		{
			if (tempString[1] <= '5')
			{
				tempString[m_ColorChangeIndex] = value + 48;
			}
		}
	}
	else if (m_ColorChangeIndex == 1)
	{
		if (tempString[0] == '2')
		{
			if (tempString[2] > '5')
			{
				if (value <= 4)
				{
					tempString[m_ColorChangeIndex] = value + 48;
				}
			}
			else
			{
				if (value <= 5)
				{
					tempString[m_ColorChangeIndex] = value + 48;
				}
			}
			
		}
		else
		{
			tempString[m_ColorChangeIndex] = value + 48;
		}
		// can't be greather than 5 when [0] == 2
	}
	else
	{
		if (tempString[0] == '2' && tempString[1] == '5')
		{
			if (value <= 5)
			{
				tempString[m_ColorChangeIndex] = value + 48;
			}
		}
		else
		{
			tempString[m_ColorChangeIndex] = value + 48;
		}
		// can't be greather than 5 when [0] == 2
	}

	if (m_HasBGRedChanged == false)
	{
		m_StarsBoxChangableColor.r = std::stof(tempString);
	}
	else if (m_HasBGGreenChanged == false)
	{
		m_StarsBoxChangableColor.g = std::stof(tempString);
	}
	else if (m_HasBGBlueChanged == false)
	{
		m_StarsBoxChangableColor.b = std::stof(tempString);
	}
}

void Game::ChangeBGColorTextColor(bool isBeingEdited)
{
	delete m_TextBGColor;
	delete m_BGRedValue;
	delete m_BGGreenValue;
	delete m_BGBlueValue;

	std::string redValue{ std::to_string(int(m_StarsBoxChangableColor.r)) };
	std::string greenValue{ std::to_string(int(m_StarsBoxChangableColor.g)) };
	std::string blueValue{ std::to_string(int(m_StarsBoxChangableColor.b)) };

	while (redValue.size() < 3)
	{
		redValue = "0" + redValue;
	}
	while (greenValue.size() < 3)
	{
		greenValue = "0" + greenValue;
	}
	while (blueValue.size() < 3)
	{
		blueValue = "0" + blueValue;
	}
	
	
	if (isBeingEdited == true)
	{
		m_TextBGColor = new Texture(m_TextBGColorBase, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
		m_BGRedValue = new Texture(redValue, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
		m_BGGreenValue = new Texture(greenValue, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
		m_BGBlueValue = new Texture(blueValue, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
	}
	else
	{
		m_TextBGColor = new Texture(m_TextBGColorBase, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_BGRedValue = new Texture(redValue, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_BGGreenValue = new Texture(greenValue, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_BGBlueValue = new Texture(blueValue, m_FontPath, m_SettingsTextSize, m_TextColor);
	}
	m_StarsBoxColor = Color4f{ m_StarsBoxChangableColor.r / 255.f, m_StarsBoxChangableColor.g / 255.f, m_StarsBoxChangableColor.b / 255.f, 1.f };
}

void Game::ChangeStarColorValue(int value)
{

	std::string tempString{ };
	if (m_HasStarRedChanged == false)
	{
		tempString = std::to_string(int(m_StarsChangableColor.r));
	}
	else if (m_HasStarGreenChanged == false)
	{
		tempString = std::to_string(int(m_StarsChangableColor.g));
	}
	else if (m_HasStarBlueChanged == false)
	{
		tempString = std::to_string(int(m_StarsChangableColor.b));
	}


	while (tempString.size() < 3)
	{
		tempString = "0" + tempString;
	}
	if (m_ColorChangeIndex == 0)
	{
		// can't be greather than 2
		if (value < 2)
		{
			tempString[m_ColorChangeIndex] = value + 48;
		}
		else if (value == 2)
		{
			if (tempString[1] <= '5')
			{
				tempString[m_ColorChangeIndex] = value + 48;
			}
		}
	}
	else if (m_ColorChangeIndex == 1)
	{
		if (tempString[0] == '2')
		{
			if (tempString[2] > '5')
			{
				if (value <= 4)
				{
					tempString[m_ColorChangeIndex] = value + 48;
				}
			}
			else
			{
				if (value <= 5)
				{
					tempString[m_ColorChangeIndex] = value + 48;
				}
			}

		}
		else
		{
			tempString[m_ColorChangeIndex] = value + 48;
		}
		// can't be greather than 5 when [0] == 2
	}
	else
	{
		if (tempString[0] == '2' && tempString[1] == '5')
		{
			if (value <= 5)
			{
				tempString[m_ColorChangeIndex] = value + 48;
			}
		}
		else
		{
			tempString[m_ColorChangeIndex] = value + 48;
		}
		// can't be greather than 5 when [0] == 2
	}

	if (m_HasStarRedChanged == false)
	{
		m_StarsChangableColor.r = std::stof(tempString);
	}
	else if (m_HasStarGreenChanged == false)
	{
		m_StarsChangableColor.g = std::stof(tempString);
	}
	else if (m_HasStarBlueChanged == false)
	{
		m_StarsChangableColor.b = std::stof(tempString);
	}
}

void Game::ChangeStarColorTextColor(bool isBeingEdited)
{
	delete m_TextStarColor;
	delete m_StarRedValue;
	delete m_StarGreenValue;
	delete m_StarBlueValue;

	std::string redValue{ std::to_string(int(m_StarsChangableColor.r)) };
	std::string greenValue{ std::to_string(int(m_StarsChangableColor.g)) };
	std::string blueValue{ std::to_string(int(m_StarsChangableColor.b)) };

	while (redValue.size() < 3)
	{
		redValue = "0" + redValue;
	}
	while (greenValue.size() < 3)
	{
		greenValue = "0" + greenValue;
	}
	while (blueValue.size() < 3)
	{
		blueValue = "0" + blueValue;
	}


	if (isBeingEdited == true)
	{
		m_TextStarColor = new Texture(m_TextStarColorBase, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
		m_StarRedValue = new Texture(redValue, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
		m_StarGreenValue = new Texture(greenValue, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
		m_StarBlueValue = new Texture(blueValue, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
	}
	else
	{
		m_TextStarColor = new Texture(m_TextStarColorBase, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_StarRedValue = new Texture(redValue, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_StarGreenValue = new Texture(greenValue, m_FontPath, m_SettingsTextSize, m_TextColor);
		m_StarBlueValue = new Texture(blueValue, m_FontPath, m_SettingsTextSize, m_TextColor);
	}
	m_StarsColor = Color4f{ m_StarsChangableColor.r / 255.f, m_StarsChangableColor.g / 255.f, m_StarsChangableColor.b / 255.f, 1.f };
}

void Game::ChangeNumberOfVerticesRegular()
{
	if (m_TempString.size() == 0)
	{
		m_TempString = "2";
	}
	int tempValue{ std::stoi(m_TempString) };
	m_TempString.clear();
	if (tempValue == 0)
	{
		tempValue = 2;
	}
	if (tempValue > 100)
	{
		tempValue = 100;
	}
	if (tempValue < m_pRegularStarPolygon->GetDensity() || tempValue / 2 <= m_pRegularStarPolygon->GetDensity())
	{

		if (float(tempValue / 2.f) > float(tempValue / 2))
		{
			m_pRegularStarPolygon->ChangeDensity(tempValue / 2);
			//std::cout << m_pRegularStarPolygon->GetNumberOfVertices() / 2 << std::endl;
		}
		else if (float(tempValue / 2.f) == float(tempValue / 2))
		{
			m_pRegularStarPolygon->ChangeDensity(tempValue / 2 - 1);
			//std::cout << m_pRegularStarPolygon->GetNumberOfVertices() / 2 - 1 << std::endl;
		
		}
		//m_pRegularStarPolygon->ChangeDensity(tempValue / 2 - 1);
		ChangeDensityColor(false);
	}

	if (tempValue < 2)
	{
		tempValue = 2;
		m_pRegularStarPolygon->ChangeDensity(1);
		ChangeDensityColor(false);
		// change density
	}
	if (tempValue < 5)
	{
		m_pRegularStarPolygon->ChangeDensity(1);
		ChangeDensityColor(false);

		// change density
	}

	
	m_pRegularStarPolygon->ChangeNumberOfVertices(tempValue);
	// change vertices to tempvalue

}

void Game::ChangeNumberOfVerticesRegularColor(bool isBeingEdited)
{
	delete m_TextNumberOfVerticesRegular;

	if (isBeingEdited == true)
	{
		m_TextNumberOfVerticesRegular = new Texture(m_TextNumberOfVerticesRegularBase + m_TempString, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
	}
	else
	{
		m_TextNumberOfVerticesRegular = new Texture(m_TextNumberOfVerticesRegularBase + std::to_string(m_pRegularStarPolygon->GetNumberOfVertices()), m_FontPath, m_SettingsTextSize, m_TextColor);
	}

}

void Game::ChangeDensity()
{
	if (m_TempString.size() == 0)
	{
		m_TempString = "0";
	}
	int tempValue{ std::stoi(m_TempString) };
	m_TempString.clear();

	if (tempValue < 1)
	{
		tempValue = 1;
		// change density
	}

	if (tempValue >= m_pRegularStarPolygon->GetNumberOfVertices() / 2)
	{
		if (float(m_pRegularStarPolygon->GetNumberOfVertices()/2.f) > float(m_pRegularStarPolygon->GetNumberOfVertices() / 2))
		{
			tempValue = m_pRegularStarPolygon->GetNumberOfVertices() / 2;
		}
		else if (float(m_pRegularStarPolygon->GetNumberOfVertices() / 2.f) == float(m_pRegularStarPolygon->GetNumberOfVertices() / 2))
		{
			tempValue = m_pRegularStarPolygon->GetNumberOfVertices() / 2 - 1;
		}

		//std::cout << float(m_pRegularStarPolygon->GetNumberOfVertices() / 2.f) << "   " << float(m_pRegularStarPolygon->GetNumberOfVertices() / 2) << std::endl;
	}
	// check if correct
	//if (tempValue < 5)
	//{
	//	m_pRegularStarPolygon->ChangeDensity(1);
	//	// change density
	//}
	m_pRegularStarPolygon->ChangeDensity(tempValue);
	// change vertices to tempvalue

}

void Game::ChangeDensityColor(bool isBeingEdited)
{
	delete m_TextDensity;

	if (isBeingEdited == true)
	{
		m_TextDensity = new Texture(m_TextDensityBase + m_TempString, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
	}
	else
	{
		m_TextDensity = new Texture(m_TextDensityBase + std::to_string(m_pRegularStarPolygon->GetDensity()), m_FontPath, m_SettingsTextSize, m_TextColor);
	}

}


void Game::ChangeAngleOffsetRegular()
{
	if (m_TempString.size() == 0)
	{
		m_TempString = "0";
	}
	float tempValue{ std::stof(m_TempString) };
	m_TempString.clear();

	if (tempValue > 360)
	{
		tempValue = 360;
	}
	
	m_pRegularStarPolygon->ChangeAngleOffset(tempValue);

}

void Game::ChangeAngleOffsetRegularColor(bool isBeingEdited)
{
	delete m_TextAngleOffsetRegular;

	if (isBeingEdited == true)
	{
		m_TextAngleOffsetRegular = new Texture(m_TextAngleOffsetRegularBase + m_TempString, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
	}
	else
	{
		m_TextAngleOffsetRegular = new Texture(m_TextAngleOffsetRegularBase + std::to_string(int(m_pRegularStarPolygon->GetAngleOffset())), m_FontPath, m_SettingsTextSize, m_TextColor);
	}

}

void Game::ChangeNumberOfVerticesIsotoxal()
{
	if (m_TempString.size() == 0)
	{
		m_TempString = "2";
	}
	int tempValue{ std::stoi(m_TempString) };
	m_TempString.clear();

	if (tempValue < 2)
	{
		tempValue = 2;
	}
	if (tempValue > 250)
	{
		tempValue = 250;
	}

	m_pIsotoxalStarPolygon->ChangeNumberOfVertices(tempValue);
	// change vertices to tempvalue
}

void Game::ChangeNumberOfVerticesIsotoxalColor(bool isBeingEdited)
{
	delete m_TextNumberOfVerticesIsotoxal;

	if (isBeingEdited == true)
	{
		m_TextNumberOfVerticesIsotoxal = new Texture(m_TextNumberOfVerticesIsotoxalBase + m_TempString, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
	}
	else
	{
		m_TextNumberOfVerticesIsotoxal = new Texture(m_TextNumberOfVerticesIsotoxalBase + std::to_string(m_pIsotoxalStarPolygon->GetNumberOfVertices()), m_FontPath, m_SettingsTextSize, m_TextColor);
	}
}

void Game::ChangeAlpha()
{
	if (m_TempString.size() == 0)
	{
		m_TempString = "0";
	}
	float tempValue{ std::stof(m_TempString) };
	m_TempString.clear();

	if (tempValue < 5.f)
	{
		tempValue = 5.f;
	}

	if (tempValue > 180.f)
	{
		tempValue = 180.f;
	}

	// check multiple things

	m_pIsotoxalStarPolygon->ChangeAlpha(tempValue);
}

void Game::ChangeAlphaColor(bool isBeingEdited)
{
	delete m_TextAlpha;

	if (isBeingEdited == true)
	{
		m_TextAlpha = new Texture(m_TextAlphaBase + m_TempString, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
	}
	else
	{
		m_TextAlpha = new Texture(m_TextAlphaBase + std::to_string(int(m_pIsotoxalStarPolygon->GetAlpha())), m_FontPath, m_SettingsTextSize, m_TextColor);
	}
}

void Game::ChangeAngleOffsetIsotoxal()
{
	if (m_TempString.size() == 0)
	{
		m_TempString = "0";
	}
	float tempValue{ std::stof(m_TempString) };
	m_TempString.clear();

	if (tempValue > 360)
	{
		tempValue = 360;
	}

	m_pIsotoxalStarPolygon->ChangeAngleOffset(tempValue);
}

void Game::ChangeAngleOffsetIsotoxalColor(bool isBeingEdited)
{
	delete m_TextAngleOffsetIsotoxal;

	if (isBeingEdited == true)
	{
		m_TextAngleOffsetIsotoxal = new Texture(m_TextAngleOffsetIsotoxalBase + m_TempString, m_FontPath, m_SettingsTextSize, m_ChangingTextColor);
	}
	else
	{
		m_TextAngleOffsetIsotoxal = new Texture(m_TextAngleOffsetIsotoxalBase + std::to_string(int(m_pIsotoxalStarPolygon->GetAngleOffset())), m_FontPath, m_SettingsTextSize, m_TextColor);
	}
}

Game::ChangableColor::ChangableColor()
	:ChangableColor{ 0.0f, 0.0f, 0.0f}
{
}

Game::ChangableColor::ChangableColor(float r, float g, float b)
	: r{ r }
	, g{ g }
	, b{ b }
{
}
