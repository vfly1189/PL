#include <iostream>
#include <vector>
#include <string>

#define MAX_TURN 1
#define MIN_TURN 0
#define INF 999999999

using namespace std;

int num_of_stones, num_of_taken;
double calculated_value;
int node_count = 0;
int max_depth = 0;
int turn = MAX_TURN;
vector<bool> prime;
vector<bool> check;
vector<int> max_player;
vector<int> min_player;
//평가함수
double evaluate(int last_num, int flag)
{
	if (flag == 0)
	{
		if (num_of_taken % 2 == 0)
		{
			if (check[1] == false) return 0;
			else if (last_num == 1)
			{
				int count = 0;
				for (int i = 1; i <= num_of_stones; i++)
				{
					if (check[i] == false && (i % last_num == 0 || last_num % i == 0))count++;
				}
				if (count % 2 == 1) return 0.5;
				else return -0.5;
			}
			else if (prime[last_num] == false)
			{
				int count = 0;
				for (int i = last_num; i <= num_of_stones; i++)
				{
					if (i % last_num == 0) count++;
				}
				if (count % 2 == 1) return 0.7;
				else return -0.7;

			}
			else if (prime[last_num] == true)
			{
				int count = 0;
				for (int i = 2; i <= num_of_stones; i++)
				{
					if (prime[i] == false) count++;
				}
				if (count % 2 == 1) return 0.6;
				else return -0.6;
			}
		}
		else
		{
			if (check[1] == false) return 0;
			else if (last_num == 1)
			{
				int count = 0;
				for (int i = 1; i <= num_of_stones; i++)
				{
					if (check[i] == false && (i % last_num == 0 || last_num % i == 0))count++;
				}
				if (count % 2 == 1) return 0.5;
				else return -0.5;
			}
			else if (prime[last_num] == false)
			{
				int count = 0;
				for (int i = last_num; i <= num_of_stones; i++)
				{
					if (i % last_num == 0) count++;
				}
				if (count % 2 == 1) return 0.7;
				else return -0.7;

			}
			else if (prime[last_num] == true)
			{
				int count = 0;
				for (int i = 2; i <= num_of_stones; i++)
				{
					if (prime[i] == false) count++;
				}
				if (count % 2 == 1) return 0.6;
				else return -0.6;
			}
		}
	}
	else
	{
		if (num_of_taken % 2 == 0)
		{
			if (check[1] == false) return 0;
			else if (last_num == 1)
			{
				int count = 0;
				for (int i = 1; i <= num_of_stones; i++)
				{
					if (check[i] == false && (i % last_num == 0 || last_num % i == 0))count++;
				}
				if (count % 2 == 1) return 0.5;
				else return -0.5;
			}
			else if (prime[last_num] == false)
			{
				int count = 0;
				for (int i = last_num; i <= num_of_stones; i++)
				{
					if (i % last_num == 0) count++;
				}
				if (count % 2 == 1) return 0.7;
				else return -0.7;

			}
			else if (prime[last_num] == true)
			{
				int count = 0;
				for (int i = 2; i <= num_of_stones; i++)
				{
					if (prime[i] == false) count++;
				}
				if (count % 2 == 1) return 0.6;
				else return -0.6;
			}
		}
		else
		{
			if (check[1] == false) return 0;
			else if (last_num == 1)
			{
				int count = 0;
				for (int i = 1; i <= num_of_stones; i++)
				{
					if (check[i] == false && (i % last_num == 0 || last_num % i == 0))count++;
				}
				if (count % 2 == 1) return 0.5;
				else return -0.5;
			}
			else if (prime[last_num] == false)
			{
				int count = 0;
				for (int i = last_num; i <= num_of_stones; i++)
				{
					if (i % last_num == 0) count++;
				}
				if (count % 2 == 1) return 0.7;
				else return -0.7;

			}
			else if (prime[last_num] == true)
			{
				int count = 0;
				for (int i = 2; i <= num_of_stones; i++)
				{
					if (prime[i] == false) count++;
				}
				if (count % 2 == 1) return 0.6;
				else return -0.6;
			}
		}
	}
	return 0;
}
//1~num_of_stones사이에 소수 판별
void check_prime()
{
	prime[0] = true;
	prime[1] = true;

	for (int i = 2; i < prime.size(); i++)
	{
		for (int j = 2; i * j < prime.size(); j++)
		{
			prime[i * j] = true;
		}
	}
}

double search(int depth, double alpha, double beta, int last_num, int flag)
{
	vector<int> can_take;
	node_count++;
	//현재노드 평가함수를 이용해 점수 계산
	double evaluate_score = evaluate(last_num, flag);

	//트리의 루트 노드일때
	if (last_num != -1)
	{
		for (int i = 1; i <= num_of_stones; i++)
		{
			if (check[i] == false && (i % last_num == 0 || last_num % i == 0))
				can_take.push_back(i);
		}
	}
	//루트노드가 아닐때
	else
	{
		for (int i = 1; i <= num_of_stones; i++)
		{
			if (((double)i < (double)num_of_stones / (double)2) && (i % 2 == 1)) can_take.push_back(i);
		}
	}
	if (max_depth < depth) max_depth = depth;

	//가져갈 수 있는 돌이 없어서 누군가가 승리할때 판별
	if (can_take.size() == 0)
	{
		if (flag == 0)
		{
			if (num_of_taken % 2 == 0) return -1.0;
			else return 1.0;
		}
		else if (flag == 1)
		{
			if (num_of_taken % 2 == 0) return -1.0;
			else return 1.0;
		}
	}
	//현재 내차례일때
	if (flag == 0)
	{
		double tmp;
		int index=can_take[0];
		//가져갈수있는 돌들 확인
		for (int i = 0; i < can_take.size(); i++)
		{
			check[can_take[i]] = true;
			tmp = search(depth + 1, alpha, beta, can_take[i], 1);
			//alpha값 갱신
			if (tmp > alpha)
			{
				index = can_take[i];
				alpha = tmp;
			}
			if (evaluate_score > alpha) alpha = evaluate_score;
			//가지치기 할지 말지 판별
			if (beta <= alpha) break;
			check[can_take[i]] = false;
		}
		if (depth == 0)
		{
			calculated_value = alpha;
			return index;
		}
		return alpha;
	}
	//상대차례
	else
	{
		double tmp;
		//가져갈수있는 돌들 확인
		for (int i = 0; i < can_take.size(); i++)
		{
			check[can_take[i]] = true;
			tmp = search(depth + 1, alpha, beta, can_take[i], 0);

			//beta값 갱신
			if (beta > tmp) beta = tmp;
			//가지치기 할지 말지 판별
			if (beta <= alpha) break;
			check[can_take[i]] = false;
		}
		return beta;
	}

}

int main(int argc, char* argv[])
{
	int last_num = -1;
	if (argc < 2)
	{
		fprintf(stderr, "input error\n");
		exit(1);
	}

	num_of_stones = atoi(argv[1]);
	num_of_taken = atoi(argv[2]);

	check.resize(num_of_stones + 1);
	prime.resize(num_of_stones + 1);

	check_prime();

	//각자 가져간 돌들 플레이어한테 배분
	if (num_of_taken != 0)
	{
		for (int i = 3; i < argc; i++)
		{
			if (turn == MAX_TURN)
			{
				max_player.push_back(atoi(argv[i]));
				turn = MIN_TURN;
			}
			else if (turn == MIN_TURN)
			{
				min_player.push_back(atoi(argv[i]));
				turn = MAX_TURN;
			}
			check[atoi(argv[i])] = true;
		}
		//마지막 숫자 체크
		if (turn == MAX_TURN)
			last_num = min_player[min_player.size() - 1];

		else if (turn == MIN_TURN)
			last_num = max_player[max_player.size() - 1];
	}

	printf("Best move : %d\n", (int)search(0, -INF, INF, last_num, 0));
	printf("Calculated value : %.1f\n", calculated_value);
	printf("Number of visited Nodes : %d\n", node_count);
	printf("Max Depth : %d\n", max_depth);
}

