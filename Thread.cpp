#include "Thread.hpp"

	Thread::Thread(void){}

	Thread::Thread(void * (*start_routine)(void *), void * arg)
	{
		_arg = arg;
		pthread_create(&_thread, NULL, start_routine, _arg);
	}

	Thread::~Thread(void) {}

int	Thread::join(void)
	{
		return (pthread_join(_thread, NULL));
	}

int	Thread::detach(void)
	{
		return (pthread_detach(_thread));
	}
