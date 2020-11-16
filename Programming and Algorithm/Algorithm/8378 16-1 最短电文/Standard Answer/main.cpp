#include <iostream>
using namespace std;
#define MAXNODE 1000
struct HTNode {
	char data;
	double weight;
	int parent;
	int lchild;
	int rchild;
}ht[2 * MAXNODE - 1];
struct HCode {
	char cd[MAXNODE];
	int start;
}hcd[MAXNODE];
char str[MAXNODE];
void CreateHT(HTNode ht[], int n)
{
	int i, j, k, lnode, rnode;
	double min1, min2;
	for (i = 0; i < 2 * n - 1; i++)
		ht[i].parent = ht[i].lchild = ht[i].rchild = -1;
	for (i = n; i < 2 * n - 1; i++) {
		min1 = min2 = 9999999;
		lnode = rnode = -1;
		for (k = 0; k <= i - 1; k++)
			if (ht[k].parent == -1) {
				if (ht[k].weight < min1) {
					min2 = min1; rnode = lnode;
					min1 = ht[k].weight; lnode = k;
				}
				else if (ht[k].weight < min2) {
					min2 = ht[k].weight; rnode = k;
				}
			}
		ht[i].weight = ht[lnode].weight + ht[rnode].weight;
		ht[i].lchild = lnode; ht[i].rchild = rnode;
		ht[lnode].parent = i; ht[rnode].parent = i;
	}
}
void CreateHCode(HTNode ht[], HCode hcd[], int n)
{
	int i, f, c;
	HCode hc;
	for (i = 0; i < n; i++) {    
		hc.start = n; c = i;
		f = ht[i].parent;
		while (f != -1) {
			if (ht[f].lchild == c)
				hc.cd[hc.start--] = '0';
			else
				hc.cd[hc.start--] = '1';
			c = f; f = ht[f].parent;
		}
		hc.start++;
		hcd[i] = hc;
	}
}
int main()
{
	cin >> str;
	int len = 0;
	for (int i = 0; str[i]; i++) {

		int j;
		for (j = 0; j < len; j++)
			if (str[i] == ht[j].data)
				break;
		if (j < len) continue;
		int count = 0;
		for (j = 0; str[j]; j++) {
			if (str[i] == str[j])
				count++;
		}
		ht[len].data = str[i];
		ht[len++].weight = count;
	}

	CreateHT(ht, len);
	CreateHCode(ht, hcd, len);
	int wpl = 0;
	for (int i = 0; i < len; i++) {
		wpl += (len - hcd[i].start + 1)*ht[i].weight;
	}
	cout << wpl;
	return 0;
}