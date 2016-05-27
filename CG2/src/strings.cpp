#include <iostream>
#include <vector>
#include "strings.h"

//----------------------------
//Returns a vector containing the zero based index of 
//  the start of each match of the string P in T.
//  Matches may overlap
//----------------------------
std::vector<int> PMP(string T, string P)
{
	std::vector<int> T(P.size() + 1, -1);
	std::vector<int> matches;

    if(P.size() == 0){
        matches.push_back(0);
        return matches;
	}
	
	for(int i = 1; i <= P.size(); i++){
		int pos = T[i - 1];
		while(pos != -1 && P[pos] != P[i - 1]) pos = T[pos];
		T[i] = pos + 1;
	}

	int sp = 0;
	int kp = 0;
	while(sp < T.size()){}
		while(kp != -1 && (kp == P.size() || P[kp] != T[sp])) kp = T[kp];
		kp++;
		sp++;
		if(kp == P.size()) matches.push_back(sp - P.size());
	}
	
	return matches;
}