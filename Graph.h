#include <string>
#include <limits>
#include <cstdlib>
#include <fstream>
#include "List.h"

struct Graph
{
	void fileRead(string fileName)
	{
		ifstream file;
		file.open(fileName, ios::in);
		if (!file.is_open())
		{
			throw invalid_argument("Error, file doesn't open");
		}
		file.unsetf(ios::skipws);
		while (!file.eof())
		{
			string temp;
			getline(file, temp, ' ');
			string arrival = temp;

			if (contains(arrival) == false)
				NewVertex(arrival);
			temp.clear();

			getline(file, temp, ' ');
			string T = temp;

			if (contains(T) == false)
				NewVertex(T);
			temp.clear();

			getline(file, temp, '\n');

			float width = stof(temp);
			newEdge(arrival, T, width);
		}
	}

	void NewVertex(string name)
	{
		for (unsigned int i = 0; i < graph.GetSize(); ++i)
			if (graph[i]->name == name)
				throw exception("Attempt to add a new vertex with the same name");
		Node* temp = new Node(name);
		graph.push_back(temp);
	}

	void newEdge(string nameDeparture, string nameDestination, float Width)
	{
		newEdge(stringToInt(nameDeparture), stringToInt(nameDestination), Width);
	}

	void print()
	{
		for (unsigned int i = 0; i < graph.GetSize(); ++i)
		{
			if (graph[i]->name != "")
				cout << graph[i]->name;
			else
				cout << i;
			cout << " - ";
			for (unsigned int j = 0; j < graph[i]->edges.GetSize(); ++j)
			{
				if (graph[i]->edges[j]->T->name != "")
					cout << graph[i]->edges[j]->T->name;
				else
					cout << graph[i]->edges[j]->T;

				cout << " <" << graph[i]->edges[j]->Width << ">";
				if (j < graph[i]->edges.GetSize() - 1)
					cout << ", ";
				else
					cout << endl;
			}
			cout << endl;
		}
	}

	bool contains(string name)
	{
		for (unsigned int i = 0; i < graph.GetSize(); ++i)
			if (graph[i]->name == name)
				return true;
		return false;
	}

	float FF(string sourceName, string outletName)
	{
		return FF(stringToInt(sourceName), stringToInt(outletName));
	}
	float FF(unsigned int sourceIndex, unsigned int outletIndex)
	{
		struct rout
		{
			List<Node*> path;
			float flow;

			rout(Node& finalNode)
			{
				this->flow = finalNode.flow;
				Node* current = &finalNode;
				while (current != nullptr)
				{
					path.push_front(current);
					current = current->S;
				}
			}
		};

		if (sourceIndex >= graph.GetSize() || outletIndex >= graph.GetSize())
			throw exception("Out of range");

		Node* source = graph[sourceIndex];
		Node* outlet = graph[outletIndex];

		List<rout*> pathways;

		source->flow = numeric_limits<float>::infinity();

		Node* current = source;
		while (1)
		{
			Node possible;
			for (unsigned int i = 0; i < current->edges.GetSize(); ++i)
			{
				if (current->edges[i]->remain > 0)
					if (current->edges[i]->T->flow == NULL)
					{
						Node::Edge* p = current->edges[i];
						possible.addEdge(p);
					}
			}
			if (possible.edges.GetSize() == 0)
			{
				if (current == source)
				{
					float total = 0;
					for (unsigned int i = 0; i < pathways.GetSize(); ++i)
					{
						total += pathways[i]->flow;
						for (unsigned int j = 0; j < pathways[i]->path.GetSize(); ++j)
						{
							cout << pathways[i]->path[j]->name;
							if (j != pathways[i]->path.GetSize() - 1)
								cout << " - ";
						}
						cout << " (" << pathways[i]->flow << ")" << endl;
					}
					source->flow = 0;

					for (unsigned int i = 0; i < graph.GetSize(); ++i)
						for (unsigned int j = 0; j < graph[i]->edges.GetSize(); ++j)
							graph[i]->edges[j]->remain = graph[i]->edges[j]->Width;

					return total;
				}
				else
				{
					Node* temp = current->S;
					for (unsigned int i = 0; i < temp->edges.GetSize(); ++i)
						if (temp->edges[i]->T == current)
						{
							temp->edges[i]->remain = 0;
							break;
						}
					current->S = nullptr;
					current->flow = NULL;
					current = temp;
					continue;
				}
			}
			else
			{
				unsigned int max = 0;
				for (unsigned int i = 0; i < possible.edges.GetSize(); ++i)
					if (possible.edges[i]->remain > possible.edges[max]->remain)
						max = i;

				Node* next = possible.edges[max]->T;
				next->S = current;
				if (current->flow > possible.edges[max]->remain)
				{
					next->flow = possible.edges[max]->remain;
					possible.edges[max]->remain = 0;
				}
				else
				{
					next->flow = current->flow;
					possible.edges[max]->remain -= current->flow;
				}

				if (next == outlet)
				{
					pathways.push_back(new rout(*next));
					Node* p = next;
					while (p != source)
					{
						for (unsigned int i = 0; i < p->S->edges.GetSize(); ++i)
							if (p->S->edges[i]->T == p)
							{
								p->S->edges[i]->remain += (p->flow - next->flow);
								if (p != next)
									p->flow = NULL;
								Node* temp = p->S;
								p->S = nullptr;
								p = temp;
								break;
							}
					}
					next->flow = NULL;
					current = source;
				}
				else
					current = next;
			}
		}
	}

private:

	void newEdge(unsigned int departure, unsigned int T, float Width)
	{
		if (departure > graph.GetSize() - 1 || T > graph.GetSize() - 1)
			throw exception("Out of range");
		Node::Edge* temp = new Node::Edge(graph[T], Width);
		graph[departure]->addEdge(temp);
	}

	unsigned int stringToInt(string input)
	{
		unsigned int answer;
		bool success = false;
		for (unsigned int i = 0; i < graph.GetSize(); ++i)
		{
			if (graph[i]->name == input)
			{
				answer = i;
				success = true;
				break;
			}
		}

		if (success == false)
			throw exception("Couldn't find such vertex");
		return answer;
	}
	struct Node
	{
		struct Edge
		{
			Node* T;
			float Width;
			float remain;

			Edge(Node* T = nullptr, unsigned int Width = NULL)
			{
				this->T = T;
				this->Width = Width;
				this->remain = Width;
			}
		};
		void addEdge(Edge* newEdge)
		{
			this->edges.push_back(newEdge);
		};
		string name;
		List<Edge*> edges;

		Node* S;
		float flow;

		Node(string name = "")
		{
			this->name = name;
			this->S = nullptr;
			this->flow = 0;
		}
	};

	List<Node*> graph;
};