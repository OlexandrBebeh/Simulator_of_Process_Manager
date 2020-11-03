#include <iostream>
#include <thread>
#include "resumable.h"
#include "manager.h"
#include <time.h>  

proccess_manager pr;

resumable start_proccess(int time) {
	co_await pr;

	cout << "Running for " << time << "seconds\n";
	this_thread::sleep_for(chrono::milliseconds(time));
}

resumable start_proccess_breakeble(int time) {

	co_await pr;

	for (int i = 1; i < time; i++) {

		this_thread::sleep_for(chrono::milliseconds(1));
		if (i % (rand() % 30 + 1) == 0) {
			cout << "Run for " << i << " milliseconds and stop!"  << " Left time:" << time-i << endl;
			co_await pr;
		}

	}

	cout << "Total run for " << time << endl;
}

void test() {
	srand(time(NULL));

	for (int i = 0; i < 10; i++) {
		start_proccess(rand() % 100 + 1);
	}

	pr.show_queue(pr);

}

void test1() {
	srand(time(NULL));

	for (int i = 0; i < 10; i++) {
		start_proccess_breakeble(rand() % 100 + 1);
	}

	while (true) {
		pr.show_queue_by_one(pr);
	}

}

int main() {

	//test();
	test1();

};
