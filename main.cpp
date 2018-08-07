#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<map>
using namespace std;

template<class T>
class Queue:public vector<T>{
public:
    void enqueue(T ob){
        vector<T>::push_back(ob);
    }
    void dequeue(unsigned int num){
        for(int i=0;i<num;i++)
            vector<T>::erase(vector<T>::begin());
    }
    T dequeue(){
        T re;
        re=*(vector<T>::begin());
        vector<T>::erase(vector<T>::begin());
        return(re);
    }
};

void ReWrite(const char* File_name,Queue<string> date,Queue<string>day);
Queue<Queue<string> > FileProcess(const char *File_name);

class cDay{
public:
	string attribute;
	int date;
	string day;
	void get(int dt,string str, string attr){
		date =dt;
		day = str;
		attribute = attr;
	}
};

class cBase{
public:
	int W6, X6;
	int Wdc, Xdc;
	cBase(){
		W6 = X6 = Wdc = Xdc = 0;
	}
    void change(){
        int i=0;
    }
private:virtual void function()=0;
};


class cLabor:protected cBase{
private:
    virtual void function(){}
    string Name;
	int Wh, Xh;
	int Wf, Xf;
	int Wff, Xff;
	string attribute;
	string next_attr;
	Queue<cDay>last7;
	Queue<cDay>next7;
	Queue<cDay>days;
	map<int,string>rule;
    string last_year_month;
    void reset();
    void SetDayInfo(Queue<cDay> &what,Queue<int>date,Queue<string>day,Queue<string> attr);
    void SetTemplateDay(Queue<cDay>&for_what,Queue<cDay>template_day,int month);
    void Show(Queue<cDay>for_what);
    void SetHoliday(int order,Queue<cDay>&what_day);
public:
    cLabor(char attr,char next_attribute){
        reset();
        attribute = attr;
		next_attr = next_attribute;
    }
    cLabor(){reset();}
    void GetRule(Queue<string> rule);
    void ShowRule();
    void pre_process(int month);
    void show_schedule();
    string name(){return(Name);}
    void SetLastSchedule(Queue<int> date,Queue<string> day,Queue<string>attr);
    void ShowLastSchedule();
    void SetCalendar(Queue<cDay> template_day,int month);
    void ShowCalendar();
    void SetNextCalendar(Queue<cDay> template_day,int month);
    void ShowNextCalendar();
    void SetHoliday(int order);
};
void cLabor::ShowRule(){
    printf("Name is %s\n",Name.c_str());
    for(int i=0,j=1;i<rule.size();i++,j++){
        printf("%d : %s\n",j,rule[j].c_str());
    }
    printf("\n");
}
void cLabor::reset(){
    Wh = Xh = Wf = Xf = Wff = Xff = 0;
}
void cLabor::GetRule(Queue<string> qrule){
    Name=qrule.dequeue();
    last_year_month=qrule.dequeue();
    for(unsigned int i=0,j=1;i<qrule.size();i++,j++)
        rule[j]=qrule[i];
}
void cLabor::SetTemplateDay(Queue<cDay>&for_what,Queue<cDay>template_day,int month){
    for(unsigned int i=0;i<template_day.size();i++){
        template_day[i].attribute=rule[month];
        for_what.enqueue(template_day[i]);
    }
}
void cLabor::SetDayInfo(Queue<cDay>&what,Queue<int>date,Queue<string>day,Queue<string> attr){
    cDay tempday;
    for(unsigned int i=0;i<day.size();++i){
        tempday.attribute=attr[i];
        tempday.date=date[i];
        tempday.day=day[i];
        what.enqueue(tempday);
    }
}
void cLabor::SetLastSchedule(Queue<int> date,Queue<string>day,Queue<string>attr){
    SetDayInfo(last7,date,day,attr);
}
void cLabor::Show(Queue<cDay>for_what){
    for(unsigned int i=0;i<for_what.size();i++){
        printf("attribute : %s\n",for_what[i].attribute.c_str());
        printf("date : %d\n", for_what[i].date);
		printf("day : %s\n", for_what[i].day.c_str());
		printf("\n\n");
    }
}
void cLabor::ShowLastSchedule(){
    Show(last7);
}
void cLabor::SetCalendar(Queue<cDay> template_day,int month){
    SetTemplateDay(days,template_day,month);
}
void cLabor::ShowCalendar(){
    Show(days);
}
void cLabor::SetNextCalendar(Queue<cDay> template_day,int month){
    SetTemplateDay(next7,template_day,month);
}
void cLabor::ShowNextCalendar(){
    Show(next7);
}
void cLabor::SetHoliday(int order,Queue<cDay>&what_day){
    for(unsigned int i=0;i<what_day.size();i++){
        if(what_day[i].date == order){
            what_day[i].attribute='z';
            order+=3;
        }
    }
}
void cLabor::SetHoliday(int order){
    SetHoliday(order,days);
    SetHoliday(order,next7);
}
class cGroup{
public:
    
};

class cBoss{
private:
    int last_month;
    int month;
    Queue<cLabor>labors;
    void OpenRule();
    Queue<Queue<string> > OpenSchedule_pre();
    void OpenSchedule();
    Queue<cDay> OpenNext_pre();
    void OpenNext();
    void OpenCalendar();
    void OpenSpecialHoliday();
public:
    void pre_process();
    cBoss(int mon){
        month=mon;
        last_month=mon-1;
    }
};
void cBoss::OpenRule(){
    Queue<Queue <string> > data;
    data=FileProcess("rule.csv");
    data.dequeue();
    //printf("test size:%lu\n",data.size());
    cLabor temp;
    for(int i=0;i<data.size();i++){
        temp.GetRule(data[i]);
        labors.enqueue(temp);
    }
}
void cBoss::pre_process(){
    //printf("test\n");
    OpenRule();
    OpenSchedule();
    OpenCalendar();
    OpenNext();
    cout<<labors.size()<<endl;
    /*
    for(unsigned int i = 0;i<labors.size();i+=1){
        labors[i].ShowLastSchedule();
        printf("\\\\\\\\\\\\\\\\n");
        labors[i].ShowCalendar();
        printf("\\\\\\\\\\\\\\\\n");
        labors[i].ShowNextCalendar();
    }
    */
    labors[0].SetHoliday(1);
    labors[0].ShowCalendar();
    labors[0].ShowNextCalendar();
}
Queue<Queue<string> > cBoss::OpenSchedule_pre(){
    Queue<Queue<string> >data;
    Queue<string>date;
    Queue<string>day;
    data=FileProcess("schedule.csv");
    date=data.dequeue();
    day=data.dequeue();
    date.dequeue();
    day.dequeue();
    ReWrite("schedule_date_and_day.txt",date,day);
    return data;
}
Queue<cDay> cBoss::OpenNext_pre(){
    Queue<cDay> template_day;
    cDay tempday;
    Queue<Queue<string> > data;
    data=FileProcess("next.csv");
    ReWrite("NewNext",data[0],data[1]);
    ifstream f("NewNext");
    if(f.is_open()){
        while(!f.eof()){
            f>>tempday.date>>tempday.day;
            template_day.enqueue(tempday);
        }
    }
    template_day.pop_back();
    /*
    for(unsigned int i=0;i<template_day.size();++i)
        cout<<template_day[i].date<<template_day[i].day<<endl;
        */
    return template_day;
}
void cBoss::OpenSchedule(){
    Queue<Queue<string> >data;
    data=OpenSchedule_pre();
    Queue<int> date;
    Queue<string> day;
    int tempdate=0;
    string tempday;
    ifstream file("schedule_date_and_day.txt");
    int i=0;
    if(file.is_open()){
        while(!file.eof()){
            file>>tempdate>>tempday;
            date.enqueue(tempdate);
            day.enqueue(tempday);
        }
    }
    date.pop_back();
    day.pop_back();
    for(unsigned int i=0;i<data.size();++i){
        for(unsigned int j=0;i<labors.size();++j){
            if(data[i][0]==labors[j].name()){
                labors[j].SetLastSchedule(date,day,data[i]);
                break;
            }
        }
    }
}
void cBoss::OpenCalendar(){
    Queue<Queue<string> > data;
    data=FileProcess("calendar.csv");
    ReWrite("NewCalendar.txt",data[0],data[1]);
    ifstream f("NewCalendar.txt");
    cDay temp;
    Queue<cDay>template_days;
    if(f.is_open()){
        while(!f.eof()){
            f>>temp.date>>temp.day;
            template_days.enqueue(temp);
        }
    }
    template_days.pop_back();
    for(unsigned int i=0;i<labors.size();i++){
        labors[i].SetCalendar(template_days,month);
    }
}
void cBoss::OpenNext(){
    Queue<cDay> template_day;
    template_day=OpenNext_pre();
    for(unsigned int i=0;i<labors.size();i++){
        labors[i].SetNextCalendar(template_day,month+1);
    }
}
void cBoss::OpenSpecialHoliday(){
    Queue<Queue<string> >data;
}


int main(){
    cBoss boss(5);
    boss.pre_process();
}
void ReWrite(const char* File_name,Queue<string> date,Queue<string>day){
    ofstream file(File_name);
    if(file.is_open()){
        for(unsigned int i=0;i<date.size();++i){
            file<<date[i]<<" "<<day[i]<<'\n';
        }
    }else{
        printf("%s cannot be writen\n",File_name);
    }
}
Queue<Queue<string> > FileProcess(const char *File_name){
    ifstream file(File_name);
    string linestr;
    string spread;
    Queue<string> linedata;
    Queue<Queue<string> > data;
    char str[100];
    char *pch;
    int i=0;
    if(file.is_open()){
        while(!file.eof()){
            getline(file,linestr,'\n');
            strcpy(str,linestr.c_str());
            pch=strtok(str,",");
            while(pch!=NULL){
                spread.assign(pch);
                linedata.enqueue(spread);
                pch=strtok(NULL,",");
            }
            data.enqueue(linedata);
            linedata.clear();
        }
    }
    return data;
}