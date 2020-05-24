/*
Необходимо написать программу на С++, которая реализует работу с простой базой данных (сокращённо «БД»). Программа будет общаться с пользователем через стандартный ввод и вывод (потоки stdin и stdout).
Будем хранить в нашей БД пары вида: дата, событие. Определим дату как строку вида Год-Месяц-День, где Год, Месяц и День — целые числа.
Событие определим как строку из произвольных печатных символов без разделителей внутри (пробелов, табуляций и пр.). Событие не может быть пустой строкой. В одну и ту же дату может произойти много разных событий, БД должна суметь их все сохранить. Одинаковые события, произошедшие в один и тот же день, сохранять не нужно: достаточно сохранить только одно из них.
Наша БД должна понимать определённые команды, чтобы с ней можно было взаимодействовать:
добавление события:                        Add Дата Событие
- удаление события:                          Del Дата Событие
- удаление всех событий за конкретную дату:  Del Дата
- поиск событий за конкретную дату:          Find Дата
- печать всех событий за все даты:           Print

*/
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <iomanip>

using namespace std;

class Date {
public:
    Date(const string& date) {
        char zn1, zn2;
        string zn3;

        istringstream input(date + "-");
        if( input ) {
            input >> m_year >> zn1 >> m_month >> zn2 >> m_day >> zn3;
            if( zn1 != '-' || zn2 != '-' || zn3 != "-" ) {
                throw invalid_argument("Wrong date format: " + date);
            } else if (1 > m_month || m_month > 12) {
                throw invalid_argument("Month value is invalid: " + to_string(m_month));
            } else if (1 > m_day || m_day > 31) {
                throw invalid_argument("Day value is invalid: " + to_string(m_day));
            }
        }
    }


    //Год-Месяц-День
    int GetYear() const
    {
        return m_year;
    }

    int GetMonth() const
    {
        return m_month;
    }

    int GetDay() const
    {
        return  m_day;
    }

private:
    int m_year;
    int m_month;
    int m_day;
};

ostream& operator<<(ostream& out, const Date& date)
{
    return out << setfill('0') << setw(4) << date.GetYear()
               << "-" << setw(2) << date.GetMonth() << "-" << setw(2) << date.GetDay();
}

bool operator<(const Date& lhs, const Date& rhs)
{
    if( lhs.GetYear() != rhs.GetYear() ) {
        return lhs.GetYear() < rhs.GetYear();
    } else if( lhs.GetMonth() != rhs.GetMonth() ) {
        return lhs.GetMonth() < rhs.GetMonth();
    } else {
        return lhs.GetDay() < rhs.GetDay();
    }
}

ostream& operator<<(ostream& out, const map<Date, set<string>> events)
{
    for( const auto& i : events) {
        if( i.first.GetYear() >= 0 ) {
            for( const auto& event : i.second) {
                out << i.first << " " << event << endl;
            }
        } else {
            continue;
        }
    }
    return out;
}

class Database {
public:
    void AddEvent(const Date& date, const string& event)
    {
        events[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const string& event)
    {
        if( events.count(date) == 1 ) {
            if (events[date].count(event) == 1 && events[date].size() == 1) {
                return events.erase(date);
            } else {
                return events[date].erase(event);
            }
        }
        return false;
    }

    int  DeleteDate(const Date& date)
    {
        int count{0};
        auto value = events.find(date);
        if( value != events.end()){
            count = events[date].size();
            events.erase(value);

        }
        return count;
    }

    // поиск событий за конкретную дату
    void Find(const Date& date) const
    {
		if( !events.empty() && (events.count(date)) > 0 ) {
			for( const auto& i : events.at(date))
				cout << i << endl;
		}
    }

    void Print() const
    {
        cout << events << endl;
    }

private:
    map<Date, set<string>> events;
};


int main() {
    Database db;
    string command;
    while (getline(cin, command)) {
        string event, date;
        try {
            istringstream in(command);
            in >> command;
            in >> date >> event;
            if (command == "Add") {
                db.AddEvent({ date }, event);
            } else if (command == "Print") {
                db.Print();
            } else if (command == "Find") {
                db.Find(date);
            } else if (command == "Del") {
                if (event.size() == 0) {
					int count = db.DeleteDate(date);
					cout << "Deleted " << count << " events\n";
                } else {
                    if (db.DeleteEvent({ date }, event)) {
                        cout << "Deleted successfully" << endl;
                    } else {
                        cout << "Event not found" << endl;
                    }
                }
            } else if (command == "") {

            } else {
                cout << "Unknown command: " << command;
                break;
            }
        }
        catch (invalid_argument& ia) {
            cout << ia.what() << endl;
            return 1;
        }
    }
    return 0;
}
