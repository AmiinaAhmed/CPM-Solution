#include <bits\stdc++.h>
#define N 1005
#define MAXNUM  5000
#define UNDONE -1
#define MINNUM -1

using namespace std;

//=============================================================================================
typedef struct node { //of node

	int visted; //(visted / not_visted)
	int early_start; // early start time
	int early_end; // early end time ( early finish )
	int duration; // duratin time (value of node)
	int last_start; // last start time 
	int last_end; // last end time ( last finish )
 //===================================================//
	int array_fathers[N]; // the father of the sons
	int array_sons[N]; // the sons of the father
	int len_fathers; // lengnth of fathers
	int len_sons; // length of sons
	int n_fathers; // number of fathers
	int n_sons; // number of sons
//===================================================//
} task_node;

task_node net_work[N]; // (n) of nodes

//===============================================================================================
queue<task_node> q; // 

void forward_path() {

	while (!q.empty()) { q.pop(); }//clean q from any gabage
	task_node start_node = net_work[0];
	q.push(start_node);
	while (!q.empty()) {
		task_node cur_node = q.front();
		q.pop();
		for (int i = 0;i < cur_node.len_sons;i++) {
			int index = cur_node.array_sons[i];
			net_work[index].n_fathers--;
			if (net_work[index].early_start < cur_node.early_end) {
				net_work[index].early_start = cur_node.early_end;
				net_work[index].early_end = net_work[index].early_start + net_work[index].duration;
			}
			if (net_work[index].n_fathers == 0 && net_work[index].visted != 1) {
				q.push(net_work[index]);
			}
		}
	}
	return;
}

//=============================================================================================

void backward_path(int end_index) {

	task_node end_node = net_work[end_index];

	while (!q.empty()) { q.pop(); }
	q.push(end_node);
	while (!q.empty()) {

		task_node cur_node = q.front();

		q.pop();

		for (int i = 0;i < cur_node.len_fathers;i++) {
			int index = cur_node.array_fathers[i];
			net_work[index].n_sons--;
			if (net_work[index].last_end > cur_node.last_start) {
				net_work[index].last_end = cur_node.last_start;
				net_work[index].last_start = net_work[index].last_end - net_work[index].duration;
			}
			if (net_work[index].n_sons == 0 && net_work[index].visted != 1) {
				q.push(net_work[index]);
			}
		}

	}
	return;
}

//=============================================================================================

bool find_critical_path(int index, string s[]) {

	if (index != 0 && net_work[index].visted == 1) return true;
	task_node cur_node = net_work[index];
	if (cur_node.early_start != cur_node.last_start) return false;
	for (int i = 0;i < cur_node.len_sons;i++) {
		if (find_critical_path(cur_node.array_sons[i], s)) {
			cout << " <<- " << s[index];
			return true;
		}
	}
	return false;
}

//=============================================================================================

void initialization(int n) {

	for (int i = 0;i <= n + 1;i++) {
		net_work[i].visted = 0;
		net_work[i].early_start = MINNUM;
		net_work[i].early_end = UNDONE;
		net_work[i].duration = 0;
		net_work[i].last_start = UNDONE;
		net_work[i].last_end = MAXNUM;
		memset(net_work[i].array_fathers, 0, sizeof(net_work[i].array_fathers));
		memset(net_work[i].array_sons, 0, sizeof(net_work[i].array_sons));
		net_work[i].len_fathers = 0;
		net_work[i].len_sons = 0;
		net_work[i].n_fathers = 0;
		net_work[i].n_sons = 0;
	}
	net_work[0].visted = 1;
	net_work[n + 1].visted = 1;
	net_work[0].early_start = 0;
	net_work[0].early_end = 0;
	return;
}

//=============================================================================================

void add_node(int fa, int son) {
	net_work[fa].array_sons[net_work[fa].len_sons++] = son;
	net_work[fa].n_sons++;
	net_work[son].array_fathers[net_work[son].len_fathers++] = fa;
	net_work[son].n_fathers++;
}

//=============================================================================================

void show_item(int n,string s[]) {

	for (int i = 1;i <= n;i++) {
		cout << "____________________________________________________________________________________\n";
		cout << s[i] << " | \t\t | ";
		cout << net_work[i].early_start << " |  \t  | " << net_work[i].early_end << " |  \t  | " << net_work[i].last_start << " |  \t  | " << net_work[i].last_end << " | " << endl;
	}

	cout << "____________________________________________________________________________________\n";

}

//=============================================================================================

int main() {
	cout << "Enter Number Of Tasks : "; int n; cin >> n;
	initialization(n);

	cout << "\nEnter " << n << " Duration Times : ";
	for (int i = 1;i <= n;i++) {
		cin >> net_work[i].duration;
	}

	string tasks_names[N];
	cout << "\nEnter " << n << " tasks name :";
	tasks_names[0] = "Start"; tasks_names[n + 1] = "End";
	for (int i = 1;i <= n;i++) {
		cin >> tasks_names[i];
	}


	cout << "\nHow many Realation do you want : "; int m; cin >> m;
	cout << "\nEnter " << m << " Realations To Calculate : (Father Node - Son Node )\n";
	int fa = -1, son = -1; ;
	for (int i = 0;i < m;i++) {
		string s1, s2; cin >> s1 >> s2;
		for (int j = 0;j <= n + 1;j++) {
			if (s1 == tasks_names[j]) fa = j;
			if (s2 == tasks_names[j]) son = j;
		}
		add_node(fa, son);
	}

	forward_path();
	net_work[n + 1].last_start = net_work[n + 1].early_start;
	net_work[n + 1].last_end = net_work[n + 1].early_end;
	backward_path(n + 1);

	printf("\n------------------------------------------------------------------------------------\n");
	cout << "\nThe Cirtical Path : \nEnd";
	find_critical_path(0, tasks_names);
	printf("\n\n------------------------------------------------------------------------------------\n\n");
	cout << "\n____________________________________________________________________________________\n";
	cout << "Name-Tasks \t Net-work ES \t Net-work EF \t Net-work LS \t Net-work LF \n";
	
	show_item(n, tasks_names); 
	cout << "\nThis is Longest Time for whole tasks : " << net_work[n].last_end << endl;

	printf("\n------------------------------------------------------------------------------------\n");
	system("pause");
	return 0;
}
//=============================================================================================

