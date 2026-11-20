#pragma once

#include "PxPhysicsAPI.h"
#include "Scene.h"
#include "RenderUtils.hpp"
#include "Model.h"
#include "Player.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <array>
#include <random>

using namespace std;

class LayoutGenerator
{
public:
	LayoutGenerator(Player* player);
	~LayoutGenerator();

	void update(double dt);
	void addTileType(std::string model);
private:
	void spawnTile(int tileIndex);
	void deleteTile();

	//struct TerrainSegment
	//{
	//	GameObject* g;
	//	//vector<physx::PxTransform> transforms;
	//	//vector<RenderItem*> renderItems;
	//	bool canBeDeleted = false;
	//};

	float zSpawn = 0;
	float delTileTime = 3;
	float tileLength = 30.0f;
	float spawnMargin = 40.0f;
	float deleteMargin = -15.0f;
	float tileSize = 5.0f;

	int nMaxActiveTiles = 5;
	vector<string> totalTiles;
	deque<GameObject*> activeTiles;

	int currTiles = 0;

	Player* _player;
	
};