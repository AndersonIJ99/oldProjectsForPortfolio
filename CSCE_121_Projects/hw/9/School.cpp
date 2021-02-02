#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include "School.h"
#include "AttendanceRecord.h"

using namespace std;

void School::addStudents(string filename)
{
  ifstream ifs(filename);
  if (!ifs.is_open())
  {
    cout << "Unable to open file: " << filename << endl;
    return;
  }
  while (!ifs.eof())
  {
    string line;
    getline(ifs, line);
    istringstream ss(line);
    string uin;
    getline(ss, uin, ',');
    string name;
    getline(ss, name);
    if (!ss.fail())
    {
      students.push_back(Student(uin, name));
    }
  }
}

void School::addCourses(std::string filename)
{
  ifstream ifs(filename);
  if (!ifs.is_open())
  {
    cout << "Unable to open file: " << filename << endl;
    return;
  }
  while (!ifs.eof())
  {
    string line;
    getline(ifs, line);
    istringstream ss(line);
    string id;
    getline(ss, id, ',');

    string start_hour;
    getline(ss, start_hour, ':');
    int start_hour_num;
    std::istringstream(start_hour) >> start_hour_num;
    string start_minute;
    getline(ss, start_minute, ',');
    int start_minute_num;
    std::istringstream(start_minute) >> start_minute_num;

    string end_hour;
    getline(ss, end_hour, ':');
    int end_hour_num;
    std::istringstream(end_hour) >> end_hour_num;
    string end_minute;
    getline(ss, end_minute, ',');
    int end_minute_num;
    std::istringstream(end_minute) >> end_minute_num;

    string title;
    getline(ss, title);
    if (!ss.fail())
    {
      courses.push_back(Course(id, title, Date(start_hour_num, start_minute_num, 0), Date(end_hour_num, end_minute_num, 0)));
    }
  }
}
void School::addAttendanceData(std::string filename)
{
  ifstream ifs(filename);
  if (!ifs.is_open())
  {
    cout << "Unable to open file: " << filename << endl;
    return;
  }
  while (!ifs.eof())
  {
    string line;
    getline(ifs, line);
    istringstream ss(line);

    string year;
    getline(ss, year, '-');
    int year_num;
    std::istringstream(year) >> year_num;

    string month;
    getline(ss, month, '-');
    int month_num;
    std::istringstream(month) >> month_num;

    string day;
    getline(ss, day, ' ');
    int day_num;
    std::istringstream(day) >> day_num;

    string hour;
    getline(ss, hour, ':');
    int hour_num;
    std::istringstream(hour) >> hour_num;

    string minute;
    getline(ss, minute, ':');
    int minute_num;
    std::istringstream(minute) >> minute_num;

    string second;
    getline(ss, second, ',');
    int second_num;
    std::istringstream(second) >> second_num;

    string id;
    getline(ss, id, ',');
    string uin;
    getline(ss, uin);

    if (!ss.fail())
    {
      AttendanceRecord rec = AttendanceRecord(id, uin, Date(year_num, month_num, day_num, hour_num, minute_num, second_num));
      for (int i = 0; i < courses.size(); ++i)
      {
        if (rec.getCourseID() == courses.at(i).getID() && rec.getDate() >= courses.at(i).getStartTime() && rec.getDate() <= courses.at(i).getEndTime())
          courses.at(i).addAttendanceRecord(rec);
      }
    }
  }
}
void School::listCourses()
{
  if (courses.empty())
    cout << "No Courses" << endl;
  else
  {
    for (int i = 0; i < courses.size(); ++i)
    {
      cout << courses[i].getID() + "," + courses[i].getStartTime().getTime() + "," + courses[i].getEndTime().getTime() + "," + courses[i].getTitle() << endl;
    }
  }
}
void School::listStudents()
{
  if (students.empty())
    cout << "No Students" << endl;
  else
  {
    for (int i = 0; i < students.size(); ++i)
    {
      cout << students[i].get_id() + "," + students[i].get_name() << endl;
    }
  }
}

void School::outputCourseAttendance(std::string course_id)
{
  if (courses.empty())
    cout << "No records" << endl;
  else
  {
    for (int i = 0; i < courses.size(); ++i)
    {
      if (course_id == courses[i].getID())
        courses[i].outputAttendance();
    }
  }
}
void School::outputStudentAttendance(std::string student_id, std::string course_id)
{
  if (courses.empty())
    cout << "No records" << endl;
  else
  {
    for (int i = 0; i < courses.size(); ++i)
    {
      if (course_id == courses[i].getID())
        courses[i].outputAttendance(student_id);
    }
  }
}
