#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <locale>
#include "strings.h"

std::vector<int> KMP(std::string S, std::string K){
	std::vector<int> T(K.size() + 1, -1);
	std::vector<int> matches;

	for(size_t i = 0; i < S.length();i++)
		S[i] = toupper(S[i]);

	for(size_t i = 0; i < K.length();i++)
		K[i] = toupper(K[i]);

	if(K.size() == 0)
	{
		matches.push_back(0);
		return matches;
	}
	for(int i = 1; i <= K.size(); i++)
	{
		int pos = T[i - 1];
		while(pos != -1 && K[pos] != K[i - 1]) pos = T[pos];
		T[i] = pos + 1;
	}

	int sp = 0;
	int kp = 0;
	while(sp < S.size())
	{
		while(kp != -1 && (kp == K.size() || K[kp] != S[sp])) kp = T[kp];
		kp++;
		sp++;
		if(kp == K.size()) matches.push_back(sp - K.size());
	}

	return matches;
}

int levenshtein_distance(const std::string& s1, const std::string& s2)
{
	std::string a = s1, b = s2;

	for(size_t i = 0; i < a.length();i++)
		a[i] = toupper(a[i]);

	for(size_t i = 0; i < b.length();i++)
		b[i] = toupper(b[i]);

	const std::size_t len1 = a.size(), len2 = b.size();
	std::vector<int> col(len2+1), prevCol(len2+1);

	for (int i = 0; i < prevCol.size(); i++)
		prevCol[i] = i;
	for (int i = 0; i < len1; i++) {
		col[0] = i+1;
		for (int j = 0; j < len2; j++)
			col[j+1] = std::min(std::min(prevCol[1 + j] + 1, col[j] + 1) , prevCol[j] + (a[i]==b[j] ? 0 : 1) );
		col.swap(prevCol);
	}
	return prevCol[len2];
}
