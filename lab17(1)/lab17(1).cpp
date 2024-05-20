#include <iostream>
#include <Windows.h>
#include <ctime>
using namespace std;

class Date {
protected:
    int year;
    int month;
    int day;

    bool isLeapYear(int year) const {
        if (year % 4 != 0) return false;
        if (year % 100 != 0) return true;
        if (year % 400 != 0) return false;
        return true;
    }

    int daysInMonth(int year, int month) const {
        if (month < 1 || month > 12) throw invalid_argument("Недійсний місяць");
        static const int daysInMonths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (month == 2 && isLeapYear(year)) return 29;
        return daysInMonths[month - 1];
    }

    void validateDate(int year, int month, int day) const {
        if (month < 1 || month > 12) throw invalid_argument("Недійсний місяць");
        if (day < 1 || day > daysInMonth(year, month)) throw invalid_argument("Недійсний день");
    }

public:
    Date(int year, int month, int day) {
        validateDate(year, month, day);
        this->year = year;
        this->month = month;
        this->day = day;
    }

    void setYear(int year) {
        validateDate(year, this->month, this->day);
        this->year = year;
    }

    void setMonth(int month) {
        validateDate(this->year, month, this->day);
        this->month = month;
    }

    void setDay(int day) {
        validateDate(this->year, this->month, day);
        this->day = day;
    }

    void addDays(int days) {
        while (days > 0) {
            int daysInCurrentMonth = daysInMonth(year, month);
            if (day + days <= daysInCurrentMonth) {
                day += days;
                return;
            }
            else {
                days -= (daysInCurrentMonth - day + 1);
                day = 1;
                if (++month > 12) {
                    month = 1;
                    ++year;
                }
            }
        }
    }

    void addMonths(int months) {
        int totalMonths = month + months;
        int newYear = year + totalMonths / 12;
        int newMonth = totalMonths % 12;
        if (newMonth == 0) {
            newMonth = 12;
            newYear -= 1;
        }
        validateDate(newYear, newMonth, day);
        year = newYear;
        month = newMonth;
    }

    void addYears(int years) {
        year += years;
        validateDate(year, month, day);
    }

    int getYear() const { return year; }
    int getMonth() const { return month; }
    int getDay() const { return day; }
};

class DateWithDayOfWeek : public Date {
private:
    const char* dayOfWeek(int year, int month, int day) {
        static const char* days[] = { "Неділя", "Понеділок", "Вівторок", "Середа", "Четвер", "П'ятниця", "Субота" };
        tm time_in = { 0, 0, 0, day, month - 1, year - 1900 };
        time_t time_temp = mktime(&time_in);
        if (time_temp == -1) {
            throw runtime_error("Недійсна дата для визначення дня тижня");
        }
        tm time_out;
        localtime_s(&time_out, &time_temp);
        return days[time_out.tm_wday];
    }

public:
    DateWithDayOfWeek(int year, int month, int day) : Date(year, month, day) {}

    void printDayOfWeek() {
        cout << "День тижня: " << dayOfWeek(getYear(), getMonth(), getDay()) << endl;
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    try {
        int year, month, day;
        cout << "Введіть рік: ";
        cin >> year;
        cout << "Введіть місяць: ";
        cin >> month;
        cout << "Введіть день: ";
        cin >> day;

        DateWithDayOfWeek date(year, month, day);
        cout << "Дата: " << date.getYear() << "-" << (date.getMonth() < 10 ? "0" : "") << date.getMonth() << "-" << (date.getDay() < 10 ? "0" : "") << date.getDay() << endl;
        date.printDayOfWeek();

        date.addDays(10);
        cout << "Після додавання 10 днів: ";
        cout << "Дата: " << date.getYear() << "-" << (date.getMonth() < 10 ? "0" : "") << date.getMonth() << "-" << (date.getDay() < 10 ? "0" : "") << date.getDay() << endl;
        date.printDayOfWeek();

        date.addMonths(2);
        cout << "Після додавання 2 місяців: ";
        cout << "Дата: " << date.getYear() << "-" << (date.getMonth() < 10 ? "0" : "") << date.getMonth() << "-" << (date.getDay() < 10 ? "0" : "") << date.getDay() << endl;
        date.printDayOfWeek();

        date.addYears(1);
        cout << "Після додавання 1 року: ";
        cout << "Дата: " << date.getYear() << "-" << (date.getMonth() < 10 ? "0" : "") << date.getMonth() << "-" << (date.getDay() < 10 ? "0" : "") << date.getDay() << endl;
        date.printDayOfWeek();
    }
    catch (const exception& e) {
        cerr << "Помилка: " << e.what() << endl;
    }

    return 0;
}
