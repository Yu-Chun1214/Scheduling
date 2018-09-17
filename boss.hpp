#ifndef __boss_hpp__
#define __boss_hpp__
#include"menboss.hpp"
#include"fileprocess.hpp"

Queue<int> cBoss::strings_to_int(Queue<string> date)
{
    Queue<int>date_int;
    for(int i = 0;i<date.size();i++){
        date_int.push_back(stoi(date[i]));
    }
    return date_int;
}

void cBoss::readCalendar()
{
    sData data;
    sData package;
    data = reading("calendar.csv");
    string name;
    for(unsigned int i = 0;i<data.data.size();i++){
        name = data.data[i][0];
        package = cBoss::package(data.data[i],data);
        labors[name].getCalendar(package);
    }
}

sData cBoss::package(Queue<string>attr,sData data)
{
    sData pac;
    pac.data.enqueue(attr);
    pac.day_amount = data.day_amount;
    pac.date = data.date;
    pac.day = data.day;
    return pac;
}

void cBoss::readSchedule()
{
    sData data;
    sData package;
    data = reading("schedule.csv");
    string name;
    for(unsigned int i = 0;i<data.data.size(); i++){
        name = data.data[i][0];

        package = cBoss::package(data.data[i],data);

        labors[name].getSchedule(package);

        // package.data.clear();

    }
}

void cBoss::readRule()
{
    Queue<Queue<string> > data;
    data = FileProcess("rule.csv");
    data.dequeue();
    string name;
    for (int i = 0; i < data.size(); i++)
    {
        name = data[i].dequeue();
        labors[name].getRule(data[i], name);
    }
}

sData cBoss::reading(const char *FileName)
{
    Queue<Queue<string> > data;
    Queue<string> string_date;
    Queue<string> days;
    Queue<int> date;
    sData day_info;

    data = FileProcess(FileName);

    string_date = data.dequeue();
    days = data.dequeue();

    days.dequeue(2);
    string_date.dequeue(2);
    date = strings_to_int(string_date);

    day_info.data = data;
    day_info.date = date;
    day_info.day = days;
    day_info.day_amount = date.size();

    return day_info;
}

#endif