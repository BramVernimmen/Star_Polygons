#pragma once
#include "Texture.h"
#include "RegularStarPolygon.h"
#include "IsotoxalStarPolygon.h"

class Game final
{
public:
	struct ChangableColor
	{
		ChangableColor();
		explicit ChangableColor(float r, float g, float b);

		float r;
		float g;
		float b;
	};
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	Point2f m_MousePos;
	bool m_IsChangingAnElement;

	const int m_TitleTextSize;
	const int m_SettingsTextSize;

	Color4f m_TextColor;
	Color4f m_ChangingTextColor;

	Rectf m_TitleBox;
	Rectf m_SettingsBox;
	Color4f m_BoxColor;
	Rectf m_StarsBox;

	Color4f m_StarsBoxColor;
	ChangableColor m_StarsBoxChangableColor;
	Color4f m_StarsColor;
	ChangableColor m_StarsChangableColor;

	const std::string m_FontPath;

	Texture* m_RegularStarPolygonText;
	Point2f m_RegularStarPolygonPosition;
	Texture* m_IsotoxalStarPolygonText;
	Point2f m_IsotoxalStarPolygonPosition;


	Point2f m_CenterRegularPosition;
	RegularStarPolygon* m_pRegularStarPolygon;

	Point2f m_CenterIsotoxalPosition;
	IsotoxalStarPolygon* m_pIsotoxalStarPolygon;


	const std::string m_TextNumberOfVerticesRegularBase;
	const std::string m_TextDensityBase;
	const std::string m_TextAngleOffsetRegularBase;

	Texture* m_TextNumberOfVerticesRegular;
	Point2f m_TextNumberOfVerticesRegularPosition;
	Texture* m_TextDensity;
	Point2f m_TextDensityPosition;
	Texture* m_TextAngleOffsetRegular;
	Point2f m_TextAngleOffsetRegularPosition;

	int m_ColorChangeIndex{0};


	const std::string m_TextBGColorBase;

	Texture* m_TextBGColor;
	Texture* m_BGRedValue;
	Texture* m_BGGreenValue;
	Texture* m_BGBlueValue;
	Point2f m_TextBGColorPosition;
	const Rectf m_TextBGColorHitBox;
	bool m_IsChangingBGColor;
	bool m_HasBGRedChanged{ false };
	bool m_HasBGGreenChanged{ false };
	bool m_HasBGBlueChanged{false};
	Point2f m_BGRedValuePositionIndex{200.f , 0.f};
	Point2f m_BGGreenValuePositionIndex{280.f , 0.f};
	Point2f m_BGBlueValuePositionIndex{360.f , 0.f};


	const std::string m_TextStarColorBase;

	Texture* m_TextStarColor;
	Texture* m_StarRedValue;
	Texture* m_StarGreenValue;
	Texture* m_StarBlueValue;
	Point2f m_TextStarColorPosition;
	const Rectf m_TextStarColorHitBox;
	bool m_IsChangingStarColor;
	bool m_HasStarRedChanged{ false };
	bool m_HasStarGreenChanged{ false };
	bool m_HasStarBlueChanged{false};
	Point2f m_StarRedValuePositionIndex{825.f , 0.f};
	Point2f m_StarGreenValuePositionIndex{905.f , 0.f};
	Point2f m_StarBlueValuePositionIndex{985.f , 0.f};



	float m_IndexWidth{ 20.f };

	void ChangeBGColorValue(int value);
	void ChangeBGColorTextColor(bool isBeingEdited);

	void ChangeStarColorValue(int value);
	void ChangeStarColorTextColor(bool isBeingEdited);


	std::string m_TempString{};


	const Rectf m_NumberVerticesRegularHitBox;
	bool m_IsChangingNumberOfVerticesRegular{false};

	void ChangeNumberOfVerticesRegular();
	void ChangeNumberOfVerticesRegularColor(bool isBeingEdited);	
	
	const Rectf m_DensityHitBox;
	bool m_IsChangingDensity{false};

	void ChangeDensity();
	void ChangeDensityColor(bool isBeingEdited);

	const Rectf m_AngleOffsetRegularHitBox;
	bool m_IsChangingAngleOffsetRegular{false};

	void ChangeAngleOffsetRegular();
	void ChangeAngleOffsetRegularColor(bool isBeingEdited);



	const Rectf m_NumberVerticesIsotoxalHitBox;
	bool m_IsChangingNumberOfVerticesIsotoxal{false};

	void ChangeNumberOfVerticesIsotoxal();
	void ChangeNumberOfVerticesIsotoxalColor(bool isBeingEdited);
	
	const Rectf m_AlphaHitBox;
	bool m_IsChangingAlpha{false};

	void ChangeAlpha();
	void ChangeAlphaColor(bool isBeingEdited);

	const Rectf m_AngleOffsetIsotoxalHitBox;
	bool m_IsChangingAngleOffsetIsotoxal{false};

	void ChangeAngleOffsetIsotoxal();
	void ChangeAngleOffsetIsotoxalColor(bool isBeingEdited);


	const std::string m_TextNumberOfVerticesIsotoxalBase;
	const std::string m_TextAlphaBase;
	const std::string m_TextAngleOffsetIsotoxalBase;

	Texture* m_TextNumberOfVerticesIsotoxal;
	Point2f m_TextNumberOfVerticesIsotoxalPosition;
	Texture* m_TextAlpha;
	Point2f m_TextAlphaPosition;
	Texture* m_TextAngleOffsetIsotoxal;
	Point2f m_TextAngleOffsetIsotoxalPosition;

};