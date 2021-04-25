#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <deque>

#include "Room/Room.h"

class LUDUMDARE48_API FNode {
public:
	FNode(ARoom* Value);

	ARoom* Value;
	std::vector<FNode*> Edges;
	bool Visited;
	FNode* BfsParent;
	
	bool IsObstacle;
	
	void Connect(FNode* V);

};

class LUDUMDARE48_API FGraph
{
public:
	FGraph(FNode* Root);

	std::vector<FNode*> Nodes;

	static void Connect(FNode* U, FNode* V);
	void AddNode(FNode* Node);
	void SetAllVisitedFlag(bool Flag);

	FNode* Bfs(FNode* Start, ARoom* Value);
	std::deque<FNode*> PathBfs(FNode* Start, FNode* Goal);
};
