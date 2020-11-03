#pragma once
#include <cassert>
#include <experimental/coroutine>
#include <iostream>
#include "resumable.h"
#include <list>

using namespace std;
using namespace std::experimental;

using coro_t = resumable::coro_handle;


class process_manager {

	struct awaiter;

	list<awaiter> list_;
	bool empty_;


	struct awaiter {
		process_manager& proccess_manager_;

		coro_t coro_ = nullptr;

		awaiter(process_manager& proccess_manager) noexcept:proccess_manager_(proccess_manager) {}

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
	process_manager(bool empty = false) : empty_(empty){}
	process_manager(const process_manager&) = delete;
	process_manager& operator = (const process_manager&) = delete;
	process_manager(process_manager&&) = delete;
	process_manager& operator = (process_manager&&) = delete;

	bool is_empty() const noexcept { return empty_; }

	void push_awaiter(awaiter awaiter_) {
		cout << "Process " << awaiter_.coro_.promise().proccess_number << " added to queue\n";
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


	void show_queue(process_manager &pr) {
		if (!empty_) {

			cout << "==================Queue==================\n";

			for (const auto& s : list_) {
				cout << "Process number " << s.coro_.promise().proccess_number << " is waiting to start\n";
			}

			cout << "=========================================\n";

			pr.execute();
		}
	}


	void show_queue_by_one(process_manager &pr) {

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