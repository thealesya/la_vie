#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <cmath>
#include "AJob.h"
#include <chrono>
/*
template <class mType>
class Sum : public AJob {
	private:
        mType a, b;
        mType result;
	public:
        Sum(mType a, mType b);
        void  ExecJob() override{
            this->result=this->a + this->b;
            this_thread::sleep_for(chrono::seconds(10));
        };
        mType GetResult() override {
            return this->result;
        };
};*/

class Sum : public AJob {
	private:
        int a = 0, b = 0;
        int result = 0;
	public:
        Sum(int a, int b){
            this->a = a;
            this->b = b;
        };
        void  ExecJob() override{
            this->result=this->a + this->b;
            this_thread::sleep_for(chrono::seconds(1));
        };
        int GetResult() override {
            /*std::cout << "!!!!!!!!!!!!!!!!!!!!!!! " << this->result << std::endl;*/
            return this->result;
        };
};
/*
template <class mType>
class Diff : public AJob {
	private:
        mType a, b;
        mType result;
	public:
        Diff(mType a, mType b);
        void  ExecJob() override{
            this->result=this->a - this->b;
            this_thread::sleep_for(chrono::seconds(10));
        };
        mType GetResult() override {
            return this->result;
        };
};

template <class mType>
class Mult : public AJob {
	private:
        mType a, b;
        mType result;
	public:
        Mult(mType a, mType b);
        void  ExecJob() override{
            this->result=this->a * this->b;
            this_thread::sleep_for(chrono::seconds(10));
        };
        mType GetResult() override {
            return this->result;
        };
};

template <class mType>
class Div : public AJob {
	private:
        mType a, b;
        mType result;
	public:
        Div(mType a, mType b);
        void  ExecJob() override{
            this->result=this->a / this->b;
            this_thread::sleep_for(chrono::seconds(10));
        };
        mType GetResult() override {
            return this->result;
        };
};

template <class mType>
class mPow : public AJob {
	private:
        mType a, b;
        mType result;
	public:
        mPow(mType a, mType b);
        void  ExecJob() override{
            this->result=pow(this->a, this->b);
            this_thread::sleep_for(chrono::seconds(10));
        };
        mType GetResult() override {
            return this->result;
        };
};*/

