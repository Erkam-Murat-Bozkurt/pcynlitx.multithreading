
 #ifndef THREAD_LOCKER_H
 #define THREAD_LOCKER_H

 #include <mutex>
 #include <cstdio>
 #include <thread>

 namespace pcynlitx {

   class thread_locker
   {
   public:
     thread_locker();
     virtual ~thread_locker();
     void lock();
     void unlock();
   private:
     std::mutex lock_mutex;
   };
 };

 #endif