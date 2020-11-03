#include <iostream>
#include <thread>
#include "resumable.h"
#include "manager.h"
#include "manager_stack.h"
#include <time.h>  

process_manager pr1;
process_manager pr2;
process_manager_stack prstack;


resumable start_proccess_stack(int time) {
	co_await prstack;

	cout << "Running for " << time << " milliseconds\n";
	this_thread::sleep_for(chrono::milliseconds(time));
}


resumable start_proccess(int time) {
	co_await pr1;

	cout << "Running for " << time << " milliseconds\n";
	this_thread::sleep_for(chrono::milliseconds(time));
}

resumable start_proccess_breakeble(int time) {

	co_await pr2;

	for (int i = 1; i < time; i++) {

		this_thread::sleep_for(chrono::milliseconds(1));
		if (i % (rand() % 30 + 1) == 0) {
			cout << "Run for " << i << " milliseconds and stop!"  << " Left time:" << time-i << endl;
			co_await pr2;
		}

	}

	cout << "Total run for " << time << endl;
}


void test0() {
	srand(time(NULL));

	for (int i = 0; i < 10; i++) {
		start_proccess_stack(rand() % 100 + 1);
	}

	prstack.show_queue(prstack);
}

void test() {
	srand(time(NULL));

	for (int i = 0; i < 10; i++) {
		start_proccess(rand() % 100 + 1);
	}

	pr1.show_queue(pr1);

}

void test1() {
	srand(time(NULL));

	for (int i = 0; i < 10; i++) {
		start_proccess_breakeble(rand() % 100 + 1);
	}

	while (true) {
		pr2.show_queue_by_one(pr2);
	}

}

int main() {

	test0();
	test();
	test1();

};
