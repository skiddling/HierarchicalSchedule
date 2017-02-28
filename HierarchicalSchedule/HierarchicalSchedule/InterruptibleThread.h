#pragma once
//#include "ga.h"
#include "schedule.h"


//using namespace std;



class InterruptFlag
{
public:
	inline void set() {
		lock_guard<mutex> guard(_mtx);
		_set = true;
	}

	inline bool is_set() {
		lock_guard<mutex> guard(_mtx);
		return _set;
	}

private:
	mutex _mtx;
	bool _set;
};

extern thread_local InterruptFlag this_thread_interrupt_flag;

class InterruptibleThread
{
public:
	//default_random_engine e_;
	//uniform_int_distribution<int> u_;
	thread _internal_thread;
	promise<Schedule>* pro_ptr_;
	template<typename FunctionType>
	InterruptibleThread(FunctionType f, int i){
		promise<InterruptFlag*> p;
		_internal_thread = thread([f, &p, this]()
		{
			p.set_value(&this_thread_interrupt_flag);
			f(i);
		});
		_interrupt_flag = p.get_future().get();
	}

	inline void interrupt()
	{
		if (_interrupt_flag != nullptr)
		{
			_interrupt_flag->set();
		}
	}

	void join() {
		_internal_thread.join();
	}

	thread::id get_id() {
		return this_thread::get_id();
	}

	InterruptFlag* getInterruptFlag() {
		return &this_thread_interrupt_flag;
	}

private:
	InterruptFlag* _interrupt_flag;
};
