#pragma once
#include <cassert>
#include <experimental/coroutine>
#include <iostream>
#include "resumable.h"
#include <list>

using namespace std;
using namespace std::experimental;

using coro_t = resumable::coro_handle;


class process_manager_stack {

	struct awaiter;

	awaiter* top = nullptr;
	bool empty_;


	struct awaiter {
		process_manager_stack& proccess_manager_;
		awaiter* next = nullptr;
		coro_t coro_ = nullptr;

		awaiter(process_manager_stack& proccess_manager) noexcept:proccess_manager_(proccess_manager) {}

		bool await_ready() const noexcept {
			return proccess_manager_.is_empty();
		}

		void await_suspend(coro_t coro) noexcept {
			coro_ = coro;
			proccess_manager_.push_awaiter(this);
		}

		void await_resume() noexcept {}

	};

public:
	process_manager_stack(bool empty = false) : empty_(empty){}
	process_manager_stack(const process_manager_stack&) = delete;
	process_manager_stack& operator = (const process_manager_stack&) = delete;
	process_manager_stack(process_manager_stack&&) = delete;
	process_manager_stack& operator = (process_manager_stack&&) = delete;

	bool is_empty() const noexcept { return empty_; }

	void push_awaiter(awaiter *awaiter_) {
		awaiter* oldtop = top;
		top = awaiter_;
		awaiter_->next = oldtop;
	}

	awaiter operator co_await() noexcept { return awaiter(*this); }

	void execute() noexcept {

		awaiter* temp = top;
		top = nullptr;

		while (temp) {
			temp->coro_.resume();
			temp = temp->next;
		}
	}


	void show_queue(process_manager_stack &pr) {
		if (!empty_) {

			cout << "==================Stack==================\n";
			awaiter* temp = pr.top;

			while (temp) {
				cout << "Process number " << temp->coro_.promise().proccess_number << " is waiting in stack\n";

				temp = temp->next;
			}
			cout << "=========================================\n";

			pr.execute();
		}
	}

};