/*
 * @Author: Sky
 * @Date: 2020-07-03 10:45:04
 * @LastEditors: Sky
 * @LastEditTime: 2020-08-19 15:47:23
 * @Description: 
 */ 

#include <iostream>

class yTestElementData
{
private:
    /* data */
public:
    yTestElementData() noexcept :data_val(nullptr) {
        
        if (nullptr == data_val) 
            data_val = new int(0);
    }
    explicit yTestElementData(int val_) noexcept :data_val(nullptr){
        if (nullptr == data_val) 
            data_val = new int(0);
        *data_val = val_;
    }
    ~yTestElementData() noexcept{
        if (nullptr != data_val) 
            delete data_val;data_val=nullptr;
    }
    
    yTestElementData & operator=(const yTestElementData & data_) {
        
        if (nullptr == data_val) 
            data_val = new int(0);
            
        *data_val = *data_.data_val; 
        return *this;
    }
    yTestElementData(const yTestElementData & data_):data_val(nullptr){
        
        if (nullptr == data_val) 
            data_val = new int(0);

        *data_val = *data_.data_val;
    }


    yTestElementData & operator=(yTestElementData && data_){
        
        *data_val = *data_.data_val;
        return *this;
    }
    yTestElementData(yTestElementData && data_){
        
        *this = std::move(data_);
    }




    operator int(){return *data_val;}
    void operator=(int val){

        if (nullptr == data_val) 
            data_val = new int(0);

        *data_val = val;  
    }

    int * data_val = nullptr;

};

bool operator==(const yTestElementData & data0_, const yTestElementData & data1_) { return (*data0_.data_val==*data1_.data_val);}
bool operator>(const yTestElementData & data0_, const yTestElementData & data1_) {  return (*data0_.data_val>*data1_.data_val);}
bool operator<(const yTestElementData & data0_, const yTestElementData & data1_) {  return (*data0_.data_val<*data1_.data_val);}

bool operator==(int data0_, const yTestElementData & data1_) { return (data0_==*data1_.data_val);}
bool operator>(int data0_, const yTestElementData & data1_) {  return (data0_>*data1_.data_val);}
bool operator<(int data0_, const yTestElementData & data1_) {  return (data0_<*data1_.data_val);}