#include "LayoutGenerator.h"

LayoutGenerator::LayoutGenerator(Player* player) : _player(player)
{
	addTileType("Models/TestBlock.obj");
	addTileType("Models/TestTile2.obj");		
	for (int i = 1; i <= nMaxActiveTiles; ++i)
	{
		spawnTile(rand() % totalTiles.size()); // entre 0 y nMaxActiveTiles-1		
	}
}

void LayoutGenerator::update(double dt)
{
	if ((_player->getTransform().p.z + spawnMargin) > zSpawn - (tileLength))
	{
		for (int i = 0; i < (activeTiles.size() - nMaxActiveTiles); ++i)
			spawnTile(rand() % totalTiles.size()); // entre 0 y nMaxActiveTiles-1		
	}

	while (!activeTiles.empty())
	{
		GameObject* tile = activeTiles.front();

		if (!tile)
		{
			activeTiles.erase(activeTiles.begin());
			continue;
		}

		if (tile->getTransform().p.z + tileLength + deleteMargin < _player->getTransform().p.z)
		{
			deleteTile();
		}
		else
		{
			break;
		}
	}
	if (int(_player->getSpeed()) % nMaxActiveTiles > 5)
	{
		nMaxActiveTiles++;
	}
}

void LayoutGenerator::addTileType(std::string model)
{
	totalTiles.push_back(model);
}

void LayoutGenerator::spawnTile(int tileIndex)
{
	activeTiles.push_back(new GameObject(totalTiles[tileIndex],
		{ 0, 10, zSpawn },
		-90,
		tileSize));

	zSpawn += tileLength;
}

void LayoutGenerator::deleteTile()
{
	if (activeTiles.empty())
		return;

	delete activeTiles.front();
	activeTiles.erase(activeTiles.begin());
}

LayoutGenerator::~LayoutGenerator()
{
	for (GameObject* g : activeTiles)
	{
		if (g != nullptr)
			delete g;
	}
	activeTiles.clear();
}