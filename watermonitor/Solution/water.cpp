// COMP2811 Coursework 1 sample solution: Quake class

#include <stdexcept>
#include <sstream>
#include "water.hpp"

using namespace std;


Water::Water(const string& id, const string& loc, const string& tm, const string& det, double res, const string& uni, const string& ty):
  ID(id), location(loc), time(tm), determinand(det), result(res), unit(uni), type (ty), latitude(0), longitude(0)
{
}
