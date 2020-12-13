#include <vector>
#include <thread>
#include <iostream>

using namespace std;

//19 вариант

class Bear {
public:
	Bear() {}

	/*
	отправляет Винни-пуха опустошать улей,
	если пчел было меньше 3 забирает весь мед,
	иначе Винни-пух уходит ни с чем
	*/
	void devastate(int& content, int& beesIn) {
		if (beesIn < 3 && content > 14) {
			cout << "bees in: " << beesIn << endl;
			cout << "Vinni-puh devastated hive" << endl;
			cout << "Honey in hive: 0" << endl;
			content = 0;
		}
		else if (content > 14) {
			cout << "bees in: " << beesIn << endl;
			cout << "Vinni-puh was bitten by bees" << endl;
			cout << "Honey in hive: " << content << endl;
		}

	}
};


class Bee {
private:
	int beesQuant;
public:
	Bee(int BeesQuant) {
		this->beesQuant = BeesQuant;
	}

	/*
	метод отвечающий за вылет пчелы
	не выпускает пчелу, если она единственная в улии или количество меда достигло максимума
	если пчела вылетела, ее поток засыпает (время добычи меда)
	*/
	void goOut(int& content, int& beesIn) {
		if (content + (beesQuant - beesIn) < 30 && beesIn > 1) {
			beesIn -= 1;
			int a = rand() % 4 + 1;
			if (a == 1)
				this_thread::sleep_for(1s);
			if (a == 2)
				this_thread::sleep_for(2s);
			if (a == 3)
				this_thread::sleep_for(3s);
			if (a == 4)
				this_thread::sleep_for(4s);
			content += 1;
			if (content > 30)
				content = 30;
			if (beesIn < beesQuant)
				beesIn += 1;
			if (beesIn > beesQuant)
				beesIn = beesQuant;
		}
	}
};

/*
ждет, пока Винни-пух проснется и улей заполнится хотя бы на половину и отправляет медведя опустошать улей
*/
void StartBear(Bear bear, int& content, int& beesIn) {
	cout << "Vinni-puh sleeps" << endl;
	while (true) {
		while (content < 15) {
			this_thread::sleep_for(1s);
		}
		bear.devastate(content, beesIn);
		cout << "Vinni-puh sleeps" << endl;
		int a = rand() % 4 + 3;
		if (a == 3)
			this_thread::sleep_for(3s);
		if (a == 4)
			this_thread::sleep_for(4s);
		if (a == 5)
			this_thread::sleep_for(5s);
		if (a == 6)
			this_thread::sleep_for(6s);
	}
}

/*
вызывает метод, проверяющий возможность пчелы вылететь и отправдяющий ее, если возможно
*/
void StartBee(Bee bee, int& content, int& beesIn) {
	while (true) {
		bee.goOut(content, beesIn);
	}
}

/*
создает вектор из заданного количествва пчел
*/
void CreateBees(int beesQuant, vector<Bee>& bees) {
	for (int i = 0; i < beesQuant; i++) {
		bees.push_back(Bee(beesQuant));
	}
}

/*
отсчитывает время для работы программы, по истечении 35 секунд останавливает ее работу
*/
void Timer() {
	this_thread::sleep_for(35s);
	exit(0);
}


int main() {
	int beesQuant; //количество пчел
	cout << "Enter quantity of bees: ";
	cin >> beesQuant;
	while (beesQuant < 4) {
		cout << "Quantity of bees must be more than 3" << endl;
		cout << "Enter quantity of bees: ";
		cin >> beesQuant;
	}

	int content = 0; //количество меда в улье
	int beesIn = beesQuant; //количество пчел в улье

	vector<Bee> bees;
	CreateBees(beesQuant, bees);
	thread* vec_bees = new thread[beesQuant];
	for (int i = 0; i < bees.size(); i++) {
		vec_bees[i] = thread(StartBee, bees[i], ref(content), ref(beesIn));
	}

	Bear bear = Bear();
	thread* th_bear = new thread(StartBear, bear, ref(content), ref(beesIn));

	thread* th = new thread(Timer);
	
	th->join();
	for (int i = 0; i < beesQuant; i++) {
		vec_bees[i].join();
	}
	th_bear[0].join();

}