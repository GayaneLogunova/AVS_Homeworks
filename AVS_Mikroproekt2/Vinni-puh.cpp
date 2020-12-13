#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>

using namespace std;
mutex mtx;
//19 вариант

class Bear {
public:
	Bear() {}

	string Time()
	{
		time_t seconds = time(NULL);
		tm* tInfo = localtime(&seconds);
		return asctime(tInfo);
	}
	/*
	отправляет Винни-пуха опустошать улей,
	если пчел было меньше 3 забирает весь мед,
	иначе Винни-пух уходит ни с чем
	*/
	void devastate(int& content, int& beesIn) {
		string time;
		if (beesIn < 3 && content > 14) {
			mtx.lock();
			cout << "bees in: " << beesIn << endl;
			time = Time(); 
			cout << time.substr(0, time.size() - 1) << "Vinni-puh devastated hive" << endl;
			cout << "Honey in hive: 0" << endl;
			mtx.unlock();
			content = 0;
		}
		else if (content > 14) {
			mtx.lock();
			cout << "bees in: " << beesIn << endl;
			time = Time(); 
			cout << time.substr(0, time.size() - 1) << "Vinni-puh was bitten by bees" << endl;
			cout << "Honey in hive: " << content << endl;
			mtx.unlock();
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

	string Time()
	{
		time_t seconds = time(NULL);
		tm* tInfo = localtime(&seconds);
		return asctime(tInfo);
	}
	/*
	метод отвечающий за вылет пчелы
	не выпускает пчелу, если она единственная в улии или количество меда достигло максимума
	если пчела вылетела, ее поток засыпает (время добычи меда)
	*/
	void goOut(int& content, int& beesIn, int id) {
		string time;
		if (content + (beesQuant - beesIn) < 30 && beesIn > 1) {
			mtx.lock();
			time = Time(); 
			cout << time.substr(0, time.size() - 1) << " bee № " << id << " flew out" << endl;
			mtx.unlock();
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
			mtx.lock();
			time = Time();
			cout << time.substr(0, time.size() - 1) << " bee № " << id << " came back" << endl;
			mtx.unlock();
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

string Time()
{
    time_t seconds = time(NULL);
    tm* tInfo = localtime(&seconds);
    return asctime(tInfo);
}

/*
ждет, пока Винни-пух проснется и улей заполнится хотя бы на половину и отправляет медведя опустошать улей
*/
void StartBear(Bear bear, int& content, int& beesIn) {
	string time;
	mtx.lock();
	time = Time();
	cout << time.substr(0, time.size() - 1)  << " Vinni-puh sleeps" << endl;
	mtx.unlock();
	while (true) {
		while (content < 15) {
			this_thread::sleep_for(1s);
		}
		bear.devastate(content, beesIn);
		mtx.lock();
		time = Time();
		cout << time.substr(0, time.size() - 1) << " Vinni-puh sleeps" << endl;
		mtx.unlock();
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
void StartBee(Bee bee, int& content, int& beesIn, int id) {
	while (true) {
		bee.goOut(content, beesIn, id);
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
		vec_bees[i] = thread(StartBee, bees[i], ref(content), ref(beesIn), i);
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
