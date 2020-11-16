#include "dfs.h"
int main() {
	GraphList g;
	createGraph(&g);
	DFSTraverse(&g);
	return 0;
}