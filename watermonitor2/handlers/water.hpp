

#pragma once

#include <string>
#include <iostream>
#include <QDateTime>

class Water
{
public:
  Water(const std::string &, const std::string &, const std::string &, const std::string &, double, const std::string &, const std::string &, const double lat,const double lon, bool);
  std::string getID() const { return ID; }
  std::string getLocation() const { return location; }
  std::string getTime() const { return time; }
  QDateTime getDateTime() const { return dateTime; }
  std::string getDeterminand() const { return determinand; }
  double getResult() const { return result; }
  std::string getUnit() const { return unit; }
  std::string getWaterType() const { return type; }
  double getLat() const { return latitude; }
  double getLong() const { return longitude; }
  void setLatLong(double lat, double lon)
  {
    latitude = lat;
    longitude = lon;
  }

  bool isComplianceSample() const { return m_isComplianceSample; }

private:
  std::string ID;
  std::string location;
  std::string time;
  QDateTime dateTime;
  std::string determinand;
  double result;
  std::string unit;
  std::string type;
  double longitude;
  double latitude;
  bool m_isComplianceSample;
};
