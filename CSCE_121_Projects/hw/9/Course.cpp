#include <iostream>
#include <fstream>
#include <string>
#include "Course.h"
#include "Date.h"
#include "AttendanceRecord.h"

using namespace std;

Course::Course(std::string id, std::string title, Date startTime, Date endTime) : startTime(startTime.getHour(), startTime.getMin(), startTime.getSec()), endTime(endTime.getHour(), endTime.getMin(), endTime.getSec())
{
    this->id = id;
    this->title = title;
    //vector<AttendanceRecord> attendanceRecords;
}
std::string Course::getID()
{
    return id;
}
std::string Course::getTitle()
{
    return title;
}
Date Course::getStartTime()
{
    return startTime;
}
Date Course::getEndTime()
{
    return endTime;
}
void Course::addAttendanceRecord(AttendanceRecord ar)
{
    attendanceRecords.push_back(ar);
}
void Course::outputAttendance()
{
    if (attendanceRecords.empty())
        cout << "No records" << endl;
    else
    {
        for (unsigned int i = 0; i < attendanceRecords.size(); ++i)
        {
            cout << attendanceRecords[i].getDate().getDate() + "," + attendanceRecords[i].getCourseID() + "," + attendanceRecords[i].getStudentID() << endl;
        }
    }
}
void Course::outputAttendance(std::string student_id)
{
    bool check = false;
    if (attendanceRecords.empty())
        cout << "No Records" << endl;
    else
    {
        for (int i = 0; i < attendanceRecords.size(); ++i)
        {
            if (attendanceRecords[i].getStudentID() == student_id)
            {
                cout << attendanceRecords[i].getDate().getDate() + "," + attendanceRecords[i].getCourseID() + "," + student_id << endl;
                check = true;
            }
        }
        if (!check)
            cout << "No Records" << endl;
    }
}