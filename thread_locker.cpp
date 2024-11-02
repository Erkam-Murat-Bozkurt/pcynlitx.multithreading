
#include <thread_locker.hpp>

pcynlitx::thread_locker::thread_locker(){


};


pcynlitx::thread_locker::~thread_locker(){

};

void pcynlitx::thread_locker::lock(){

     this->lock_mutex.lock();
}


void pcynlitx::thread_locker::unlock(){

     this->lock_mutex.unlock();
}

