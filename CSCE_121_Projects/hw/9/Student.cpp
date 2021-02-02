#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Student.h"

using namespace std;

Student::Student(string id, string name)
{
    this->id = id;
    this->name = name;
    std::vector<std::string> course_ids;
}

std::string Student::get_id()
{
    return id;
}

std::string Student::get_name()
{
    return name;
}

void Student::addCourse(std::string course_id)
{
    for (unsigned int i = 0; i < course_ids.size(); ++i)
    {
        if (i == course_ids.size() - 1)
            course_ids.push_back(course_id);
    }
}

void Student::listCourses()
{
    if (course_ids.size() == 0)
        cout << "No courses" << endl;
    for (unsigned int i = 0; i < course_ids.size(); ++i)
    {
        cout << course_ids[i] << endl;
    }
}