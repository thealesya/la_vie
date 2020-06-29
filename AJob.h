#pragma once
using namespace std;

class AJob {
public:
    virtual ~AJob() = default;
	virtual void ExecJob()=0;
	virtual int GetResult()=0;
};



