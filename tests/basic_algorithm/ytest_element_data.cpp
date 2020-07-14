/*
 * @Author: Sky
 * @Date: 2020-07-03 10:45:04
 * @LastEditors: Sky
 * @LastEditTime: 2020-07-03 17:04:31
 * @Description: 
 */ 

class yTestElementData
{
private:
    /* data */
public:
    yTestElementData() noexcept{data_val = new int(0);}
    explicit yTestElementData(int val_) noexcept{data_val = new int(0);*data_val = val_;}
    ~yTestElementData() noexcept{delete data_val;}

    yTestElementData(const yTestElementData & data_){data_val = new int(0);*data_val = *data_.data_val;}
    


    //bool operator==(yTestElementData & data_) { return (data_.data_val==data_val);}
    // bool operator>(yTestElementData & data_) {  return (data_.data_val>data_val);}
    // bool operator<(yTestElementData & data_) {  return (data_.data_val<data_val);}

    void operator=(int val_){*data_val = val_;}

    operator int(){return *data_val;}


    int * data_val = nullptr;

};

bool operator==(const yTestElementData & data0_, const yTestElementData & data1_) { return (*data0_.data_val==*data1_.data_val);}
bool operator>(const yTestElementData & data0_, const yTestElementData & data1_) {  return (*data0_.data_val>*data1_.data_val);}
bool operator<(const yTestElementData & data0_, const yTestElementData & data1_) {  return (*data0_.data_val<*data1_.data_val);}

bool operator==(int data0_, const yTestElementData & data1_) { return (data0_==*data1_.data_val);}
bool operator>(int data0_, const yTestElementData & data1_) {  return (data0_>*data1_.data_val);}
bool operator<(int data0_, const yTestElementData & data1_) {  return (data0_<*data1_.data_val);}