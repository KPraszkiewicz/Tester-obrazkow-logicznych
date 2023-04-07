#pragma once

#include<vector>

using uchar = unsigned char;
using uint = unsigned int;

class Obrazek_baza
{
protected:
	
public:
	std::vector<std::vector<uchar>> wiersze;
	std::vector<std::vector<uchar>> kolumny;
	std::vector<std::vector<uchar>> dane;
	enum TYP_POLA
	{
		PUSTE,
		KROPKA,
		WYPELNIONE,
		POZA_POLEM
	};

	uint max_k, max_w;
	uint wym_x, wym_y;
};