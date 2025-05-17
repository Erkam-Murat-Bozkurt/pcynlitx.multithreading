




 #ifndef THREAD_BLOCKER_HPP
 #define THREAD_BLOCKER_HPP

 #include "thread_data_holder.hpp"
 #include "thread_mutex.hpp"
 #include <thread>
 #include <mutex>
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <chrono>
 #include <condition_variable>
 #include <algorithm>


 namespace pcynlitx{

   class thread_blocker
   {
    
   public:

    thread_blocker();
    virtual ~thread_blocker();

    void Receive_Data_Holder(thread_data_holder * ptr);

    // THREAD CONTROL FUNCTIONS --------------------------------------

    void stop(int Number);
    void run(int Number);

    void stop(int Number, int Rescuer_Thread);
    void run(int Number, int Rescuer_Thread_Number);

    
    // ---------------------------------------------------------------

   private: 

    thread_mutex Locker;

    std::condition_variable cv;
    
    std::mutex mtx;

    thread_data_holder * data_holder_pointer;

   };
   
 };

 #endif /* THREAD_BLOCKER_HPP */