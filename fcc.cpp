#include "pch.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_LENGTH 1000

bool* funcdef;
unsigned int N;

unsigned int get_defvector_length(const int n) {
	if (n < 0) {
		printf("\nFatal error: attempted to refer to a bool func with a negative number of arguments.\n");
		exit(EXIT_FAILURE);
	}
	unsigned int length = 1;
	for (unsigned int i = n; i > 0; i--)
		length *= 2;
	return length;
}

void print_vector(const bool* vector, const unsigned int size) {
	if (!vector) {
		printf("\nFatal error: attempted to refer to a null vector.\n");
		exit(EXIT_FAILURE);
	}
	if (!size) {
		printf("\nFatal error: attempted to refer to a null-size vector.\n");
		exit(EXIT_FAILURE);
	}
	for (unsigned int i = 0; i < size; i++)
		printf("%d", vector[i]);
}

bool can_increment_vector(const bool* vector, const unsigned int size) {
	if (!vector) {
		printf("\nFatal error: attempted to refer to a null vector.\n");
		exit(EXIT_FAILURE);
	}
	if (!size) {
		printf("\nFatal error: attempted to refer to a null-size vector.\n");
		exit(EXIT_FAILURE);
	}
	for (unsigned int i = 0; i < size; i++)
		if (vector[i] == 0)
			return true;
	return false;
}

void increment_vector(bool* vector, const unsigned int size) {
	if (!vector) {
		printf("\nFatal error: attempted to refer to a null vector.\n");
		exit(EXIT_FAILURE);
	}
	if (!size) {
		printf("\nFatal error: attempted to refer to a null-size vector.\n");
		exit(EXIT_FAILURE);
	}
	if (!can_increment_vector(vector, size)) {
		printf("\nFatal error: attempted to increment a vector of 1's.\n");
		exit(EXIT_FAILURE);
	}
	for (unsigned int i = size - 1; i >= 0; i--) {
		if (vector[i] == 0) {
			vector[i] = 1;
			for (unsigned int j = i + 1; j < size; j++)
				vector[j] = 0;
			return;
		}
	}
}

unsigned int bin_to_int(const bool* bin, const unsigned int size) {
	if (!bin) {
		printf("\nFatal error: attempted to refer to a null vector.\n");
		exit(EXIT_FAILURE);
	}
	if (!size) {
		printf("\nFatal error: attempted to refer to a null-size vector.\n");
		exit(EXIT_FAILURE);
	}
	unsigned int result = 0;
	for (int i = size - 1; i >= 0; i--)
		result += get_defvector_length(size - 1 - i) * (unsigned int)bin[i];
	return result;
}

bool get_scalar_product(const bool* a, const bool* x, const unsigned int size) {
	if (!a || !x) {
		printf("\nFatal error: attempted to refer to a null vector.\n");
		exit(EXIT_FAILURE);
	}
	if (!size) {
		printf("\nFatal error: attempted to refer to a null-size vector.\n");
		exit(EXIT_FAILURE);
	}
	bool product = 0;
	for (unsigned int i = 0; i < size; i++)
		product = product ^ (a[i] && x[i]);
	return product;
}

bool* bin_parse(char* str, unsigned int expected_length) {
	if (!str) {
		printf("\nFatal error: attempted to refer to an empty string.\n");
		exit(EXIT_FAILURE);
	}
	unsigned int length;
	for (length = 0; str[length] != '\0'; length++)
		if (str[length] != '1' && str[length] != '0') {
			printf("Fatal error: the definition vector contains illegal characters.\n");
			exit(EXIT_FAILURE);
		}
	if (length - get_defvector_length(expected_length)) {
		printf("Fatal error: the definition vector doesn't match the number of arguments.\n");
		exit(EXIT_FAILURE);
	}
	bool* bin_arr = (bool*)calloc(length, sizeof(bool));
	for (unsigned int i = 0; i < length; i++)
		bin_arr[i] = (str[i] == '0') ? false : true;
	return bin_arr;
}

int main(int argc, char* argv[]) {
	char* buffer = (char*)calloc(MAX_INPUT_LENGTH, sizeof(char));
	if (argc != 3) {
		printf("\n\n-------------------------------------------\n"
			"Calculates Fourier coefficients for a function with N arguments defined by its values vector.\n"
			"\nFCC N vector\n"
			"\n	N		Number of function arguments."
			"\n	vector		Function definition vector.\n"
			"\nExample:\n"
			"\nFCC 3 01101001\n"
			"\nMake sure the definition vector matches the specified number of function arguments.\n"
			"-------------------------------------------\n\n");
		printf("Entering manual input mode.\nInput the number of arguments: ");
		scanf_s("%999s", buffer, MAX_INPUT_LENGTH);
		if(atoi(buffer) < 1) {
			printf("\nFatal error: failed to parse input.\n");
			exit(EXIT_FAILURE);
		}
		N = atoi(buffer);
		printf("Input func definition vector: ");
		scanf_s("%999s", buffer, MAX_INPUT_LENGTH);
		funcdef = bin_parse(buffer, N);
	}
	else {
		N = atoi(argv[1]);
		funcdef = bin_parse(argv[2], N);
	}
	printf("\n\nGot func definition vector from input: ");
	print_vector(funcdef, get_defvector_length(N));
	printf("\n\n");

	printf("Allocating memory...\n");
	int* fourier_coeffs = (int*)calloc(get_defvector_length(N), sizeof(int));
	bool* a = (bool*)calloc(N, sizeof(bool));

	printf("Calculating...\n");
	for (unsigned int j = 0; j < get_defvector_length(N); j++) {
		bool* x = (bool*)calloc(N, sizeof(bool));
		for (unsigned int i = 0; i < get_defvector_length(N); i++) {
			fourier_coeffs[j] += funcdef[bin_to_int(x, N)] * (get_scalar_product(a, x, N) ? -1 : 1);
			if (can_increment_vector(x, N))
				increment_vector(x, N);
		}
		if (can_increment_vector(a, N))
			increment_vector(a, N);
		free(x);
	}
	printf("\nSuccess!\n\nFourier coefficients for func are:\n\n");
	bool* ct = (bool*)calloc(N, sizeof(bool));
	for (unsigned int i = 0; i < get_defvector_length(N); i++) {
		printf("c(");
		print_vector(ct, N);
		printf(") = %d\n", fourier_coeffs[i]);
		if (can_increment_vector(ct, N))
			increment_vector(ct, N);
	}
	printf("\nPress Enter to quit.\n\n");
	getchar();
	getchar();

	free(ct);
	free(funcdef);
	free(a);

	return 0;
}
