#include "PreCompile.h"
#include "MapGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCoreDebug.h>
#include <EngineCore/EngineAPICore.h>
#include "Background.h"
#include "Map.h"


AMapGameMode::AMapGameMode()
{
}

AMapGameMode::~AMapGameMode()
{
}

void AMapGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SetCameraToMainPawn(false);

	Scale = { 33, 18 };
	FVector2D CenterPivot;
	CenterPivot.X = (1280 - (Scale.X * 36)) / 2;
	CenterPivot.Y = (720 - (Scale.Y * 36)) / 2;

	ABackground* BG = GetWorld()->SpawnActor<ABackground>();
	AMap* WorldMap = GetWorld()->SpawnActor<AMap>();
	SelectBox = GetWorld()->SpawnActor<ASelectBox>();
	SelectBox->SetActorLocation(CenterPivot + FVector2D((9 * 36) + 18, (15 * 36) + 18));

	TileMap = GetWorld()->SpawnActor<ATileMap>();
	TileMap->Create(Scale, { 36, 36 });
	TileMap->SetActorLocation(CenterPivot);

	{
		// NumBG
		TileMap->SetTile("WorldMapNumberBack36.png", { 9, 15 }, 0, static_cast<int>(EMapOrder::BGCIRCLE), ERenderOrder::LOWER);
		TileMap->SetTile("WorldMapNumberBack36.png", { 10, 13 }, 0, static_cast<int>(EMapOrder::BGCIRCLE), ERenderOrder::LOWER);
		TileMap->SetTile("WorldMapNumberBack36.png", { 11, 13 }, 0, static_cast<int>(EMapOrder::BGCIRCLE), ERenderOrder::LOWER);
		TileMap->SetTile("WorldMapNumberBack36.png", { 10, 12 }, 0, static_cast<int>(EMapOrder::BGCIRCLE), ERenderOrder::LOWER);
		TileMap->SetTile("WorldMapNumberBack36.png", { 11, 12 }, 0, static_cast<int>(EMapOrder::BGCIRCLE), ERenderOrder::LOWER);
		TileMap->SetTile("WorldMapNumberBack36.png", { 12, 12 }, 0, static_cast<int>(EMapOrder::BGCIRCLE), ERenderOrder::LOWER);
		TileMap->SetTile("WorldMapNumberBack36.png", { 10, 11 }, 0, static_cast<int>(EMapOrder::BGCIRCLE), ERenderOrder::LOWER);
		TileMap->SetTile("WorldMapNumberBack36.png", { 11, 11 }, 0, static_cast<int>(EMapOrder::BGCIRCLE), ERenderOrder::LOWER);
		
		// Line
		TileMap->SetTile("Line.png", { 10, 15 }, 8, static_cast<int>(EMapOrder::LINE), ERenderOrder::LOWER);
		TileMap->SetTile("Line.png", { 10, 14 }, 12, static_cast<int>(EMapOrder::LINE), ERenderOrder::LOWER);
		TileMap->SetTile("Line.png", { 12, 13 }, 8, static_cast<int>(EMapOrder::LINE), ERenderOrder::LOWER);
		TileMap->SetTile("Line.png", { 12, 11 }, 14, static_cast<int>(EMapOrder::LINE), ERenderOrder::LOWER);
		
		// SelectBox
		// TileMap->SetTile("SelectBox.png", { 9, 15 }, { 0, 0 }, { 54, 54 }, 0, static_cast<int>(EMapOrder::SELECTBOX), ERenderOrder::UPPER);
		
		// NUMBER
		TileMap->SetTile("LevelNum.png", { 9, 15 }, 0, static_cast<int>(EMapOrder::NUMBER), ERenderOrder::UPPER);
		TileMap->SetTile("LevelNum.png", { 10, 13 }, 1, static_cast<int>(EMapOrder::NUMBER), ERenderOrder::UPPER);
		TileMap->SetTile("LevelNum.png", { 10, 12 }, 2, static_cast<int>(EMapOrder::NUMBER), ERenderOrder::UPPER);
		TileMap->SetTile("LevelNum.png", { 11, 13 }, 3, static_cast<int>(EMapOrder::NUMBER), ERenderOrder::UPPER);
		TileMap->SetTile("LevelNum.png", { 11, 12 }, 4, static_cast<int>(EMapOrder::NUMBER), ERenderOrder::UPPER);
		TileMap->SetTile("LevelNum.png", { 10, 11 }, 5, static_cast<int>(EMapOrder::NUMBER), ERenderOrder::UPPER);
		TileMap->SetTile("LevelNum.png", { 12, 12 }, 6, static_cast<int>(EMapOrder::NUMBER), ERenderOrder::UPPER);
		TileMap->SetTile("LevelNum.png", { 11, 11 }, 7, static_cast<int>(EMapOrder::NUMBER), ERenderOrder::UPPER);
	}
}

void AMapGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	FIntPoint SelectBoxIndex = TileMap->LocationToIndex(SelectBox->GetActorLocation());
	UEngineDebug::CoreOutPutString("SelectBoxIndex : " + SelectBoxIndex.ToString());

	BoxMove(_DeltaTime);
}

void AMapGameMode::BoxMove(float _DeltaTime)
{
	ActionTime += _DeltaTime * 10.0f;

	if (true == UEngineInput::GetInst().IsDown('W') || true == UEngineInput::GetInst().IsDown(VK_UP))
	{
		FVector2D StartPos = SelectBox->GetActorLocation();
		FVector2D EndPos = StartPos + (FVector2D::UP * 36);

		if (false == IsMovable(EndPos))
		{
			return;
		}

		FVector2D CurPos = FVector2D::Lerp(StartPos, EndPos, ActionTime);
		SelectBox->SetActorLocation(CurPos);
	}

	if (true == UEngineInput::GetInst().IsDown('S') || true == UEngineInput::GetInst().IsDown(VK_DOWN))
	{
		FVector2D StartPos = SelectBox->GetActorLocation();
		FVector2D EndPos = StartPos + (FVector2D::DOWN * 36);

		if (false == IsMovable(EndPos))
		{
			return;
		}

		FVector2D CurPos = FVector2D::Lerp(StartPos, EndPos, ActionTime);
		SelectBox->SetActorLocation(CurPos);
	}

	if (true == UEngineInput::GetInst().IsDown('A') || true == UEngineInput::GetInst().IsDown(VK_LEFT))
	{
		FVector2D StartPos = SelectBox->GetActorLocation();
		FVector2D EndPos = StartPos + (FVector2D::LEFT * 36);

		if (false == IsMovable(EndPos))
		{
			return;
		}

		FVector2D CurPos = FVector2D::Lerp(StartPos, EndPos, ActionTime);
		SelectBox->SetActorLocation(CurPos);
	}

	if (true == UEngineInput::GetInst().IsDown('D') || true == UEngineInput::GetInst().IsDown(VK_RIGHT))
	{
		FVector2D StartPos = SelectBox->GetActorLocation();
		FVector2D EndPos = StartPos + (FVector2D::RIGHT * 36);

		if (false == IsMovable(EndPos))
		{
			return;
		}

		FVector2D CurPos = FVector2D::Lerp(StartPos, EndPos, ActionTime);
		SelectBox->SetActorLocation(CurPos);
	}
}

bool AMapGameMode::IsMovable(FVector2D _NextPos)
{
	FIntPoint NextIndex = TileMap->LocationToIndex(_NextPos - FVector2D(36, 36));

	for (int i = 0; i < static_cast<int>(EMapOrder::MAX); i++)
	{
		Tile* NextTile = TileMap->GetTileRef(NextIndex, i);

		if (nullptr == NextTile)
		{
			continue;
		}

		if (2 == NextTile->FloorOrder || 3 == NextTile->FloorOrder)
		{
			return true;
		}
	}

	return false;
}