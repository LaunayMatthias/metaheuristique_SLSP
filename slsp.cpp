#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Team{
public:
  int _name;
  vector<int> _hasMet;
  Team(int name):_name(name){}

  int getName()const {return _name;}

  void meet(Team t){
    _hasMet.push_back(t.getName());
  }

  bool operator== (const Team &t1){
    return getName() == t1.getName();
  }

};

class SLSP{
public:
  int _T;
  int _P;
  int _W;
  vector<Team> _teams;

  vector<vector<pair<Team, Team>>> _schedule;

  SLSP(int t):_T(t), _P(t/2), _W(t-1){ //constructor

    //initialise les teams
    for(int i=0; i<_T; i++){
      _teams.push_back(Team(i));
    }

    //genere tous les matchs
    vector<pair<Team,Team>> allMatches;

    for(Team t: _teams){
      for(int i=0; i<_T; i++){
        pair<Team, Team> candidate = make_pair(Team(t.getName()), Team(i));
        if(t.getName() < i && i != t.getName()){
        allMatches.push_back(candidate);
      }
      }
    }

    //initialise le planning
    vector<pair<Team, Team>> v;

    for(int i=0; i<_P; i++){
      v.push_back(make_pair(Team(0), Team(1)));
    }

    for(int i=0; i<_W; i++){
      _schedule.push_back(v);
    }


    //assigne les au planning
    vector<pair<Team,Team>> matchesToAssign = allMatches;
    vector<pair<Team,Team>> matchesAssigned;

    for(int i=0; i<_schedule.size(); i++){
      for(int j=0; j<_schedule[i].size(); j++){
        _schedule[i][j] = matchesToAssign[_P*i + j];
      }
    }

    // for(auto p : allMatches){
    //   std::cout << p.first.getName() <<"vs" << p.second.getName()<< ' ';
    // }

    //affiche le planning
    showSchedule();
  }


  void showSchedule(){
    for (int i = 0; i < _schedule.size(); i++)
    {
      std::cout <<"{";
        for (int j = 0; j < _schedule[i].size(); j++)
        {
            cout << _schedule[i][j].first.getName() << "vs" << _schedule[i][j].second.getName() << " ";
        }
        std::cout <<"}\n";
    }
  }


};
