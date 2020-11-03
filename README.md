# Simulator_of_Process_Manager
This is simulation of process manager on c++ with using corutines.

The efficiency of the computer system does not depend only on the own efficiency of information processing algorithms and technical characteristics of the computer system, but also on the rules adopted in the system of work, reception and processing of user records.

The efficiency of maintenance methods determines the possible value of delays or loss of applications for packaging, as well as when the applications are in the system. Depending on the type of management and scheduling system, the request delay can be added to the total average delay time or the allowable waiting time.


## FIRST  IN- FIRST OUT (FIFO)

The Firstin queue algorithm, Firstout (FIFO), also called First Come First Served, is the simplest.

FIFO is the simplest process planning strategy and is that the resource is transferred to the process that first turned to it. When a process enters the queue of finished processes, the process control block joins the tail of the queue. The average waiting time for a FIFO strategy is often quite large and depends on the order in which the processes enter the queue of finished processes.

### Functions

This process manager written with help of corutines. So for better understanding you need to read about them more.

** THIS CODE NOT RUN WITH VISUAL STUDIO BUCAUSE OF USING std::experimental. TO START THIS YOU NEED USE Visual Studio 2019 Developer Command Prompt v16.7.6. **

`void show_queue_by_one(process_manager &pr)` - shows queue of processes and execute them;

`void execute()` - execute processes that are in queue.
	

## Usege

To use the manager you need to create fuction instance of `process_manager` , than create of `resumable` class and call `co_await process_manager_instance` to added this process to queue in process manager


## Examples

> Create `process_manager`

Code

```
	process_manager pr1;
```


> Create function which we will execute.

Code

```
	resumable start_proccess(int time) {
		co_await pr1;

		cout << "Running for " << time << " milliseconds\n";
		
		this_thread::sleep_for(chrono::milliseconds(time));
	}
```

> Let we start some processes and after that execute them.

Code

```
		srand(time(NULL));

		for (int i = 0; i < 10; i++) {
			start_proccess(rand() % 100 + 1);
		}
		
		pr1.show_queue(pr1);
```

Output

![example 2](/images/example2.png)

> Next lets try it in more diffical situation. When processes stops itselves and go back to queue.

Code

```
	process_manager pr2;
	
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
```

> Now we create some processes and execute them till they ends.

Code

```
	void test1() {
		srand(time(NULL));

		for (int i = 0; i < 10; i++) {
			start_proccess_breakeble(rand() % 100 + 1);
		}

		while (true) {
			pr2.show_queue_by_one(pr2);
		}

	}
```
Output

![example 3](/images/example3.png)
![example 4](/images/example4.png)
![example 5](/images/example5.png)
![example 6](/images/example6.png)

> This the same manager but works with stack.

Code

```
		process_manager_stack prstack;


	resumable start_proccess_stack(int time) {
		co_await prstack;

		cout << "Running for " << time << " milliseconds\n";
		this_thread::sleep_for(chrono::milliseconds(time));
	}
	
	void test0() {
		srand(time(NULL));

		for (int i = 0; i < 10; i++) {
			start_proccess_stack(rand() % 100 + 1);
		}

		prstack.show_queue(prstack);
	}
```

Output

![example 1](/images/example1.png)