#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct Team{
public:
  int _name;
  Team(int name):_name(name){}

  Team(const Team& t) = default;


  int getName()const {return _name;}

  bool operator== (const Team &t1){
    return getName() == t1.getName();
  }

  bool operator <(Team const& rhs) const{
        return rhs.getName() < getName();
    }

};

bool contains(vector<pair<Team, Team>> v, pair<Team, Team> t){
  for(auto t1 : v){
    if(t1.first.getName() == t.first.getName()
    || t1.first.getName() == t.second.getName()
    || t1.second.getName() == t.first.getName()
    || t1.second.getName() == t.second.getName()){
      return true;
    }
  }
  return false;
}

bool contains(pair<Team, Team> m1, pair<Team, Team> m2){
    if(m1.first.getName() == m2.first.getName()
    || m1.first.getName() == m2.second.getName()
    || m1.second.getName() == m2.first.getName()
    || m1.second.getName() == m2.second.getName()){
      return true;
    }
  return false;
}

class SLSP{
public:
  int _T;
  int _P;
  int _W;
  vector<Team> _teams;

  vector<pair<Team,Team>> _allMatches;

  vector<vector<pair<Team, Team>>> _schedule;

  SLSP(const SLSP& p) = default;

  SLSP(int t):_T(t), _P(t/2), _W(t-1){ //constructor

    //initialise les teams
    for(int i=0; i<_T; i++){
      _teams.push_back(Team(i));
    }

    //genere tous les matchs
    for(Team t: _teams){
      for(int i=0; i<_T; i++){
        pair<Team, Team> candidate = make_pair(Team(t.getName()), Team(i));
        if(t.getName() < i && i != t.getName()){
        _allMatches.push_back(candidate);
      }
      }
    }

    //initialise le planning
    vector<pair<Team, Team>> v;

    for(int i=0; i<_P; i++){
      v.push_back(make_pair(Team(-1), Team(-1)));
    }

    for(int i=0; i<_W; i++){
      _schedule.push_back(v);
    }

    //assigne les matchs au planning (contrainte ALLDIFF + WEEK)
    vector<pair<Team,Team>> matchesToAssign = _allMatches;

      int k=0;
      for(int i=0; i<_schedule.size(); i++){
        for(int j=0; j<_schedule[i].size(); j++){
          while(contains(_schedule[i], matchesToAssign[k]) && k<matchesToAssign.size()){
            k++;
          }
            _schedule[i][j] = matchesToAssign[k];
            matchesToAssign.erase(matchesToAssign.begin()+k);
            k=0;
        }
      }


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

  int evaluation(){
    int excess = 0;
    for(int i=0; i<_P; i++){
      vector<pair<Team, Team>> period;
      for(int j=0; j<_W; j++){
        period.push_back(_schedule[j][i]);
      }

      for(int k=0; k<period.size(); k++){
        int occurences_t1 = 1;
        int occurences_t2 = 1;
        int t1 = period[k].first.getName();
        int t2 = period[k].second.getName();
        for(int l=k+1; l<period.size(); l++){
          if(t1 == period[l].first.getName()
          ||t1 == period[l].second.getName()){
            occurences_t1++;
          }
          if(t2 == period[l].first.getName()
          ||t2 == period[l].second.getName()){
            occurences_t2++;
          }

          if(occurences_t1 > 2)
            excess+= occurences_t1-2;

          if(occurences_t2 > 2)
            excess+= occurences_t2-2;
        }
      }
    }
    return excess;
  }

  SLSP swap(int w, int p1, int p2){
    SLSP neighbor(*this);
    auto tmp = neighbor._schedule[w][p1];
    neighbor._schedule[w][p1] = neighbor._schedule[w][p2];
    neighbor._schedule[w][p2] = tmp;
    return neighbor;
  }
};


class TabuSearch{
public:
  int _maxInterations;
  SLSP _p;
  map<pair<Team, Team> ,int> _tabuList;

  TabuSearch(SLSP p, int maxInterations):_p(p), _maxInterations(maxInterations){
    int it = 0;
    for(auto m : _p._allMatches){
      _tabuList.insert(make_pair(m, 0));
    }
    int consecutive_non_improvement = 0;
    int last_evaluation = 10000;
      while(maxInterations != it && _p.evaluation() != 0){
        it++;
        int weekchosen = 0;
        int bestSwap = 10000;
        pair<Team, Team> match1(make_pair(Team(-1), Team(-1)));
        int p1;
        pair<Team, Team> match2(make_pair(Team(-1), Team(-1)));
        int p2;
        SLSP neighbor(_p);
        for(int week=0; week<_p._schedule.size(); week++){
          for(int i=0; i<_p._schedule[week].size(); i++){
            for(int j=i+1; j<_p._schedule[week].size(); j++){
              if((_tabuList[match1] + 50 > it) && (_tabuList[match2] + 50 > it)){
                //if not tabu
                neighbor = _p.swap(week, i,j);
                int swapevaluation = neighbor.evaluation();
                if(swapevaluation < bestSwap){
                  bestSwap = swapevaluation;
                  match1 = _p._schedule[week][i];
                  p1 = i;
                  match2 = _p._schedule[week][j];
                  p2 = j;
                  weekchosen = week;
                  _tabuList[match1] = it;
                  _tabuList[match2] = it;
                }
              }

            }
          }
        }
        _p = _p.swap(weekchosen, p1, p2);
        int evaluation = _p.evaluation();
        if(evaluation == last_evaluation){
          consecutive_non_improvement++;
        }
        last_evaluation = evaluation;

        if(consecutive_non_improvement == 10){
          for(auto p : _tabuList){
            p.second = 0;
          }
          consecutive_non_improvement = 0;
        }
      }
      int evaluation = _p.evaluation();
      std::cout << "evaluation: " << evaluation << '\n';
      std::cout << "iterations: " << it << '\n';
    _p.showSchedule();
  }
};
