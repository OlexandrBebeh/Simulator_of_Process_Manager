#pragma once
#include <cassert>
#include <experimental/coroutine>
#include <iostream>
#include "resumable.h"
#include <list>

using namespace std;
using namespace std::experimental;

using coro_t = resumable::coro_handle;


class proccess_manager {

	struct awaiter;

	list<awaiter> list_;
	bool empty_;


	struct awaiter {
		proccess_manager& proccess_manager_;

		coro_t coro_ = nullptr;

		awaiter(proccess_manager& proccess_manager) noexcept:proccess_manager_(proccess_manager) {}

		bool await_ready() const noexcept {
			return proccess_manager_.is_empty();
		}

		void await_suspend(coro_t coro) noexcept {
			coro_ = coro;
			proccess_manager_.push_awaiter(*this);
		}

		void await_resume() noexcept {}

	};

public:
	proccess_manager(bool empty = false) : empty_(empty){}
	proccess_manager(const proccess_manager&) = delete;
	proccess_manager& operator = (const proccess_manager&) = delete;
	proccess_manager(proccess_manager&&) = delete;
	proccess_manager& operator = (proccess_manager&&) = delete;

	bool is_empty() const noexcept { return empty_; }

	void push_awaiter(awaiter awaiter_) {
		empty_ = false;
		list_.push_back(awaiter_);
	}

	awaiter operator co_await() noexcept { return awaiter(*this); }

	void execute() noexcept {

		list<awaiter> toresume;
		toresume.splice(toresume.begin(), list_);

		while (!toresume.empty()) {
			const auto& s = toresume.front();

			if (!s.coro_.done())
				s.coro_.resume();

			toresume.pop_front();
		}

		if (list_.empty()) {
			cout << "Proccesses end." << endl;
			empty_ = true;
		}

	}


	void show_queue(proccess_manager &pr) {
		if (!empty_) {

			cout << "==================Queue==================\n";

			for (const auto& s : list_) {
				cout << "Process number " << s.coro_.promise().proccess_number << " is waiting to start\n";
			}

			cout << "=========================================\n";

			pr.execute();
		}
	}


	void show_queue_by_one(proccess_manager &pr) {

		if (!empty_) {

			cout << "==================Queue==================\n";

			for (const auto& s : list_) {
				cout << "Process number " << s.coro_.promise().proccess_number << " is waiting to start\n";
			}

			cout << "=========================================\n";

			pr.execute();
		}
	}
};