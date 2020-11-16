#include <stdio.h>
#include <string.h>

int main(void) {
	char s[1000], stack[1000];
	scanf("%s", s);

	int len = strlen(s), i = 0, top = 0, is_match = 1;
	for (i = 0; i < len; i++) {
		if (s[i] == '[' || s[i] == '(' || s[i]=='{') {
			stack[top++] = s[i];
		}
		else if (s[i] == ']') {
			if (top > 0 && stack[top - 1] == '[') {
				top--;
			}
			else {
				is_match = 0;
				break;
			}
		}
		else if (s[i] == '}') {
			if (top > 0 && stack[top - 1] == '{') {
				top--;
			}
			else {
				is_match = 0;
				break;
			}
		}
		else if (s[i] == ')') {
			if (top > 0 && stack[top - 1] == '(') {
				top--;
			}
			else {
				is_match = 0;
				break;
			}
		}
	}


	if (is_match && top == 0) {
		printf("Yes\n");
	}
	else {
		printf("No\n");
	}
	return 0;
}