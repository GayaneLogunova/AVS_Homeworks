#include <iostream>
#include <omp.h>

using namespace std;

int desicion = -1;
// вариант 19


void server(int N) {
	int check_desicion;
	do {
		#pragma omp critical 
		{
		cout << "Введите № выбранной валентинки: ";
		}
		cin >> check_desicion;
		cout << endl;
		if (check_desicion > N) {
			cout << "Номер выбранной валентинки не может превышать общее количесвто валентинок: " << endl;
		}
	} while (check_desicion > N);
	desicion = check_desicion;
}

void client(int my_number) {

	while (desicion == -1) {
		continue;
	}
	if (desicion != my_number) {
		#pragma omp critical
		{
			cout << "Валентинка №" << my_number << " отклонена" << endl;
		}
	}
	else {
		#pragma omp critical 
		{
			cout << "валентинка №" << my_number << " принята" << endl;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	int N;
	cout << "Введите количество валентинок N: ";
	cin >> N;
	
	server(N);
	
	#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		client(i);
	}
}

