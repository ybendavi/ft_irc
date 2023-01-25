#ifndef THREAD_HPP
# define THREAD_HPP

#include <pthread.h>

class	Thread
{
	private:		pthread_t	_thread;
				void *		_arg;
	public:
					Thread(void);
					Thread(void * (*start_routine)(void *), void * arg);
					~Thread(void);
				int	join(void);
				int	detach(void);
};

#endif

