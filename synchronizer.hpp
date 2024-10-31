
#ifndef SYNCHRONIZER_HPP
#define SYNCHRONIZER_HPP

#include <iostream>
#include <thread>
#include <vector>

namespace pcynlitx {

  template<class T>     
  class synchronizer
  {
     public:
     synchronizer(){};
     synchronizer(T * ptr){

        this->objPtr=ptr;
     };

     T * objPtr;

     template<typename A>
     A function(A (T::* fPtr) (void)){

        this->threadPool.push_back(std::thread(fPtr,objPtr));
     }

     template<typename B, typename... args>
     B function(B (T::* fPtr) (args... thParams), args... thParams){

        this->threadPool.push_back(std::thread(fPtr,objPtr,thParams...));
     };

     template<typename B, typename... args>
     B function(B (* func_Ptr) (args... thParams), args... thParams){

        this->threadPool.push_back(std::thread(func_Ptr,objPtr,thParams...));
     };

     std::vector<std::thread> threadPool;
   };
}

#endif  /* SYNCHRONIZER_HPP */