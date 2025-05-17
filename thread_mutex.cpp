
#include <thread_mutex.hpp>

pcynlitx::thread_mutex::thread_mutex(){


};


pcynlitx::thread_mutex::~thread_mutex(){

};

void pcynlitx::thread_mutex::lock(){

     this->lock_mutex.lock();
}


void pcynlitx::thread_mutex::unlock(){

     this->lock_mutex.unlock();
}

