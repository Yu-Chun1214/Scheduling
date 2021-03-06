#include "group.h"
#include <vector>
#include <algorithm>
#include <pthread.h>

Group::Group(std::string type, int damount):dayAmount(damount){
	generator.seed(rand());
	Group::type = type;
	Wno = 20000;
	W1 = 10000;
}


void Group::addMember( Labor * labor){
#ifdef DEBUG
	if (members.count(labor->Name()))
		std::cout<<colored(labor->Name() + " is duplicate",fontstyle::RED);
#endif
	members[labor->Name()] = labor;
	vectorOfmembers.push_back(labor);
}

void Group::setUpHoliday(){
	int i = 0;
	// cout<<"Group is "<<type<<endl;
	for(map<string, Labor *>::iterator it = members.begin(), end = members.end(); it != end; it++, i++)
		it->second->setHoliday(i);
	selector = new uniform_int_distribution<int>(0,members.size() - 1);
}

int Group::PeopleWorkDays(unsigned int people_amount){
//	cout<<"members.size = "<<members.size()<<endl;
	int days = 0;
	unsigned int howManyPeopleWork;
	for(int i = 0; i < dayAmount; ++i){
		howManyPeopleWork = 0;
		for(map<string, Labor*>::iterator it = members.begin(), end = members.end(); it != end; it++){
			if(it->second->isWoringThisDay(i)){
				++howManyPeopleWork;	
			}
		}
		if(howManyPeopleWork == people_amount)
			++days;
//		cout<<"==================="<<endl;
	}
	return days;
}

double Group::ComputationGroupQuality(){
	double quility = 0.0;
	double PQmax_PQmin = 0.0;
	int PQ = 0;
	int PQmax, PQmin;
	PQmax = 1;
	PQmin = 1000000;
	int noPeopleWork = PeopleWorkDays();
	int onePeopleWork = PeopleWorkDays(1);
	quility += Wno * noPeopleWork;
	quility += W1 * onePeopleWork;
	for(std::map<std::string, Labor *>::iterator it = members.begin(), end = members.end(); it != end; it++){
		PQ = it->second->ComputatePersonalQuality();
		PQmax_PQmin += PQ;
		if(PQ > PQmax){
			PQmax = PQ;
		}
		if(PQ < PQmin){
			PQmin = PQ;
		}
	}
	// cout<<"PQ max = "<<PQmax<<" PQmin = "<<PQmin<<endl;
	PQmax_PQmin /= 3.0;
	quility += PQmax;
	// printf("Hno = %d, H1 = %d,(PQmax - PQmin) / 3 = %f,  Quality = %f\n",noPeopleWork, onePeopleWork,PQmax_PQmin, quility);	
	return quility;
}

void Group::randomlySelectLaborSwapTheDay(){
	int rnd = selector->operator()(generator);
	Labor * labor = vectorOfmembers[rnd];
	labor->randomlySwapDayType();
	lastSelectedLabor = labor;
}

void Group::laborScheduleRestore(){
	if(lastSelectedLabor)
		lastSelectedLabor->restoreLastSchedule();
}

std::vector<std::vector<std::string> >  Group::showUpGroupSchedule(){
	map<string, Labor *>::iterator it = members.begin();
	vector<vector<string> > rows;
	vector<string> row;
	cout<<"Date  ";
	row.push_back("Date");	
	vector<Day *> * d = new vector<Day *>();
	d = it->second->PersonalSchedule();
	for(unsigned int i = 0, size = d->size(); i < size; ++i){
		row.push_back(to_string(d->at(i)->date()));
		if (to_string(d->at(i)->date()).length() == 2){
			cout<<d->at(i)->date()<<"  ";
		}else{
			cout<<d->at(i)->date()<<"   ";
		}
	}
	cout<<endl;
	
	rows.push_back(row);
	row.clear();

	cout<<"Day   ";
	row.push_back("Day");

	for(unsigned int i = 0, size = d->size(); i < size; ++i){
		row.push_back(d->at(i)->day());
		cout<<d->at(i)->day()<<" ";
	}
	rows.push_back(row);
	row.clear();

	cout<<endl;
	for(map<string, Labor *>::iterator it = members.begin(), end = members.end(); it != end; it++){
		vector<Day *> * d = new vector<Day *>();
		d = it->second->PersonalSchedule();
		row.push_back(it->first);

		cout<<it->first<<"  ";
		int i = 0, size = 0;
		for(; i < 7; ++i){
			cout<<d->at(i)->attr()<<"  ";	
				
		}
		// cout<<"| ";
		for(size = d->size() - 7; i < size; ++i){
			// if(d->at(i)->attr() == "Z"){
			// 	d->at(i)->setColored();
			// }
			cout<<d->at(i)->attr()<<"  ";
		}
		//cout<<"| ";
		for(size = d->size(); i < size; ++i)
			cout<<d->at(i)->attr()<<"  ";
		i = 0;
		for(size = d->size(); i < size; ++i){
			row.push_back(d->at(i)->attr());
		}
		rows.push_back(row);
		row.clear();

		cout<<endl;
	}
	return rows;
}

void Group::backup(){
	for(map<string, Labor *>::iterator it = members.begin(), end = members.end(); it != end; it++){
		it->second->backupSchedule();
	}
}

void Group::restore(){
	for(map<string, Labor *>::iterator it = members.begin(), end = members.end(); it != end; it++){
		it->second->restoreSchedule();
	}
}
