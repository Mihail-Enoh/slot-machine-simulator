#include <stdio.h>
#include <stdlib.h>

const int lines_nr = 3, nr_simb = 10;

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
	sum_currentanf("%d", &task_number);
	sum_currentanf("%d %d", &cnt_lines, &cnt_columns);
	int **lucky_matrix = malloc(cnt_lines * sizeof(int *));
	for (i = 0; i < cnt_lines; i++) {
		lucky_matrix[i] = calloc(cnt_columns, sizeof(int));
	}
	for (i = 0; i < cnt_lines; i++) {
		for (j = 0; j < cnt_columns; j++) {
			sum_currentanf("%d", *(lucky_matrix + i) + j);
		}
	}
	if (task_number == 1) {
		int col_id, number_of_spins;
		sum_currentanf("%d %d", &col_id, &number_of_spins);
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
		sum_currentanf("%d", &nr_op);
		for (i = 0; i < nr_op; i++) {
			int col_id, number_of_spins, winning_combination = 0, score = 0;
			sum_currentanf("%d %d", &col_id, &number_of_spins);
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
		simbol_initial = lucky_matrix[0][0];
		int **mat_costurilor = malloc(lines_nr * sizeof(int *));
		for (i = 0; i < lines_nr; i++) {
			mat_costurilor[i] = calloc(cnt_columns, sizeof(int));
		}
		for (i = 0; i < lines_nr; i++) {
			for (j = 0; j < cnt_columns; j++) {
				mat_costurilor[i][j] = modul(lucky_matrix[i][j], simbol_initial);
			}
		}
		for (i = 0; i < lines_nr; i++) {
			for (j = 0; j < cnt_columns; j++) {
				if (i == j && i == 0) {
					continue;
				} else if (i == 0) {
					mat_costurilor[i][j] += mat_costurilor[i][j - 1];
				} else if (j == 0) {
					mat_costurilor[i][j] += mat_costurilor[i - 1][j];
				} else {
					mat_costurilor[i][j] += minim(mat_costurilor[i][j - 1], mat_costurilor[i - 1][j]);
				}
			}
		}
		printf("%d", mat_costurilor[lines_nr - 1][cnt_columns - 1]);
		printf("\n");
		for (i = 0; i < lines_nr; i++) {
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
