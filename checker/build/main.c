#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

const int lines_nr = 3, nr_simb = 10;

typedef struct {
	int x, y, cost;
} Node;

typedef struct {
	Node *heap;
	int size;
	int capacity;
} PriorityQueue;

PriorityQueue* createPQ(int capacity) {
	PriorityQueue *pq = malloc(sizeof(PriorityQueue));
	pq->heap = malloc(capacity * sizeof(Node));
	pq->size = 0;
	pq->capacity = capacity;
	return pq;
}

void swap(Node *a, Node *b) {
	Node temp = *a;
	*a = *b;
	*b = temp;
}

void heapifyUp(PriorityQueue *pq, int idx) {
	while (idx > 0) {
	int parent = (idx - 1) / 2;
	if (pq->heap[idx].cost < pq->heap[parent].cost) {
		swap(&pq->heap[idx], &pq->heap[parent]);
		idx = parent;
	} else {
		break;
	}
	}
}

void heapifyDown(PriorityQueue *pq, int idx) {
	while (1) {
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;
	
	if (left < pq->size && pq->heap[left].cost < pq->heap[smallest].cost)
		smallest = left;
	if (right < pq->size && pq->heap[right].cost < pq->heap[smallest].cost)
		smallest = right;
	
	if (smallest != idx) {
		swap(&pq->heap[idx], &pq->heap[smallest]);
		idx = smallest;
	} else {
		break;
	}
	}
}

void push(PriorityQueue *pq, int x, int y, int cost) {
	if (pq->size >= pq->capacity) return;
	pq->heap[pq->size].x = x;
	pq->heap[pq->size].y = y;
	pq->heap[pq->size].cost = cost;
	heapifyUp(pq, pq->size);
	pq->size++;
}

Node pop(PriorityQueue *pq) {
	Node result = pq->heap[0];
	pq->size--;
	pq->heap[0] = pq->heap[pq->size];
	heapifyDown(pq, 0);
	return result;
}

int isEmpty(PriorityQueue *pq) {
	return pq->size == 0;
}

int minim(int val1, int val2) {
	if (val1 > val2) {
		return val2;
	}
	return val1;
}
int modul(int val3, int val4) {
	if (val3 - val4 >= 0) {
		return val3 - val4;
	}
	return val4 - val3;
}

int dijkstra(int **lucky_matrix, int lines_nr, int cnt_columns) {

	int **dist = malloc(lines_nr * sizeof(int *));
	for (int i = 0; i < lines_nr; i++) {
	dist[i] = malloc(cnt_columns * sizeof(int));
	for (int j = 0; j < cnt_columns; j++) {
		dist[i][j] = INT_MAX;
	}
	}

	// Direcții
	int dx[] = { -1, 0, 1, 0 };
	int dy[] = { 0, 1, 0, -1 };

	PriorityQueue *pq = createPQ(lines_nr * cnt_columns * 4);

	// pornim din (0,0) EXACT ca în C++
	dist[0][0] = lucky_matrix[0][0];
	push(pq, 0, 0, dist[0][0]);

	while (!isEmpty(pq)) {

	Node current = pop(pq);
	int x = current.x;
	int y = current.y;
	int c = current.cost;

	// IMPORTANT: Skip node if outdated (lazy deletion, ca în set-ul C++)
	if (c != dist[x][y])
		continue;

	// Dacă am ajuns la final
	if (x == lines_nr - 1 && y == cnt_columns - 1) {

		int result = dist[x][y];

		for (int i = 0; i < lines_nr; i++)
		free(dist[i]);
		free(dist);

		free(pq->heap);
		free(pq);

		return result;
	}

	// Relaxăm vecinii
	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		if (nx < 0 || nx >= lines_nr || ny < 0 || ny >= cnt_columns)
		continue;

		int new_cost = dist[x][y] + lucky_matrix[nx][ny];

		if (new_cost < dist[nx][ny]) {
		dist[nx][ny] = new_cost;
		push(pq, nx, ny, new_cost);
		}
	}
	}

	// fallback (nu există drum)
	int result = -1;

	for (int i = 0; i < lines_nr; i++)
	free(dist[i]);
	free(dist);
	free(pq->heap);
	free(pq);

	return result;
}

void spin_specific_column(int **lucky_matrix, int cnt_lines, int cnt_columns, int col_id, int number_of_spins) {
	int *aux;
	aux = calloc(cnt_lines, sizeof(int));
	if (number_of_spins >= cnt_lines) {
		number_of_spins = number_of_spins % cnt_lines;
	}
	if (number_of_spins != 0) {
		for (int i = 0; i < cnt_lines; i++) {
			aux[i] = lucky_matrix[(i + cnt_lines - number_of_spins) % cnt_lines][col_id];
		}
		for (int i = 0; i < cnt_lines; i++) {
			lucky_matrix[i][col_id] = aux[i];
		}
		free(aux);
	}
}
void find_all_winning_combinations_and_calculate_score(int **lucky_matrix, int cnt_lines, int cnt_columns, int *winning_combination, int *score) {
	int is_diffrent = 0;
	int *frecv = calloc(nr_simb, sizeof(int));
	for (int i = 0; i < lines_nr; i++) {
		int sum_max = 0, val_max = 0, sum_current = 0, val_current = 0;
		for (int j = 0; j < cnt_columns; j++) {
			if (frecv[lucky_matrix[i][j]] == 0) {
				is_diffrent++;
				frecv[lucky_matrix[i][j]]++;
			}
			frecv[lucky_matrix[i][j]]++;
			if (i == 0 && j < cnt_columns - 2) {
				int forms_an_x = 0;
				if (lucky_matrix[i][j] == lucky_matrix[2 + i][j] && lucky_matrix[i][j] == lucky_matrix[i + 1][j + 1]) {
					if (lucky_matrix[i][j] == lucky_matrix[i + 2][j + 2] && lucky_matrix[i][j] == lucky_matrix[i][j + 2]) {
						forms_an_x = 1;
						(*winning_combination)++;
						(*score) += 21;
						if (lucky_matrix[i][j] == 7) {
							(*score) += 21;
						}
					}
				}
				if (forms_an_x == 0) {
					if (lucky_matrix[i][j] == lucky_matrix[i + 1][j + 1] && lucky_matrix[i][j] == lucky_matrix[i + 2][j + 2]) {
						(*winning_combination)++;
						(*score) += 7;
						if (lucky_matrix[i][j] == 7) {
							(*score) += 7;
						}
					} else if (lucky_matrix[i + 2][j] == lucky_matrix[i + 1][j + 1] && lucky_matrix[i + 2][j] == lucky_matrix[i][j + 2]) {
						(*winning_combination)++;
						(*score) += 7;
						if (lucky_matrix[i + 2][j] == 7) {
							(*score) += 7;
						}
					}
				}
			}
			if (j < cnt_columns - 1) {
				if (lucky_matrix[i][j] == lucky_matrix[i][j + 1]) {
					if (sum_current == 0)
						sum_current++;
					sum_current++;
					val_current = lucky_matrix[i][j];
				} else {
					if (sum_current > sum_max) {
						sum_max = sum_current;
						val_max = val_current;
					}
					sum_current = 0;
				}
			}
		}
		if (sum_current > sum_max) {
			val_max = val_current;
			sum_max = sum_current;
		}
		if (sum_max > 2) {
			(*winning_combination)++;
			(*score) += sum_max;
			if (val_max == 7)
				(*score) += sum_max;
		}
	}
	if (is_diffrent <= 2) {
		(*score) += 100;
	} else if (is_diffrent <= 4) {
		(*score) += 15;
	}
	free(frecv);
}

int main(void) {
	int i, j, task_number, cnt_lines, cnt_columns;
	scanf("%d", &task_number);
	scanf("%d %d", &cnt_lines, &cnt_columns);
	int **lucky_matrix = malloc(cnt_lines * sizeof(int *));
	for (i = 0; i < cnt_lines; i++) {
		lucky_matrix[i] = calloc(cnt_columns, sizeof(int));
	}
	for (i = 0; i < cnt_lines; i++) {
		for (j = 0; j < cnt_columns; j++) {
			scanf("%d", *(lucky_matrix + i) + j);
		}
	}
	if (task_number == 1) {
		int col_id, number_of_spins;
		scanf("%d %d", &col_id, &number_of_spins);
		spin_specific_column(lucky_matrix, cnt_lines, cnt_columns, col_id, number_of_spins);
		for (i = 0; i < cnt_lines; i++) {
			for (j = 0; j < cnt_columns; j++) {
				printf("%d ", lucky_matrix[i][j]);
			}
			printf("\n");
		}
	} else if (task_number == 2) {
		int winning_combination = 0, score = 0;
		find_all_winning_combinations_and_calculate_score(lucky_matrix, cnt_lines, cnt_columns, &winning_combination, &score);
		printf("%d\n%d\n", winning_combination, score);
	} else if (task_number == 3) {
		int total_score = 0, nr_op;
		scanf("%d", &nr_op);
		for (i = 0; i < nr_op; i++) {
			int col_id, number_of_spins, winning_combination = 0, score = 0;
			scanf("%d %d", &col_id, &number_of_spins);
			spin_specific_column(lucky_matrix, cnt_lines, cnt_columns, col_id, number_of_spins);
			find_all_winning_combinations_and_calculate_score(lucky_matrix, cnt_lines, cnt_columns, &winning_combination, &score);
			total_score += score;
		}
		printf("%d\n", total_score);
	} else if (task_number == 4) {
		int castig_max = 0, castig_curent = 0, temp = 0;
		for (i = 0; i < lines_nr; i++) {
			for (j = 0; j < cnt_columns; j++) {
				for (int inl = 0; inl < 10; inl++) {
					castig_curent = 0;
					int aux = lucky_matrix[i][j];
					lucky_matrix[i][j] = inl;
					find_all_winning_combinations_and_calculate_score(lucky_matrix, cnt_lines, cnt_columns, &temp, &castig_curent);
					lucky_matrix[i][j] = aux;
					if (castig_curent > castig_max)
						castig_max = castig_curent;
				}
			}
		}
		printf("%d\n", castig_max);
	} else if (task_number == 5) {
		int score_spins_max = 0, score_spins_cur = 0, temp = 0;
		for (i = 0; i < cnt_columns; i++) {
			for (j = i + 1; j < cnt_columns; j++) {
				for (int k1 = 0; k1 < cnt_lines; k1++) {
					spin_specific_column(lucky_matrix, cnt_lines, cnt_columns, i, k1);
					for (int k2 = 0; k2 < cnt_lines; k2++) {
						score_spins_cur = 0;
						spin_specific_column(lucky_matrix, cnt_lines, cnt_columns, j, k2);
						find_all_winning_combinations_and_calculate_score(lucky_matrix, cnt_lines, cnt_columns, &temp, &score_spins_cur);
						if (score_spins_cur > score_spins_max) {
							score_spins_max = score_spins_cur;
						}
						spin_specific_column(lucky_matrix, cnt_lines, cnt_columns, j, cnt_lines - k2);
					}
					if (score_spins_cur > score_spins_max) {
						score_spins_max = score_spins_cur;
					}
					spin_specific_column(lucky_matrix, cnt_lines, cnt_columns, i, cnt_lines - k1);
				}
			}
		}
		printf("%d", score_spins_max);
		printf("\n");
	} else if (task_number == 6) {
		int simbol_initial = 0;
		int nr_of_lines = 3;
		// if we have something to scan, we scan and the value will be the "nr_of_lines"
		if (scanf("%d", &nr_of_lines) == 1) {
			printf("Sigur, vom calcula pe mai mult de 3 linii, adica pe %d linii!\n", nr_of_lines);
		}
		simbol_initial = lucky_matrix[0][0];
		int **mat_costurilor = malloc(cnt_lines * sizeof(int *));
		for (i = 0; i < cnt_lines; i++) {
			mat_costurilor[i] = calloc(cnt_columns, sizeof(int));
		}
		for (i = 0; i < cnt_lines; i++) {
			for (j = 0; j < cnt_columns; j++) {
				mat_costurilor[i][j] = modul(lucky_matrix[i][j], simbol_initial);
				//printf("%d ", mat_costurilor[i][j]);
			}
			//printf("\n");
		}
		int result = dijkstra(mat_costurilor, nr_of_lines, cnt_columns);
		printf("%d\n", result);
		for (i = 0; i < cnt_lines; i++) {
			free(mat_costurilor[i]);
		}
		free(mat_costurilor);
	} else {
		printf("Not implemented yet...\n");
	}
	for (i = 0; i < cnt_lines; i++) {
		free(lucky_matrix[i]);
	}
	free(lucky_matrix);
	return 0;
}
