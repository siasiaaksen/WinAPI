#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineSerializer.h>

#include "ContentsEnum.h"


class Tile : public AActor, public ISerializObject
{
public:
	USpriteRenderer* SpriteRenderer;
	bool IsMove = true;
	int FloorOrder = -1;

	FVector2D Scale;
	FVector2D Pivot;
	FVector2D SpriteLocation;

	std::string SpriteName;
	int SpriteIndex;

	EDeathState DeathState = EDeathState::NONE;
	EMoveType MoveType = EMoveType::NONE;
	EStateType StateType = EStateType::NONE;
	ELogicType FLogicType = ELogicType::NONE;
	EVLogicType SLogicType = EVLogicType::NONE;
	ELogicType TLogicType = ELogicType::NONE;

	void Serialize(UEngineSerializer& _Ser)
	{
		std::string SpriteName;
		if (nullptr != SpriteRenderer)
		{
			SpriteName = SpriteRenderer->GetCurSpriteName();
		}
		_Ser << SpriteName;
		_Ser << IsMove;
		_Ser << FloorOrder;
		_Ser << Scale;
		_Ser << Pivot;
		_Ser << SpriteIndex;
	}

	void DeSerialize(UEngineSerializer& _Ser)
	{
		std::string SpriteName;
		_Ser >> SpriteName;
		_Ser >> IsMove;
		_Ser >> FloorOrder;
		_Ser >> Scale;
		_Ser >> Pivot;
		_Ser >> SpriteIndex;
	}
};

class History
{
public:
	Tile* Tile;
	FIntPoint Prev;
	FIntPoint Next;
};

class ATileMap : public AActor, public ISerializObject
{
public:
	ATileMap();
	~ATileMap();

	ATileMap(const ATileMap& _Other) = delete;
	ATileMap(ATileMap&& _Other) noexcept = delete;
	ATileMap& operator=(const ATileMap& _Other) = delete;
	ATileMap& operator=(ATileMap&& _Other) noexcept = delete;

	void Create(FIntPoint _Count, FVector2D _TileSize);

	void SetTileLocation(std::string_view _Sprite, FVector2D _Location, int _SpriteIndex, int _FloorOrder, ERenderOrder _Order, ELogicType _FLogicType, EVLogicType _SLogicType, ELogicType _TLogicType);

	void SetTile(std::string_view _Sprite, FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex, int _FloorOrder, ERenderOrder _Order, ELogicType _FLogicType = ELogicType::NONE, EVLogicType _SLogicType = EVLogicType::NONE, ELogicType _TLogicType = ELogicType::NONE);
	void SetTile(std::string_view _Sprite, FIntPoint _Index, int _SpriteIndex, int _FloorOrder, ERenderOrder _Order, ELogicType _FLogicType = ELogicType::NONE, EVLogicType _SLogicType = EVLogicType::NONE, ELogicType _TLogicTyp = ELogicType::NONE);

	std::vector<FIntPoint> FindMoveTile(ELogicType _FLogicType);
	std::vector<FIntPoint> FindMoveTile();

	void AllTileMoveCheck(FIntPoint _MoveIndex);

	void TileMove(FIntPoint _CurIndex, FIntPoint _MoveIndex);
	bool TileMoveCheck(FIntPoint _NextIndex, FIntPoint _MoveIndex);

	// 다음 인덱스의 스프라이트가 있어? 모든 FloorOrder 체크
	bool IsVoid(FIntPoint _NextIndex);

	Tile* GetTileRef(FIntPoint _Index, int _FloorOrder);
	Tile* GetTileRef(FVector2D _Location, int _FloorOrder);

	FVector2D IndexToTileLocation(FIntPoint _Index);
	FIntPoint LocationToIndex(FVector2D _Location);

	bool IsIndexOver(FIntPoint _Index);

	void Serialize(UEngineSerializer& _Ser);
	void DeSerialize(UEngineSerializer& _Ser);

	void MoveTileTypeReset();
	void MoveTileStateReset();

	void ChangeMoveMode(ELogicType _FLogicType, EMoveType _MoveType);
	void ChangeStateMode(ELogicType _FLogicType, EStateType _StateType);

	void DeathTileToAlive();
	void DeathTile();

	void Action(float _DeltaTime);
	void Undo(float _DeltaTime);

	bool IsActionEnd()
	{
		return ActionTime >= 1.0f;
	}

protected:

private:
	FIntPoint TileCount;
	FVector2D TileSize;

	////     Y         X            층
	std::vector<std::vector<std::map<int, Tile*>>> AllTiles;

	std::list<History> CurHistorys;

	std::list<std::list<History>> Historys;

	std::list<History> LastHistorys;

	float ActionTime = 0.0f;
};

