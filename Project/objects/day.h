#ifndef __DAY_H__
#define __DAY_H__

#include <string>
#include <iostream>
#include <vector>

class Day{ 
private:
	int _month;
	std::string _day;
	std::string _attr;
	int _date;
public:
	Day(int mon, std::string day, int date, std::string attribute);
	Day(int mon, std::string day, std::string date, std::string attribute);
	std::string day();
	std::string attr();
	int month();
	int date();
	friend std::ostream & operator <<(std::ostream & out , Day & d);
	friend std::ostream & operator <<(std::ostream & out, std::vector<Day> days);
	friend std::ostream & operator <<(std::ostream & out, std::vector<Day *> days_pointer);

};

std::ostream & operator <<(std::ostream & out, Day & d);
std::ostream & operator <<(std::ostream & out, std::vector<Day> days);
std::ostream & operator <<(std::ostream & out, std::vector<Day *> days_pointer);

#endif 