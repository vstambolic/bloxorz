#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#define MAX_CHAR 32

/*Colours*/
void cyan() {
	printf("\033[0;36m");
}
void reset() {
	printf("\033[0m");
}
void blue() {
	printf("\033[0;34m");
}
void boldblue() {
	printf("\033[1;34m");
}

typedef struct node {
	int x, y;
	char label;
	struct node * next;
} Node;
typedef Node * Pointer;
enum State { straight, horizontal, vertical };
typedef struct {
	int str;
	int hor;
	int ver;
} Element;

typedef Element * Visit_Vector;

typedef struct queue_elem {
	enum State state;
	int pos_1;
	int pos_2;
	struct queue_elem * next;

} Queue_Elem;

typedef Queue_Elem * Queue;

int queue_empty(Queue Q) {
	if (Q==NULL) return 1;
	else return 0;
}

char ** load_map(int*m, int*n) {
	char **A=NULL;
	int i, j;

	printf("Enter the name of the file which contains map concept: ");
	char s[MAX_CHAR], x;
	scanf("%s", &s);

	
	FILE *f;
	f = fopen(s, "r");

	if (f == NULL) {
		printf("Invalid file path.\n");
		return A;
	}

	if (feof(f)) {
		printf("File is empty.\n");
		return A;
	}
	
	A = malloc(sizeof(char*));
	A[0] = NULL;
	i = 0, j = 0;

	while ((x = fgetc(f))!='\n') {
		
		A[0] = realloc(A[0], (j + 1) * sizeof(char));
		A[0][j] = x;
		j++;
	}

	*n = j;


	while ((x = fgetc(f)) != EOF) {
		i++;
		A = realloc(A, (i + 1) * sizeof(char*));
		A[i] = calloc(*n, sizeof(char));
		A[i][0] = x;

		for (j = 1; j < *n; j++) {
			x = fgetc(f);
			A[i][j] = x;
		}
		fgetc(f);   // get \n
	


	}
	*m = i+1;
	fclose(f);
	return A;
}

/*Add neighbour at the end of Adjacency List*/
Pointer add_neighbour(Pointer h, int i, int j,char label) {
	Pointer tmp;
	tmp = malloc(sizeof(Node));
	tmp->x = i;
	tmp->y = j;
	tmp->label = label;
	tmp->next = NULL;

	if (h == NULL) {
		return tmp;
	}
	else {
		Pointer ptr = h;
		while (ptr->next != NULL) 
			ptr = ptr->next;

		ptr->next = tmp;

		return h;
	}
}

/*Create graph based on the matrix*/
Pointer create_graph(char** A, int m, int n,int *node_num) {
	Pointer g = NULL;
	int i, j;
	*node_num = 0;

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++) {
			if (A[i][j] != '-') {

				g = realloc(g, (*node_num + 1) * sizeof(Node));
				g[*node_num].x = i;
				g[*node_num].y = j;
				g[*node_num].label = A[i][j];
				g[*node_num].next = NULL;
				

				if (i - 1 >= 0)
					if (A[i - 1][j] != '-')
						g[*node_num].next = add_neighbour(g[*node_num].next, i - 1, j, A[i - 1][j]);
				if (j + 1 < n)
					if (A[i][j + 1] != '-')
						g[*node_num].next = add_neighbour(g[*node_num].next, i, j + 1, A[i][j + 1]);
				if (i + 1 < m)
					if (A[i + 1][j] != '-')
						g[*node_num].next = add_neighbour(g[*node_num].next, i + 1, j, A[i + 1][j]);
				if (j - 1 >= 0)
					if (A[i][j - 1] != '-')
						g[*node_num].next=add_neighbour(g[*node_num].next, i, j - 1, A[i][j - 1]);

				(*node_num)++;

			}
		}


	
	
	return g;
}


/*Delete Adjacency List*/
void delete_list(Pointer h) {
	Pointer old;
	while (h) {
		old = h;
		h = h->next;
		free(old);
	}
}
/*Delete Graph*/
void delete_graph(Pointer g, int node_num) {
	for (int i = 0; i < node_num; i++) {
		delete_list(g[i].next);
	}
	free(g);
}

/*Delete Map*/
void delete_map(char ** A, int m) {
	for (int i = 0; i < m; i++) {
		free(A[i]);
	}
	free(A);
}
/*Find node with coordinates (x,y) (if it can't be found, returns NULL)*/
Pointer find_node(Pointer g, int node_num, int x, int y) {
	int i;								
	for (i = 0; i < node_num; i++) {
		if (g[i].x == x && g[i].y == y) return &(g[i]);
	}
	return NULL;

}
/*Find id of node with coordinates (x,y) (if it can't be found, returns -1)*/
int find_node_id(Pointer g, int node_num, int x, int y) {
	int i;
	for (i = 0; i < node_num; i++)
		if (g[i].x == x && g[i].y == y) return i;
	return -1;
}

/*Find node by label*/
int find_node_id_by_label(Pointer g, int node_num, char label) {
	int i;
	for (i = 0; i < node_num; i++)
		if (g[i].label == label) return i;
	return -1;
}
/*Number of neighbours*/
int neighbour_count(Pointer h) {
	int k = 0;
	while (h) {
		k++;
		h = h->next;
	}
	return k;
}


/*Insert Field*/
Pointer add_node(Pointer g, int node_num, int x, int y) {
	int i = 0;
	int id = 0, tmp_id;
	g = realloc(g, (node_num) * sizeof(Node));
	if (g == NULL) {
		printf("Error\n");
		exit(1);
	}

	while ((id < node_num-1) && (g[id].x < x)) {
		id++;
	}

	while ((g[id].x == x) && (g[id].y < y)) {
			id++;
	}
	



	for (int j = node_num-1; j != id; j--) {
		g[j] = g[j - 1];
	}


	g[id].x = x;
	g[id].y = y;
	g[id].label = 'o';
	g[id].next = NULL;
	
	if ((tmp_id = find_node_id(g, node_num, x - 1, y)) != -1) {
		g[tmp_id].next = add_neighbour(g[tmp_id].next, x, y, 'o');
		g[id].next = add_neighbour(g[id].next, g[tmp_id].x, g[tmp_id].y, g[tmp_id].label);
	}
	if ((tmp_id = find_node_id(g, node_num, x, y+1)) != -1) {
		g[tmp_id].next = add_neighbour(g[tmp_id].next, x, y, 'o');
		g[id].next = add_neighbour(g[id].next, g[tmp_id].x, g[tmp_id].y, g[tmp_id].label);
	}
	if ((tmp_id = find_node_id(g, node_num, x+1, y)) != -1) {
		g[tmp_id].next = add_neighbour(g[tmp_id].next, x, y, 'o');
		g[id].next = add_neighbour(g[id].next, g[tmp_id].x, g[tmp_id].y, g[tmp_id].label);
	}
	if ((tmp_id = find_node_id(g, node_num, x, y-1)) != -1) {
		g[tmp_id].next = add_neighbour(g[tmp_id].next, x, y, 'o');
		g[id].next = add_neighbour(g[id].next, g[tmp_id].x, g[tmp_id].y, g[tmp_id].label);
	}
	
	return g;
}
Pointer add_field(char**A, int m, int n, Pointer g, int *node_num) {
	int x, y;
	printf("Insert coordinates of the place where field needs to be inserted: ");
	while (1) {
		scanf("%d%d", &x, &y);
		int p = 0;

		if (x >= m || x < 0 || y >= n || y < 0 || A[x][y] != '-')
		{
			printf("Invalid coordinates. Repeat the input: ");
			continue;

		}

		if ((x != 0) && (A[x - 1][y] != '-')) p = 1;
		if ((x != m - 1) && (A[x + 1][y] != '-')) p = 1;
		if ((y != 0) && (A[x][y - 1] != '-')) p = 1;
		if ((y != n - 1) && (A[x][y + 1] != '-')) p = 1;



		if (p) {
			A[x][y] = 'o';
			(*node_num)++;
			g=add_node(g, *node_num, x, y);
			break;
		}
		else {
			printf("Invalid coordinates. Repeat the input: ");
		}
	}
	return g;
}

/*Remove field (node)*/
Pointer remove_neighbour(Pointer h, int x, int y) {
	Pointer tmp=h, prev=h,old;
	while (tmp) {
		if (tmp->x == x && tmp->y==y) {
			if (tmp == h) h = h->next;
			else
				prev->next = tmp->next;
			old = tmp;
			free(old);
			return h;
		}
		else {
			prev = tmp;
			tmp = tmp->next;
		}
	}
	return h;
}
Pointer remove_field(char**A, Pointer g, int *node_num) {
	int x, y;
	int id;

	printf("Enter coordinates of the field that needs to be deleted: ");
	while (1) {
		scanf("%d%d", &x, &y);

		if ((id=find_node_id(g, *node_num, x, y)) == -1) {
			printf("A field with such coordinates doesn't exist. Repeat the input: ");
			continue;
		}
		
		if (neighbour_count(g[id].next) > 3) {
			printf("Field must be located at the map's edge. Repeat the input: ");			
			continue;
		}

		if ((g[id].label == 'S') || (g[id].label == 'T')) {
			printf("This field can't be deleted. Repeat the input: ");
			continue;
		}

		A[x][y] = '-';

		int id = find_node_id(g, *node_num, x, y);
		delete_list(g[id].next);

		(*node_num)--;

		for (int i = id; i < (*node_num); i++) {
			g[i] = g[i + 1];
		}
		g = realloc(g, (*node_num) * sizeof(Node));

		int tmp_id;
		if ((tmp_id = find_node_id(g, *node_num, x - 1, y)) != -1) {
			g[tmp_id].next = remove_neighbour(g[tmp_id].next, x, y);
		}
		if ((tmp_id = find_node_id(g, *node_num, x, y + 1)) != -1) {
			g[tmp_id].next = remove_neighbour(g[tmp_id].next, x, y);
		}
		if ((tmp_id = find_node_id(g, *node_num, x + 1, y)) != -1) {
			g[tmp_id].next = remove_neighbour(g[tmp_id].next, x, y);
		}
		if ((tmp_id = find_node_id(g, *node_num, x, y - 1)) != -1) {
			g[tmp_id].next = remove_neighbour(g[tmp_id].next, x, y);
		}

		break;






	}
	return g;

}




void print_map(char** A, int m, int n) {
	if (!A) {
		printf("Map needs to be loaded first.\n");
		return;
	}
	for (int i = 0; i < m; i++) {
		putchar('\t'); 		putchar('\t');

		for (int j = 0; j < n; j++) {
			switch (A[i][j]) {
			case 'o': cyan(); break;

			case 'S': case 'T': boldblue(); break;
			//case '#': boldblue(); break;
			}
			printf("%c", A[i][j]);
			reset();

		}
		putchar('\n');
	}
}

/*Print all nodes*/
void print_nodes(Pointer g, int n) {

	printf("Number of nodes: %d\n", n);

	for (int i = 0; i < n; i++) {
		printf("Node %d:\n\tCoordinates: (%d, %d)\n", i + 1, g[i].x, g[i].y);
		if (g[i].next) {
			printf("\tIts neighbours:\n");
			Pointer tmp = g[i].next;
			while (tmp) {
				printf("\t\tCoordinates: (%d, %d)\n", tmp->x, tmp->y);
				tmp = tmp->next;

			}

		}

		else printf("No neihbours\n");
	}
}

/*Moving*/
int move_up(Pointer g, int node_num, int*pos_1, int*pos_2, enum State * state) {
	switch (*state)
	{
	case straight: 
		if ((((*pos_1) = find_node_id(g, node_num, g[*pos_1].x - 1, g[*pos_1].y)) != -1)
			&& (((*pos_2) = find_node_id(g, node_num, g[*pos_1].x - 1, g[*pos_1].y)) != -1)) {

			(*state) = vertical;
			
		}
		else return 0;
		break;

	case vertical:
		if (((*pos_1) = find_node_id(g, node_num, g[*pos_2].x - 1, g[*pos_2].y)) != -1) {
			(*pos_2) = -1;
			(*state) = straight;
		}
		else return 0;
		break;

	case horizontal:
		if ((((*pos_1) = find_node_id(g, node_num, g[*pos_1].x - 1, g[*pos_1].y)) != -1) &&
			(((*pos_2) = find_node_id(g, node_num, g[*pos_2].x - 1, g[*pos_2].y)) != -1)) {
			(*state) = horizontal;
		}
		else return 0;
		break;


	}
	return 1;
}
int move_down(Pointer g, int node_num, int*pos_1, int*pos_2, enum State * state) {
	switch (*state)
	{
	case straight:
		if ((((*pos_2) = find_node_id(g, node_num, g[*pos_1].x + 1, g[*pos_1].y)) != -1) &&
			(((*pos_1) = find_node_id(g, node_num, g[*pos_2].x + 1, g[*pos_2].y)) != -1)) {

			(*state) = vertical;
		}
		else return 0;
		break;

	case vertical:
		if (((*pos_1) = find_node_id(g, node_num, g[*pos_1].x + 1, g[*pos_1].y)) != -1) {
			(*pos_2) = -1;
			(*state) = straight;
		}
		else return 0;
		break;

	case horizontal:
		if ((((*pos_1) = find_node_id(g, node_num, g[*pos_1].x + 1, g[*pos_1].y)) != -1) &&
			(((*pos_2) = find_node_id(g, node_num, g[*pos_2].x + 1, g[*pos_2].y)) != -1)) {
			(*state) = horizontal;
		}
		else return 0;
		break;

	}
	return 1;

}
int move_right(Pointer g, int node_num, int*pos_1, int*pos_2, enum State * state) {
	switch (*state)
	{
	case straight: 
		if ((((*pos_1) = find_node_id(g, node_num, g[*pos_1].x, g[*pos_1].y + 1)) != -1) &&
			(((*pos_2) = find_node_id(g, node_num, g[*pos_1].x, g[*pos_1].y + 1)) != -1)) {

			(*state) = horizontal;
		}
		else return 0;
		break;

	case vertical:
		if ((((*pos_1) = find_node_id(g, node_num, g[*pos_1].x, g[*pos_1].y + 1)) != -1) &&
			(((*pos_2) = find_node_id(g, node_num, g[*pos_2].x, g[*pos_2].y + 1)) != -1)) {

			(*state) = vertical;
		}
		else return 0;
		break;

	case horizontal:
		if (((*pos_1) = find_node_id(g, node_num, g[*pos_2].x, g[*pos_2].y + 1)) != -1) {
			(*pos_2) = -1;
			(*state) = straight;
		}
		else return 0;
		break;

	
	}
	return 1;
}

int move_left(Pointer g, int node_num, int*pos_1, int*pos_2, enum State * state) {
	switch (*state)
	{
	case straight:
		if ((((*pos_2) = find_node_id(g, node_num, g[*pos_1].x, g[*pos_1].y - 1)) != -1) &&
			(((*pos_1) = find_node_id(g, node_num, g[*pos_2].x, g[*pos_2].y - 1)) != -1)) {

			(*state) = horizontal;
		}
		else return 0;
		break;
	case vertical:
		if ((((*pos_1) = find_node_id(g, node_num, g[*pos_1].x, g[*pos_1].y - 1)) != -1) &&
			(((*pos_2) = find_node_id(g, node_num, g[*pos_2].x, g[*pos_2].y - 1)) != -1)) {

			(*state) = vertical;
		}
		else return 0;
		break;
	case horizontal:
		if (((*pos_1) = find_node_id(g, node_num, g[*pos_1].x, g[*pos_1].y - 1)) != -1) {
			(*pos_2) = -1;
			(*state) = straight;
		}
		else return 0;
		break;


	}
	return 1;

}

void gameplay(char**A, int m, int n, Pointer g, int node_num) {
	
	enum State state = straight;
	int i;
	int k=0;
	int game_over = 1;
	
	char c;
	int s = find_node_id_by_label(g, node_num, 'S');
	int t = find_node_id_by_label(g, node_num, 'T');

	A[g[s].x][g[s].y] = '#';

	int pos_1=s, pos_2 = -1;

	for (i = 0; i < 35; i++) putchar('\n');
	printf("Game:\t\t\t\t\t\tMove count: %d\n", k);
	print_map(A, m, n);
	putchar('\n');
	printf("[W] - up\n");
	printf("[A] - left\n");
	printf("[S] - down\n");
	printf("[D] - right\n\n");
	printf("[0] - give up\n");


	getchar();

	while (1) {
		if (pos_1 == s) {
			A[g[s].x][g[s].y] = 'S';
		}
		else
		if (pos_1 == t) {
			A[g[t].x][g[t].y] = 'T';
		} 
		else A[g[pos_1].x][g[pos_1].y] = 'o';

		if (pos_2 == s) {
			A[g[s].x][g[s].y] = 'S';
		}
		else
			if (pos_2 == t) {
				A[g[t].x][g[t].y] = 'T';
			}
			else
				if (state) A[g[pos_2].x][g[pos_2].y] = 'o';


		c = _getch();
		switch (c) {
		case 'w': case 'W': 
			game_over=move_up(g, node_num, &pos_1, &pos_2, &state);
			break;
			
		case 'a': case 'A': 
			game_over=move_left(g, node_num, &pos_1, &pos_2, &state);
			break;
		case 's': case 'S': 
			game_over=move_down(g, node_num, &pos_1, &pos_2, &state); 
			break;
		case 'd': case 'D': 
			game_over=move_right(g, node_num, &pos_1, &pos_2, &state);
			break;
		case '0': game_over = 0; break;
		default: continue;


		}

		if (!game_over) {
			for (i = 0; i < 45; i++) putchar('\n');
			printf("Game:\t\t\t\t\t\tMove count: %d\n", ++k);
			print_map(A, m, n);
			printf("\nGame over.\n");
			break;
		}

		A[g[pos_1].x][g[pos_1].y] = '#';
		if (state) A[g[pos_2].x][g[pos_2].y] = '#';
		

		for (i = 0; i < 45; i++) putchar('\n');
		printf("Game:\t\t\t\t\t\tMove count: %d\n", ++k);
		print_map(A, m, n);

		
		

		if (!state && pos_1 == t) {
			printf("\nWin!\n");
			A[g[t].x][g[t].y] = 'T';
			break;
		}
		else {
			printf("[W] - up\n");
			printf("[A] - left\n");
			printf("[S] - down\n");
			printf("[D] - right\n\n");
			printf("[0] - give up\n");
		}

	}
	
}

/*Queue operations*/
Queue insert_in_queue(Queue h, enum state state, int pos_1, int pos_2) {

	Queue tmp;
	tmp = malloc(sizeof(Queue_Elem));
	tmp->state = state;
	tmp->pos_1 = pos_1;
	tmp->pos_2 = pos_2;
	tmp->next = NULL;

	if (h == NULL) {
		return tmp;
	}
	else {
		Queue ptr = h;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = tmp;
		return h;
	}
}
Queue delete_from_queue(Queue Q,enum state * state, int * pos_1, int * pos_2) {

	if (queue_empty(Q)) return Q;

	(*state) = Q->state;
	(*pos_1) = Q->pos_1;
	(*pos_2) = Q->pos_2;

	Q = Q->next;
	

	return Q;
}

/*Movement reconstruction*/
char * reconstruction(Pointer g,int node_num, Visit_Vector V) {
	char *s;

	int tmp_pos_1,pos_1 = find_node_id_by_label(g, node_num, 'T');
	int tmp_pos_2,pos_2 = -1;
	enum State tmp_state, state = straight;


	int step = V[pos_1].str;
	s = calloc(step, sizeof(char));

	step--;
	s[step] = '\0';


	while (step) {
		
			switch (state)
			{
			case straight: {

				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_up(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].ver == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 's';

						break;
					}

				}


				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_right(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].hor == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'a';

						break;
					}

				}

				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_down(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].ver == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'w';

						break;
					}
					


				}


				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_left(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].hor == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'd';

						break;
					}
				}

				break;
			}

			case vertical: {

				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_up(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].str == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 's';

						break;
					}
				}





				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_right(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].ver == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'a';

						break;
					}
				}

				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_down(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].str == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'w';

						break;
					}
				}


				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_left(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].ver == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'd';

						break;
					}
				}

				break;
			}
			
			case horizontal: {

				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_up(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].hor == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 's';

						break;
					}
				}


				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_right(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].str == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'a';

						break;
					}
				}

				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_down(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].hor == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'w';

						break;
					}

				}


				tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
				if (move_left(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
					if (V[tmp_pos_1].str == step) {

						state = tmp_state;
						pos_1 = tmp_pos_1;
						pos_2 = tmp_pos_2;

						step--;
						s[step] = 'd';

						break;
					}
				}

				break;
			}
			}
	}

	return s;
}

/*BFS*/
char * solution(char**A, int m, int n, Pointer g, int node_num) {
	char * moves = NULL;

	/*Queue initialization*/
	Queue Q = NULL;
	int q_num = 0;

	/*Visit vector initialization*/
	Visit_Vector visit;
	visit = calloc(node_num, sizeof(Element));
	int i;
	for (i = 0; i < node_num; i++) {
		visit[i].str = 0;
		visit[i].hor = 0;
		visit[i].ver = 0;
	}

	enum state state, tmp_state;
	int pos_1, tmp_pos_1;
	int pos_2, tmp_pos_2;


	int s = find_node_id_by_label(g, node_num, 'S');
	visit[s].str = 1;

	Q = insert_in_queue(Q, straight, s, -1);

	int v = 1;
	int p = 0;
	while (!queue_empty(Q) && (!p)) {
		Q = delete_from_queue(Q, &state, &pos_1, &pos_2);

		switch (state)
		{
		case straight: {
			v = visit[pos_1].str;

			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_up(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].ver == 0) || (v + 1 < visit[tmp_pos_1].ver)) {
					visit[tmp_pos_1].ver = v + 1;
					Q = insert_in_queue(Q, vertical, tmp_pos_1, tmp_pos_2);
				}

			}


			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_right(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].hor == 0) || (v + 1 < visit[tmp_pos_1].hor)) {
					visit[tmp_pos_1].hor = v + 1;
					Q = insert_in_queue(Q, horizontal, tmp_pos_1, tmp_pos_2);
				}
			}

			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_down(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].ver == 0) || (v + 1 < visit[tmp_pos_1].ver)) {
					visit[tmp_pos_1].ver = v + 1;
					Q = insert_in_queue(Q, vertical, tmp_pos_1, tmp_pos_2);
				}

			}


			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_left(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].hor == 0) || (v + 1 < visit[tmp_pos_1].hor)) {
					visit[tmp_pos_1].hor = v + 1;
					Q = insert_in_queue(Q, horizontal, tmp_pos_1, tmp_pos_2);
				}
			}

			break;
		}



		case vertical: {
			v = visit[pos_1].ver;

			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_up(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].str == 0) || (v + 1 < visit[tmp_pos_1].str)) {
					visit[tmp_pos_1].str = v + 1;
					if (g[tmp_pos_1].label == 'T') {
						p = 1;
					}

					Q = insert_in_queue(Q, straight, tmp_pos_1, tmp_pos_2);
				}
			}





			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_right(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].ver == 0) || (v + 1 < visit[tmp_pos_1].ver)) {
					visit[tmp_pos_1].ver = v + 1;
					Q = insert_in_queue(Q, vertical, tmp_pos_1, tmp_pos_2);
				}
			}

			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_down(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].str == 0) || (v + 1 < visit[tmp_pos_1].str)) {
					visit[tmp_pos_1].str = v + 1;
					if (g[tmp_pos_1].label == 'T') {

						//	printf("Found path")\n");	
						p = 1;

					}
					Q = insert_in_queue(Q, straight, tmp_pos_1, tmp_pos_2);
				}

			}


			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_left(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].ver == 0) || (v + 1 < visit[tmp_pos_1].ver)) {
					visit[tmp_pos_1].ver = v + 1;
					Q = insert_in_queue(Q, vertical, tmp_pos_1, tmp_pos_2);
				}
			}

			break;
		}
		case horizontal: {
			v = visit[pos_1].hor;

			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_up(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].hor == 0) || (v + 1 < visit[tmp_pos_1].hor)) {
					visit[tmp_pos_1].hor = v + 1;
					Q = insert_in_queue(Q, horizontal, tmp_pos_1, tmp_pos_2);
				}
			}





			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_right(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].str == 0) || (v + 1 < visit[tmp_pos_1].str)) {
					visit[tmp_pos_1].str = v + 1;
					if (g[tmp_pos_1].label == 'T') {

						p = 1;

					}
					Q = insert_in_queue(Q, straight, tmp_pos_1, tmp_pos_2);
				}
			}

			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_down(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].hor == 0) || (v + 1 < visit[tmp_pos_1].hor)) {
					visit[tmp_pos_1].hor = v + 1;
					Q = insert_in_queue(Q, horizontal, tmp_pos_1, tmp_pos_2);
				}

			}


			tmp_state = state;	tmp_pos_1 = pos_1; tmp_pos_2 = pos_2;
			if (move_left(g, node_num, &tmp_pos_1, &tmp_pos_2, &tmp_state)) {
				if ((visit[tmp_pos_1].str == 0) || (v + 1 < visit[tmp_pos_1].str)) {
					visit[tmp_pos_1].str = v + 1;
					if (g[tmp_pos_1].label == 'T') {

						p = 1;

					}
					Q = insert_in_queue(Q, straight, tmp_pos_1, tmp_pos_2);
				}
			}

			break;
		}
		}

		if (p == 0) {
			if (queue_empty(Q))
				printf("\n It's impossible to find path for this map.\n");
		}
		else {
			moves = reconstruction(g, node_num, visit);
		}
	}
	return moves;
}

void simulation(char**A, int m, int n, Pointer g, int node_num,char *moves) {
	int i;
	printf("\nSeries of moves that lead to victory:\n\n");
	for (i = 0; moves[i] != '\0'; i++) {
		switch (moves[i])
		{
		case 'w': printf("[W] Up\n"); break;
		case 'a': printf("[A] Left\n"); break;
		case 's': printf("[S] Down\n"); break;
		case 'd': printf("[D] Right\n"); break;
		}
	}
	

	printf("\n1. Simulation\n2. Back to menu\n");
	
	scanf("%d", &i);
	if (i != 1) return;


	enum State state = straight;
	int j=0,game_over;
	int k = 0;
	time_t cnt;

	char c;
	int s = find_node_id_by_label(g, node_num, 'S');
	int t = find_node_id_by_label(g, node_num, 'T');

	A[g[s].x][g[s].y] = '#';

	int pos_1 = s, pos_2 = -1;

	for (i = 0; i < 35; i++) putchar('\n');
	printf("Simulation:\t\t\t\t\t\tMove Count: %d\n", k);
	print_map(A, m, n);
	putchar('\n');
	printf("Next Move: ");
	switch (moves[j])
	{
	case 'w': printf("[W] Up\n"); break;
	case 'a': printf("[A] Left\n"); break;
	case 's': printf("[S] Down\n"); break;
	case 'd': printf("[D] Right\n"); break;
	}
	cnt = time(NULL);
	while (time(NULL) < cnt + 1);


	getchar();

	while ((c = moves[j++])!='\0') {
		if (pos_1 == s) {
			A[g[s].x][g[s].y] = 'S';
		}
		else
			if (pos_1 == t) {
				A[g[t].x][g[t].y] = 'T';
			}
			else A[g[pos_1].x][g[pos_1].y] = 'o';

		if (pos_2 == s) {
			A[g[s].x][g[s].y] = 'S';
		}
		else
			if (pos_2 == t) {
				A[g[t].x][g[t].y] = 'T';
			}
			else
				if (state) A[g[pos_2].x][g[pos_2].y] = 'o';



		switch (c) {
		case 'w': case 'W':
			move_up(g, node_num, &pos_1, &pos_2, &state);
			break;

		case 'a': case 'A':
			move_left(g, node_num, &pos_1, &pos_2, &state);
			break;
		case 's': case 'S':
			move_down(g, node_num, &pos_1, &pos_2, &state);
			break;
		case 'd': case 'D':
			move_right(g, node_num, &pos_1, &pos_2, &state);
			break;
		case '0': game_over = 1; break;
		default: continue;


		}

		A[g[pos_1].x][g[pos_1].y] = '#';
		if (state) A[g[pos_2].x][g[pos_2].y] = '#';

	
		cnt = time(NULL);
		while (time(NULL) < cnt + 1);

		for (i = 0; i < 45; i++) putchar('\n');
		printf("Simulation:\t\t\t\t\t\tMove Count: %d\n", ++k);

		print_map(A, m, n);
		putchar('\n');

		
		
		if (!state && pos_1 == t) {
			
			A[g[t].x][g[t].y] = 'T';
			break;
		} 
		else {
			printf("Next move: ");
			switch (moves[j])
			{
			case 'w': printf("[W] Up\n"); break;
			case 'a': printf("[A] Left\n"); break;
			case 's': printf("[S] Down\n"); break;
			case 'd': printf("[D] Right\n"); break;
			}

		}

	}

}


void main(void) {

	int x;
	int m = 0, n = 0, p = 0;
	int node_num;
	char *moves;

	char **map = NULL;
	Pointer g = NULL;

	while (1) {
		printf(" _______________________________________________________________________________\n");
		printf("|                                 [BLOXORZ]                                     |\n");
		printf("|                                                                               |\n");
		printf("| 1. Load new map                                                               |\n");
		printf("| 2. Add field at the edge of the map                                           |\n");
		printf("| 3. Remove field from the edge of the map                                      |\n");
		printf("| 4. Play the game                                                              |\n");
		printf("| 5. Solve the game                                                             |\n");
		printf("|                                                                               |\n");
		printf("| 0. Quit                                                                       |\n");
		printf("|_______________________________________________________________________________|\n");

		scanf("%d", &x);

		switch (x)
		{
		case 1: {
			if (p == 1) {
				delete_map(map, m);
				delete_graph(g, node_num);
				p = 0;
			}
			map = load_map(&m, &n);
			print_map(map, m, n);
			g = create_graph(map, m, n, &node_num);
			p = 1;
			
			break;
		}
		case 2: {
			if (p == 0) {
				printf("First load the map.\n");
				map = load_map(&m, &n);
				g = create_graph(map, m, n, &node_num);
			}
			g = add_field(map, m, n, g, &node_num);
			print_map(map, m, n);
			//	print_nodes(g, node_num);
			//	print_map(map, m, n);
			break;
		}


		case 3:
			if (p == 0) {
				printf("First load the map.\n");
				map = load_map(&m, &n);
				g = create_graph(map, m, n, &node_num);
			}
			remove_field(map, g, &node_num);
			print_map(map, m, n);
			//	print_nodes(g, node_num);
			//	print_map(map, m, n);

			break;
		case 4:
			if (p == 0) {
				printf("First load the map.\n");
				map = load_map(&m, &n);
				g = create_graph(map, m, n, &node_num);
			}

			gameplay(map, m, n, g, node_num);
			break;
		case 5:
			if (p == 0) {
				printf("First load the map.\n");
				map = load_map(&m, &n);
				g = create_graph(map, m, n, &node_num);
			}

			moves = solution(map, m, n, g, node_num);
			if (moves) simulation(map, m, n, g, node_num, moves);
			break;

		case 0: {
			if (p == 1) {
				delete_map(map, m);
				delete_graph(g, node_num);
			}
			printf("Goodbye.\n");
			exit(1);
			break;
		}
		default:
			printf("Repeat the input.\n");
			break;
		
		}
	}
}