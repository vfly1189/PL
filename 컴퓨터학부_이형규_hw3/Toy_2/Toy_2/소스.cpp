#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
#include <vector>
#include <iostream>
#include <ctype.h>
#include <iostream>
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
void add_minus_if();
void define_defun();
void load_defun();
void interpreter();
bool check_error(vector<string>& tokens, int* index, string command);
bool make_tokens3(vector<string>& dst, string command);
void converter(vector<string>& old_v, vector<string>& new_v, int* index);
void make_tokens2(vector<string>& tmp1, char* commands);
bool check_bracket_cnt(vector<string>& tokens);
Node* make_tree(vector<string>& tokens, int* index);
Node* create_node(string name);
void print_tree(Node* move);
bool is_character(char a);
bool is_number(char a);
void calc_tree(Node* root);

stack<int> st;
vector<string> defun_names;
vector<pair<vector<string>, vector<string>>> defun_data;

int main()
{
    start_interpreter();
}

void start_interpreter()
{
    int select;

    add_minus_if();

    while (1)
    {
        printf("==================================\n");
        printf("1. Define DEFUN\n2. Print DEFUN\n3. Interpreter\n4. Exit\n");
        printf("==================================\n");

        printf("메뉴를 선택하세요 >> ");
        scanf("%d", &select);

        if (select == 1)
        {
            define_defun();
        }
        else if (select == 2)
        {
            load_defun();


            for (int i = 0; i < defun_data.size(); i++)
            {
                if (!defun_data[i].first[0].compare("MINUS") || !defun_data[i].first[0].compare("IF")) continue;
                for (int j = 0; j < defun_data[i].first.size(); j++)
                    cout << defun_data[i].first[j] << " ";
                cout << "  ";
                for (int j = 0; j < defun_data[i].second.size(); j++)
                    cout << defun_data[i].second[j] << " ";
                cout << "\n";
            }
            /*

                     for (int i = 0; i < defun_names.size(); i++)
                        cout << defun_names[i] << endl;
                     cout << endl;

                     for (int i = 0; i < defun_data.size(); i++)
                     {
                        printf("%d번째\n", i);
                        for (int j = 0; j < defun_data[i].first.size(); j++)
                        {
                          cout << j << " : " << defun_data[i].first[j] << endl;
                        }
                        printf("========\n");
                        for (int j = 0; j < defun_data[i].second.size(); j++)
                        {
                          cout << j << " : " << defun_data[i].second[j] << endl;
                        }
                        printf("\n");
                     }
                     */

        }
        else if (select == 3)
        {
            interpreter();
        }
        else if (select == 4)
        {
            printf("\n프로그램을 종료합니다.");
            exit(0);
        }
        else
        {
            printf("입력오류\n");
            continue;
        }
    }
}

void add_minus_if()
{
    defun_names.push_back("MINUS");
    defun_names.push_back("IF");

    pair<vector<string>, vector<string>> tmp;
    vector<string> tmp2;

    tmp2.push_back("MINUS"); tmp2.push_back("("); tmp2.push_back("x"); tmp2.push_back("y"); tmp2.push_back(")");
    tmp.first = tmp2;
    tmp2.clear();
    tmp2.push_back("("); tmp2.push_back("MINUS"); tmp2.push_back("x"); tmp2.push_back("y"); tmp2.push_back(")");
    tmp.second = tmp2;
    defun_data.push_back(tmp);
    tmp2.clear();

    tmp2.push_back("IF"); tmp2.push_back("("); tmp2.push_back("x"); tmp2.push_back("y"); tmp2.push_back(")");
    tmp.first = tmp2;
    tmp2.clear();
    tmp2.push_back("("); tmp2.push_back("IF"); tmp2.push_back("x"); tmp2.push_back("y"); tmp2.push_back(")");
    tmp.second = tmp2;
    defun_data.push_back(tmp);
}

void define_defun()
{
    char* buf = (char*)calloc(BUF_SIZE, sizeof(char));
    getchar();
    fgets(buf, BUF_SIZE, stdin);

    pair<vector<string>, vector<string>> input_data;
    vector<string> tmp1;
    FILE* fp;
    char* p = buf;
    string tmp2;
    int l_bracket = 0, r_bracket = 0;
    int flag = 0;
    int i = 0;

    for (i = 0; i < strlen(buf); i++)
    {
        char letter = buf[i];

        if (is_character(letter) || is_number(letter) || letter == '/')
        {
            tmp2.push_back(letter);
        }
        else if (letter == ' ')
        {
            if (flag == 0 && tmp1.size() == 0)
            {
                if (tmp2.compare("DEFUN") == 0)
                {
                    p = buf + i;
                    tmp2.clear();
                    flag = 1;
                    continue;
                }
                else
                {
                    printf("Is DEFUN?\n");
                    return;
                }
            }

            if (tmp2.size() != 0) tmp1.push_back(tmp2);
            tmp2.clear();
        }
        else if (letter == '(')
        {
            if (tmp2.size() != 0)
            {
                tmp1.push_back(tmp2);
                tmp2.clear();
            }

            tmp2.push_back(letter);
            tmp1.push_back(tmp2);
            tmp2.clear();
            l_bracket++;
        }
        else if (letter == ')')
        {
            if (tmp2.size() != 0)
            {
                tmp1.push_back(tmp2);
                tmp2.clear();
            }

            tmp2.push_back(letter);
            tmp1.push_back(tmp2);
            tmp2.clear();
            r_bracket++;

            if (l_bracket != 0 && r_bracket != 0)
            {
                if (l_bracket == r_bracket)
                {
                    i++;
                    break;
                }
            }
        }
        else
        {
            printf("이상한 기호\n");
            return;
        }
    }

    ////////////////////오류체크//////////////////////////
    for (int k = 0; k < defun_names.size(); k++)
    {
        if (tmp1[0].compare(defun_names[k]) == 0)
        {
            printf("겹치는 함수\n");
            tmp1.clear();
            return;
        }
    }
    for (int k = 0; k < tmp1.size(); k++)
    {
        if (tmp1[k].compare("(") == 0)
            l_bracket = k;
        else if (tmp1[k].compare(")") == 0)
            r_bracket = k;
    }
    if (l_bracket + 1 == r_bracket)
    {
        printf("매개변수 없음\n");
        return;
    }
    ///////////////////////////////////////////////////////////
    defun_names.push_back(tmp1[0]);
    input_data.first = tmp1;
    tmp1.clear();
    tmp2.clear();
    while (buf[i] == ' ') i++;

    make_tokens2(tmp1, buf + i);
    input_data.second = tmp1;

    if ((fp = fopen("defun.txt", "a+")) == NULL)
    {
        fprintf(stderr, "open error for defun.txt\n");
        return;
    }
    while (*p == ' ') p++;
    p[strlen(p)] = '\0';
    fwrite(p, strlen(p), 1, fp);
    fclose(fp);

    defun_data.push_back(input_data);
}

void load_defun()
{
    defun_data.clear();
    defun_names.clear();
    add_minus_if();

    FILE* fp;
    int index = 0;
    char** commands = (char**)calloc(200, sizeof(char*));
    for (int i = 0; i < 200; i++)
        commands[i] = (char*)calloc(BUF_SIZE, sizeof(char));
    char* fname = (char*)calloc(BUF_SIZE, sizeof(char));
    char* buf = (char*)calloc(BUF_SIZE, sizeof(char));

    strcpy(fname, "defun.txt");

    if ((fp = fopen(fname, "r")) == NULL)
    {
        fprintf(stderr, "open error for %s\n", fname);
        return;
    }

    int last_tmp = 0;
    while (1)
    {
        char tmp = fgetc(fp);
        if (tmp == EOF)
        {
            if (last_tmp == 0) index++;
            break;
        }

        if (tmp == '\n')
        {
            last_tmp = 1;
            index++;
        }
        else
        {
            last_tmp = 0;
            commands[index][strlen(commands[index])] = tmp;
        }
    }

    for (int i = 0; i < index; i++)
    {
        int index2 = 0;
        pair<vector<string>, vector<string>> input_data;
        vector<string> tmp1;
        string tmp2;

        int l_bracket = 0, r_bracket = 0;

        for (index2 = 0; index2 < strlen(commands[i]); index2++)
        {
            char letter = commands[i][index2];
            if (is_character(letter) || is_number(letter) || letter == '/')
            {
                tmp2.push_back(letter);
            }
            else if (letter == ' ')
            {
                if (tmp2.size() != 0) tmp1.push_back(tmp2);
                tmp2.clear();
            }
            else if (letter == '(')
            {
                if (tmp2.size() != 0)
                {
                    tmp1.push_back(tmp2);
                    tmp2.clear();
                }

                tmp2.push_back(letter);
                tmp1.push_back(tmp2);
                tmp2.clear();
                l_bracket++;
            }
            else if (letter == ')')
            {
                if (tmp2.size() != 0)
                {
                    tmp1.push_back(tmp2);
                    tmp2.clear();
                }

                tmp2.push_back(letter);
                tmp1.push_back(tmp2);
                tmp2.clear();
                r_bracket++;
                if (l_bracket != 0 && r_bracket != 0)
                {
                    if (l_bracket == r_bracket)
                    {
                        index2++;
                        break;
                    }
                }
            }
        }
        defun_names.push_back(tmp1[0]);
        input_data.first = tmp1;

        while (commands[i][index2] == ' ') index2++;
        strcpy(buf, commands[i] + index2);
        tmp1.clear();
        tmp2.clear();
        make_tokens2(tmp1, buf);

        input_data.second = tmp1;

        defun_data.push_back(input_data);
    }
}

void interpreter()
{
    vector<string> commands;
    string commands_tmp;
    int index = 0;
    FILE* fp;
    char* fname = (char*)calloc(BUF_SIZE, sizeof(char));
    printf("실행 파일명을 입력하세요 >> ");

    scanf("%s", fname);

    if ((fp = fopen(fname, "r")) == NULL)
    {
        fprintf(stderr, "open error for %s\n", fname);
        return;
    }
    while (1)
    {
        char tmp = fgetc(fp);
        if (tmp == EOF)
        {
            if (commands_tmp.size() != 0) commands.push_back(commands_tmp);
            break;
        }
        else if (tmp == '\n')
        {
            commands.push_back(commands_tmp);
            commands_tmp.clear();
        }
        else
        {
            commands_tmp.push_back(tmp);
        }
    }

    for (int i = 0; i < commands.size(); i++)
    {
        int index = 0;
        bool check;
        vector<string> tokens;
        vector<string> convert;

        cout << "\n************\n";
        cout << commands[i] << " -> ";

        //cout << "commands : " << commands[i] << " --------> ";
        check = make_tokens3(tokens, commands[i]);
        if (check == false) continue;


        check = check_error(tokens, &index, commands[i]);
        if (check == false) continue;
        index = 0;

        check = check_bracket_cnt(tokens);
        if (check == false) continue;


        int count = 0;
        while (1)
        {
            converter(tokens, convert, &index);
            int flag = 0;
            for (int j = 0; j < convert.size(); j++)
            {
                if (is_character(convert[j][0]))
                {
                    if (!convert[j].compare("MINUS") || !convert[j].compare("IF")) continue;
                    else
                    {
                        for (int h = 0; h < defun_names.size(); h++)
                        {
                            if (!convert[j].compare(defun_names[h]))
                            {
                                flag = 1; break;
                            }
                        }
                    }
                }
                if (flag == 1) break;
            }
            if (flag == 0) break;

            index = 0;
            tokens = convert;
            convert.clear();
        }

        
        for (int j = 0; j < convert.size(); j++)
        {
            cout << convert[j] << " ";
        }
        cout << endl;
        index = 0;
        Node* root = make_tree(convert, &index);

        
        cout << "\nPrefix To Postfix :  ";
        print_tree(root);
        calc_tree(root);
        cout << "\nResult : " << st.top(); st.pop();
        cout << endl;
           
        while (!st.empty()) st.pop();
    }
}

void converter(vector<string>& old_v, vector<string>& new_v, int* index)
{
    while (1)
    {
        if (*index >= old_v.size()) break;
        if (is_character(old_v[*index][0]))
        {
            if (!old_v[*index].compare("MINUS") || !old_v[*index].compare("IF"))
            {
                new_v.push_back(old_v[*index]);
            }
            else
            {
                vector<string> exp;
                vector<pair<string, vector<string>>> args;
                int cnt = 0;
                int left = 0, right = 0, arg_cnt = 0;

                //알맞은 defun 찾기
                for (cnt = 0; cnt < defun_data.size(); cnt++)
                {
                    if (!defun_data[cnt].first[0].compare(old_v[*index]))
                    {
                        exp = defun_data[cnt].second;
                        break;
                    }
                }

                //매개변수 갯수
                for (int j = 0; j < defun_data[cnt].first.size(); j++)
                {
                    if (!defun_data[cnt].first[j].compare("(")) left = j;
                    else if (!defun_data[cnt].first[j].compare(")")) right = j;
                }
                arg_cnt = right - left - 1;

              
                *index = *index + 1;
                for (int j = 0; j < arg_cnt; j++)
                {
                    pair<string, vector<string>> tmp;
                    tmp.first = defun_data[cnt].first[j + left + 1];
                    if (old_v[*index][0] == '-' || is_number(old_v[*index][0]))
                    {
                        tmp.second.push_back(old_v[*index]);
                        args.push_back(tmp);
                        *index = *index + 1;
                    }
                    else
                    {
                        if (!old_v[*index].compare("("))
                        {
                            tmp.second.push_back(old_v[*index]);
                            int l_b = 1, r_b = 0;
                            *index = *index + 1;
                            while (l_b != r_b)
                            {
                                if (!old_v[*index].compare("(")) l_b++;
                                else if (!old_v[*index].compare(")")) r_b++;
                                tmp.second.push_back(old_v[*index]);
                                *index = *index + 1;
                            }
                            args.push_back(tmp);
                        }
                    }
                }

 

                for (int i = 0; i < exp.size(); i++)
                {
                    int flag = 0;
                    for (int j = 0; j < args.size(); j++)
                    {
                        if (!exp[i].compare(args[j].first))
                        {
                            flag = 1;
                            for (int h = 0; h < args[j].second.size(); h++)
                                new_v.push_back(args[j].second[h]);
                            break;
                        }
                    }
                    if (flag == 0)
                        new_v.push_back(exp[i]);
                }
                new_v.push_back(")");
            }
        }
        else
            new_v.push_back(old_v[*index]);
        *index = *index + 1;
    }
}

bool check_error(vector<string>& tokens, int* index, string command)
{
    if (is_number(tokens[*index][0]) || tokens[*index][0] == '-')
    {
        return true;
    }

    if (tokens[*index].compare("("))
    {
        printf("\"(\"의 위치가 잘못됐습니다\n");
        return false;
    }
    else *index = *index + 1;

    int left = 0, right = 0;
    int arg_cnt = 0;
    for (int cnt = 0; cnt < defun_data.size(); cnt++)
    {
        if (!tokens[*index].compare(defun_data[cnt].first[0]))
        {
            for (int j = 0; j < defun_data[cnt].first.size(); j++)
            {
                if (!defun_data[cnt].first[j].compare("(")) left = j;
                else if (!defun_data[cnt].first[j].compare(")")) right = j;
            }
        }
    }
    arg_cnt = right - left - 1;
    *index = *index + 1;
    for (int i = 0; i < arg_cnt; i++)
    {
        if (!tokens[*index].compare("("))
        {
            bool check = check_error(tokens, index, command);
            if (check == false) return false;
        }
        else if (is_number(tokens[*index][0]) || tokens[*index][0] == '-')
        {
            *index = *index + 1;
        }
        else
        {
            printf("매개변수 갯수 오류\n");
            return false;
        }
    }
    if (tokens[*index].compare(")"))
    {
        printf("\")\"의 위치가 잘못되었습니다.\n");
        return false;
    }
    else
    {
        *index = *index + 1;
        return true;
    }
}

bool check_bracket_cnt(vector<string>& tokens)
{
    stack<int> bracket;

    for (int i = 0; i < tokens.size(); i++)
    {
        if (!tokens[i].compare("("))
        {
            bracket.push(1);
        }
        else if (!tokens[i].compare(")"))
        {
            if (bracket.size() == 0)
            {
                printf("\")\"의 위치가 잘못되었습니다.\n");
                return false;
            }
            else
                bracket.pop();
        }
    }
    if (bracket.size() != 0)
    {
        printf("\"(\"의 위치가 잘못되었습니다.\n");
        return false;
    }
    return true;
}

void print_tree(Node* root)
{
    if (root == NULL) return;

    print_tree(root->left);
    print_tree(root->right);

    cout << root->name << " ";
}

Node* make_tree(vector<string>& tokens, int* index)
{
    Node* root = NULL;
    Node* cur = NULL;

    while (1)
    {
        if (*index >= tokens.size()) break;
        if (!tokens[*index].compare(")"))
            return root;
        else if (is_character(tokens[*index][0]))
        {
            if (!tokens[*index].compare("MINUS") || !tokens[*index].compare("IF"))
            {
                Node* new_node = create_node(tokens[*index]);

                if (root == NULL)
                {
                    root = new_node;
                    cur = new_node;
                }

            }
        }
        else if (!tokens[*index].compare("("))
        {
            *index = *index + 1;
            Node* new_tree = make_tree(tokens, index);

            if (root == NULL)
            {
                root = new_tree;
                cur = new_tree;
            }
            else
            {
                if (cur->left == NULL)
                    cur->left = new_tree;
                else if (cur->right == NULL)
                    cur->right = new_tree;
            }
        }
        else
        {
            Node* new_node = create_node(tokens[*index]);

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
        *index = *index + 1;
    }
    return root;
}

Node* create_node(string name)
{
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    new_node->name = (char*)calloc(BUF_SIZE, sizeof(char));
    strcpy(new_node->name, name.c_str());
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

void make_tokens2(vector<string>& tmp1, char* commands)
{
    string tmp;

    for (int i = 0; i < strlen(commands); i++)
    {
        char letter = commands[i];

        if (is_character(letter) || is_number(letter) || letter == '-' || letter == '/')
        {
            tmp.push_back(letter);
        }
        else if (letter == ' ')
        {
            if (tmp.size() != 0) tmp1.push_back(tmp);
            tmp.clear();
        }
        else if (letter == '(' || letter == ')')
        {
            if (tmp.size() != 0)
            {
                tmp1.push_back(tmp);
                tmp.clear();
            }

            tmp.push_back(letter);
            tmp1.push_back(tmp);
            tmp.clear();
        }
    }
}

bool make_tokens3(vector<string>& dst, string command)
{
    string tmp;
    for (int i = 0; i < command.size(); i++)
    {
        if (command[i] == '(')
        {
            if (tmp.size() != 0)
            {
                dst.push_back(tmp);
                tmp.clear();
            }
            dst.push_back("(");
        }
        else if (command[i] == ')')
        {
            if (tmp.size() != 0)
            {
                dst.push_back(tmp);
                tmp.clear();
            }
            dst.push_back(")");
        }
        else if (command[i] == ' ')
        {
            if (tmp.size() != 0)
            {
                dst.push_back(tmp);
                tmp.clear();
            }
        }
        else if (is_number(command[i]) || command[i] == '-')
        {
            if (command[i] == '-')
            {
                if (tmp.size() != 0)
                {
                    printf("- 기호 사용 오류\n");
                    dst.clear();
                    return false;
                }
            }
            tmp.push_back(command[i]);
        }

        else if (is_character(command[i]))
        {
            int flag = 0;
            for (int cnt = 0; cnt < defun_names.size(); cnt++)
            {
                for (int z = i; ; z++)
                {
                    if (command[z] == ' ' || command[z] == 0 || command[z] == '\n') break;
                    tmp.push_back(command[z]);
                }


                if (!tmp.compare(defun_names[cnt]))
                {
                    flag = 1;
                    break;
                }
                else
                    tmp.clear();
            }
            if (flag == 1)
            {
                i = i + tmp.size() - 1;
                dst.push_back(tmp);
                tmp.clear();
            }
            else
            {
                printf("undefined\n");
                dst.clear();
                return false;
            }
        }
        else
        {
            printf("이상한 기호\n");
            dst.clear();
            return false;
        }
    }
    if (tmp.size() != 0) dst.push_back(tmp);
    tmp.clear();
}

void calc_tree(Node* root)
{
    if (root == NULL) return;

    calc_tree(root->left);
    calc_tree(root->right);

  
    if (root->name[0] == '-' || is_number(root->name[0]))
    {
        st.push(atoi(root->name));
    }
    else if (!strcmp(root->name, "MINUS") || !strcmp(root->name, "IF"))
    {
        int arg1 = st.top(); st.pop();
        int arg2 = st.top(); st.pop();

        if (!strcmp(root->name, "MINUS"))
            st.push(arg2 - arg1);
        else if (!strcmp(root->name, "IF"))
        {
            if (arg2 <= 0) st.push(0);
            else st.push(arg1);
        }
    }
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