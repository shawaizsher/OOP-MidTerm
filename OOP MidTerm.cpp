#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

class Restaurant;

class Reservation
{
    Restaurant* restaurant;
    string name;
    string startTime;
    string endTime;

public:
    Reservation(string n, string st, string et) : name(n), startTime(st), endTime(et) {}
    string getName() const { return name; }
    string getStartTime() const { return startTime; }
    string getEndTime() const { return endTime; }
};

class Restaurant
{
private:
    vector<string> slots;
    vector<Reservation> reservations; 
    string open;
    string close;
    int slotDuration;

public:
    Restaurant(string o, string c, int duration) : open(o), close(c), slotDuration(duration) {}

    void generateTimeSlots()
    {
        int currentTime = stoi(open.substr(0, 2)) * 60 + stoi(open.substr(3)) + 60;
        int closingTime = stoi(close.substr(0, 2)) * 60 + stoi(close.substr(3));

        while (currentTime < closingTime)
        {
            int hours = currentTime / 60;
            int minutes = currentTime % 60;
            string slot = to_string(hours) + ":" + (minutes < 10 ? "0" : "") + to_string(minutes);
            slots.push_back(slot);
            currentTime += slotDuration;
        }
    }

    void displayAvailableSlots() const
 {
    cout << "Available Time Slots:" << endl;
    for (size_t i = 0; i + 1 < slots.size(); ++i)
    {
        cout << slots[i] << " - " << slots[i + 1];
        bool booked = false;
        for (const auto& res : reservations)
        {
            if (res.getStartTime() == slots[i])
            {
                cout << " (Reserved by: " << res.getName() << ")";
                booked = true;
                break;
            }
        }
        if (!booked)
        {
            cout << " (Available)";
        }
        cout << endl;
    }
}
    void bookReservation(const Reservation& reservation)
    {
        auto it = find(slots.begin(), slots.end(), reservation.getStartTime());
        if (it != slots.end())
        {
            slots.erase(it);
			cout << "Reservation for " << reservation.getName() << " of " << reservation.getStartTime() << " is confirmed." << endl;
        }
        else
        {
            cout << "Sorry, the requested time slot is not available." << endl;
        }
    }

    string getSlot(int index) const
    {
        if (index >= 0 && index < slots.size())
            return slots[index];
        else
            return "Invalid index";
    }

    int getSlotCount() const
    {
        return slots.size();
    }
};

class WeekSchedule
{
    vector<Restaurant> restaurants;
    vector<string> days = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
public:
    WeekSchedule()
    {
        for (int i = 0; i < 7; ++i)
        {
            restaurants.push_back(Restaurant("09:00", "22:00", 60)); // Opening and closing times assumed to be same for all days
            restaurants[i].generateTimeSlots();
        }
    }

    void displayDaySchedule(int day) const
    {
        if (day >= 1 && day <= 7)
        {
            cout << "Schedule for " << days[day - 1] << ":" << endl;
            restaurants[day - 1].displayAvailableSlots(); 
        }
        else
        {
            cout << "Invalid day number. Please enter a number between 1 and 7." << endl;
        }
    }

    void displayWeekSchedule() const
    {
        for (int i = 0; i < 7; ++i)
        {
            cout << "Schedule for " << days[i] << ":" << endl;
            restaurants[i].displayAvailableSlots();
            cout << endl;
        }
        system("pause");
    }

    void bookReservation(int day, const Reservation& reservation)
    {
        if (day >= 1 && day <= 7)
        {
            restaurants[day - 1].bookReservation(reservation); 
        }
        else
        {
            cout << "Invalid day number. Please enter a number between 1 and 7." << endl;
        }
    }

    string getSlot(int day, int index) const
    {
        if (day >= 1 && day <= 7)
        {
            return restaurants[day - 1].getSlot(index);
        }
        else
        {
            cout << "Invalid day number. Please enter a number between 1 and 7." << endl;
            return "Invalid index";
        }
    }

    int getSlotCount(int day) const
    {
        if (day >= 1 && day <= 7)
        {
            return restaurants[day - 1].getSlotCount();
        }
        else
        {
            cout << "Invalid day number. Please enter a number between 1 and 7." << endl;
            return 0;
        }
    }
};

int main()
{
    WeekSchedule weekSchedule;

    int choice;
    do
    {
    	system("cls");
        cout << "\nShawaiz's Restaurant";
        cout << "\n==================";
        cout << "\n1. Display weekly schedule";
        cout << "\n2. Add reservation";
        cout << "\n3. Exit the program";
        cout << "\n==================";
        cout << "\n-> Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            weekSchedule.displayWeekSchedule();
            break;
        case 2:
        {
            int day, slotIndex; 
            string name, startTime, endTime;
            cout << "Enter day (1-7): "; 
            cin >> day;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            weekSchedule.displayDaySchedule(day);
            cout << "\nEnter the slot number you want to reserve: ";
            cin >> slotIndex;
            if (slotIndex >= 1 && slotIndex <= weekSchedule.getSlotCount(day))
            {
                startTime = weekSchedule.getSlot(day, slotIndex - 1);
                endTime = startTime; 
                Reservation res(name, startTime, endTime);
                weekSchedule.bookReservation(day, res);
                
                system("pause");
            }
            else
            {
                cout << "Invalid slot number. Please try again." << endl;
            }
            break;
        }
        case 3:
            cout << "Exiting the program." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);

    return 0;
}
