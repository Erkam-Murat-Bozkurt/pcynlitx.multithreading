
#ifndef SYNCHRONIZER_HPP
#define SYNCHRONIZER_HPP

#include <iostream>
#include <thread>
#include <vector>

namespace pcynlitx {

  template<typename T>     
  class synchronizer
  {
     public:
      synchronizer(){};
      synchronizer(T * ptr){

        this->objPtr=ptr;
      };

      T * objPtr;

      template<typename A>
      A function(A (T::* fPtr) (void));

      template<typename B, typename... args>
      B function(B (T::* fPtr) (args... thParams), args... thParams);

      template<typename B, typename... args>
      B function(B (* func_Ptr) (args... thParams), args... thParams);

      void join(int thrNum);

     protected:
      std::vector<std::thread> threadPool;
   };


   template<typename T> template<typename A>
   A synchronizer<T>::function(A (T::* fPtr) (void)){

     this->threadPool.push_back(std::thread(fPtr,objPtr));
   }
   
   template<typename T> template<typename B, typename... args>
   B synchronizer<T>::function(B (T::* fPtr) (args... thParams), args... thParams){

      this->threadPool.push_back(std::thread(fPtr,objPtr,thParams...));
   };

   template<typename T> template<typename B, typename... args>
   B synchronizer<T>::function(B (* func_Ptr) (args... thParams), args... thParams){

      this->threadPool.push_back(std::thread(func_Ptr,objPtr,thParams...));
   };

   template<typename T>
   void synchronizer<T>::join(int thrNum){

         this->threadPool.at(thrNum).join();
   }

};




#endif  /* SYNCHRONIZER_HPP */