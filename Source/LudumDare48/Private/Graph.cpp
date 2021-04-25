#include "Graph.h"

#include <queue>

FNode::FNode(ARoom* Value) {
	this->Value = Value;
	this->IsObstacle = Value->Properties().IsObstacle;

	Visited = false;
	BfsParent = nullptr;
}

void FNode::Connect(FNode* V) {
	for (auto& Node : Edges) {
		if (Node == V)
			return;
	}

	Edges.push_back(V);
}


FGraph::FGraph(FNode* Root) {
	Nodes.push_back(Root);
}

void FGraph::Connect(FNode* U, FNode* V) {
	U->Connect(V);
	V->Connect(U);
}

void FGraph::AddNode(FNode* Node) {
	Nodes.push_back(Node);
}

void FGraph::SetAllVisitedFlag(const bool Flag) {
	for (auto& N : Nodes) {
		N->Visited = Flag;
		N->BfsParent = nullptr;
	}
}

FNode* FGraph::Bfs(FNode* Start, ARoom* Value) {
	SetAllVisitedFlag(false);

	std::queue<FNode*> Q;

	Q.push(Start);
	Start->Visited = true;

	while (Q.empty() != true) {
		FNode* Current = Q.front();
		Q.pop();

		for (auto& Neighbour : Current->Edges) {
			if (Neighbour->Visited != true) {
				Q.push(Neighbour);
				Neighbour->Visited = true;
			}

			if (Neighbour->Value == Value) {
				return Neighbour;
			}
		}
	}

	return nullptr;
}

std::deque<FNode*> FGraph::PathBfs(FNode* Start, FNode* Goal) {

	if(Start->IsObstacle == true || Goal->IsObstacle == true) return {};
 
	SetAllVisitedFlag(false);
	std::deque<FNode*> Q;
	std::deque<FNode*> Path;

	Q.push_back(Start);

	while (Q.empty() != true) {
		FNode* Parent = Q.front();
		Q.pop_front();

		if (Parent == Goal) {
			Path.push_back(Parent);
			FNode* Temp = Parent->BfsParent;
			while (Temp != nullptr) {
				Path.push_back(Temp);
				Temp = Temp->BfsParent;
			}
			return Path;
		}
		
		for (auto& Child : Parent->Edges) {
			if (Child->Visited || Child->IsObstacle)
				continue;

			bool Found = false;
			for (auto& Node : Q) {
				if (Node == Child) {
					Found = true;
					break;
				}
			}
			if (!Found) {
				Child->BfsParent = Parent;
				Q.push_back(Child);
			}
		}

		Parent->Visited = true;
	}

	return Path;
}