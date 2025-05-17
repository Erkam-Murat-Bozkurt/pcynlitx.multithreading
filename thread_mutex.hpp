
 #ifndef THREAD_MUTEX_HPP
 #define THREAD_MUTEX_HPP

 #include <mutex>
 #include <cstdio>
 #include <thread>

 namespace pcynlitx {

   class thread_mutex
   {
   public:
     thread_mutex();
     virtual ~thread_mutex();
     void lock();
     void unlock();
   private:
     std::mutex lock_mutex;
   };
 };

 #endif /* THREAD_MUTEX_HPP */