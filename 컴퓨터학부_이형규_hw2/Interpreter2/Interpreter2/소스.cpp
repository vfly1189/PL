#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <vector>
#include <iostream>
#include <ctype.h>
#include <string>

#define COM_CNT 200
#define BUF_SIZE 1024
#define TOK_CNT 200

typedef struct Node
{
	char* name;
	struct Node* left;
	struct Node* right;
}Node;

using namespace std;

void start_interpreter();
void file_load();
void interactive_mode();
char* ltrim(char* command);
char* rtrim(char* command);
void make_tokens(char* command, char** tokens, int *index);
Node* make_tree(char** tokens, int* index);
Node* create_node(char* name);
void print_tree(Node* move);
bool is_character(char a);
bool is_number(char a);
bool is_integer(char* a);
bool check_brackets(char **tokens, int* index);

bool error_flag = false;
stack<int> st;

int main()
{
	start_interpreter();
}

void start_interpreter()
{
	int select;

	while (1)
	{
		printf("==================================\n");
		printf("1. File Load\n2. Interactive Mode\n3. Exit\n");
		printf("==================================\n");

		printf("메뉴를 선택하세요 >> ");
		scanf("%d", &select);

		if (select == 1)
		{
			file_load();
		}
		else if (select == 2)
		{
			interactive_mode();
		}
		else if (select == 3)
		{
			printf("\n프로그램을 종료합니다.");
			exit(0);
		}
		else
		{
			printf("올바른 번호를 입력하세요\n");
		}
	}
}

void file_load()
{
	FILE* fp;
	int index = 0;
	char** commands = (char**)calloc(200, sizeof(char *));
	for (int i = 0; i < 200; i++)
		commands[i] = (char*)calloc(BUF_SIZE, sizeof(char));
	char* fname = (char*)calloc(BUF_SIZE, sizeof(char));

	printf("\n파일명을 입력하세요 >> ");
	scanf("%s", fname);

	if ((fp = fopen(fname, "r")) == NULL)
	{
		fprintf(stderr, "open error for %s\n", fname);
		return;
	}

	printf("\n파일 내용은\n");
	printf("----------------------------------\n");

	while (1)
	{
		char tmp = fgetc(fp);
		if (tmp == EOF) break;

		if (tmp == '\n')
		{
			index++;
		}
		else commands[index][strlen(commands[index])] = tmp;
		
	}
	for (int i = 0; i <= index; i++) printf("%s\n", commands[i]);
	printf("----------------------------------\n입니다.\n\n");

	for (int i = 0; i < index + 1; i++)
	{
		int idx = 0;
		int idx2 = 0;
		char** tokens = (char**)calloc(TOK_CNT, sizeof(char*));
		for (int i = 0; i < TOK_CNT; i++)
			tokens[i] = (char*)calloc(BUF_SIZE, sizeof(char));
		error_flag = false;

		commands[i] = ltrim(rtrim(commands[i]));
		make_tokens(commands[i], tokens, &idx);
		if (error_flag) continue;

		int index3 = 0;
		if (check_brackets(tokens, &index3) == 0)
			continue;

		Node* root = make_tree(tokens, &idx2);

		print_tree(root);

		printf("결과: %d\n", st.top());
	}
	printf("\n");
	while (!st.empty()) st.pop();
}

void interactive_mode()
{
	int index = 0;
	int index2 = 0;
	char* pre = (char*)calloc(BUF_SIZE, sizeof(char));
	char* post = (char*)calloc(BUF_SIZE, sizeof(char));
	char** tokens = (char**)calloc(TOK_CNT, sizeof(char*));
	for (int i = 0; i < TOK_CNT; i++)
		tokens[i] = (char*)calloc(BUF_SIZE, sizeof(char));
	
	printf("\n문장을 입력하세요.\n");
	printf("----------------------------------\n");

	getc(stdin);
	fgets(pre, BUF_SIZE, stdin);
	pre[strlen(pre) - 1] = '\0';


	pre = ltrim(rtrim(pre));
	
	make_tokens(pre, tokens, &index);
	if (error_flag) return;
	int index3 = 0;
	if (check_brackets(tokens, &index3) == 0)
		return;

	Node *root = make_tree(tokens, &index2);
	
	print_tree(root);

	printf("\n----------------------------------\n");
	printf("결과 : %d\n", st.top());
	printf("----------------------------------\n");

	while (!st.empty()) st.pop();
}

void print_tree(Node* move)
{
	if (move == NULL) return;

	print_tree(move->left);
	print_tree(move->right);
	
	if (!strcmp(move->name, "MINUS"))
	{
		int operand1 = st.top(); st.pop();
		int operand2 = st.top(); st.pop();

		st.push(operand2 - operand1);
	}
	else if (!strcmp(move->name, "IF"))
	{
		int operand1 = st.top(); st.pop();
		int operand2 = st.top(); st.pop();

		if (operand2 <= 0) st.push(0);
		else st.push(operand1);
	}
	else
	{
		int number = atoi(move->name);
		st.push(number);
	}
}

Node* make_tree(char** tokens, int* index)
{
	Node* root = NULL;
	Node* cur = NULL;

	while (1)
	{
		if (!strcmp(tokens[*index], ""))
			break;
		if (!strcmp(tokens[*index], ")"))
		{
			return root;
		}

		if (!strcmp(tokens[*index], "MINUS") || !strcmp(tokens[*index], "IF"))
		{
			Node* new_node = create_node(tokens[*index]);

			if (root == NULL)
			{
				root = new_node;
				cur = new_node;
			}
		}
		else if (!strcmp(tokens[*index], "("))
		{
			*index = *index + 1;
			Node* new_node = make_tree(tokens, index);

			if (root == NULL)
			{
				root = new_node;
				cur = new_node;
			}
			else
			{
				if (cur->left == NULL)
					cur->left = new_node;
				else if (cur->right == NULL)
					cur->right = new_node;
			}

		}
		else
		{
			Node* new_node = create_node(tokens[*index]);

			if (cur->left == NULL)
				cur->left = new_node;
			else if (cur->right == NULL)
				cur->right = new_node;
			else
			{
				printf("토큰 에러\n");
				return NULL;
			}
		}
		*index = *index + 1;
	}

	return root;
}

Node* create_node(char* name)
{
	Node* new_node = (Node*)calloc(1, sizeof(Node));
	new_node->name = (char*)calloc(BUF_SIZE, sizeof(char));
	strcpy(new_node->name, name);
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;
}

void make_tokens(char *command, char **tokens, int*index)
{
	for (int i = 0; i < strlen(command); i++)
	{
		if (command[i] == '(')
			strcat(tokens[(*index)++],"(");
		else if (command[i] == ')')
			strcat(tokens[(*index)++], ")");
		else if (is_character(command[i]))
		{
			if (!strncmp((command + i), "MINUS", 5))
			{
				strncat(tokens[(*index)++], "MINUS", 5);
				i = i + 4;
			}
			else if (!strncmp((command + i), "IF", 2))
			{
				strncat(tokens[(*index)++], "IF", 2);
				i = i + 1;
			}
			else
			{
				printf("undefined\n");
				error_flag = true;
				return;
			}
		}
		else if (command[i] == '-')
		{
			if (strcmp(tokens[*index - 1], "-"))
				strcat(tokens[(*index)++], "-");
			else
			{
				printf("\"-\" 사용 오류\n");
				error_flag = true;
				return;
			}
		}
		else if (is_number(command[i]))
		{
			if ((*index > 0 &&!strcmp(tokens[*index - 1], "-")) || (*index > 0 && is_number(command[i-1])))
				strncat(tokens[*index - 1], command + i, 1);
			else
				strncat(tokens[(*index)++], command+i, 1);
		}
		else if (command[i] == ' ')
		{
			continue;
		}
		else
		{
			printf("이상한 기호\n");
			error_flag = true;
			return;
		}
	}
}

char* ltrim(char* command)
{
	char* start = command;

	while (*start == ' ') { start++; }
	return start;
}

char* rtrim(char* command)
{
	char* buf = (char*)calloc(BUF_SIZE, sizeof(char));
	char* end = command + strlen(command) - 1;
	while (*end == ' ') end--;
	strncpy(buf, command, end - command + 1);

	return buf;
}

bool is_character(char a)
{
	if ((a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z')) return true;
	else return false;
}

bool is_number(char a)
{
	if (a >= '0' && a <= '9') return true;
	else return false;
}

bool is_integer(char* a)
{
	double tmp = atof(a);
	if (tmp - (int)tmp != 0) return false;
	else return true;
}

bool check_brackets(char **tokens, int* index)
{
	if (tokens[*index][0] != '(')
	{
		printf("'(' 위치가 잘못됐습니다\n");
		return false;
	}
	*index = *index + 1;

	if (strcmp(tokens[*index], "MINUS") && strcmp(tokens[*index], "IF"))
	{
		printf("undefined\n");
		return false;
	}
	*index = *index + 1;


	if (tokens[*index][0] == '(')
	{
		bool check = check_brackets(tokens, index);
		if (check == false)
			return false;
	}
	else if (is_number(tokens[*index][0]) || tokens[*index][0] == '-')
	{
		*index = *index + 1;
	}
	else
	{
		printf("'(' 위치가 잘못됐습니다\n");
		return false;
	}


	if (tokens[*index][0] == '(')
	{
		bool check = check_brackets(tokens, index);
		if (check == false)
			return false;
	}
	else if (is_number(tokens[*index][0]) || tokens[*index][0] == '-')
	{
		*index = *index + 1;
	}
	else
	{
		printf("'(' 위치가 잘못됐습니다\n");
		return false;
	}

	if (tokens[*index][0] != ')')
	{
		printf("')' 위치가 잘못됐습니다\n");
		return false;
	}
	else
	{
		*index = *index + 1;
		return true;
	}
}