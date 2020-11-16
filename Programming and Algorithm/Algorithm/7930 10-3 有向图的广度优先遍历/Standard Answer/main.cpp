#include "bfs.h"
int main() {
	GraphList g;
	createGraph(&g);
	BFSTraverse(&g);
	return 0;
}