#pragma once
#include <cassert>
#include <experimental/coroutine>
#include <iostream>

using namespace std;
using namespace std::experimental;

int queue = 1;

struct resumable
{
	struct promise_type {

		int proccess_number;

		using coro_handle = coroutine_handle<promise_type>;


		auto get_return_object() {
			return coro_handle::from_promise(*this);
		};
		auto initial_suspend() {
			return suspend_never();
		}
		auto final_suspend() { return suspend_never(); }
		void return_void() {}
		void unhandled_exeption() { terminate(); }
		promise_type() {
			proccess_number = queue;
			queue++;
		}
		~promise_type() {
			cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ " << "\n";
			cout << "Ended proccess number: " << proccess_number << "\n";
			cout << "///////////////////////////////////" << "\n";
		}
	};

	using coro_handle = coroutine_handle<promise_type>;

	resumable(coro_handle handle) {}
	resumable(resumable&) {}
	resumable(resumable&& rhs) {}

};

