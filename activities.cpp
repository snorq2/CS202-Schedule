/*
 * Sawyer Norquist
 * CS202
 * Program 1
 * activities.cpp
 * 7/8/2020
 *
 * This code block contains the implementations for the time and activity classes.
 */

#include "activities.h"

//takes in a single char, converts to ASCII equivalent and offsets to output the equivalent number
//If the output is not a number between 0 and 9, outputs -1 to indicate input error
int time_clock::convert_int(char source) const
{

    int converted = 0; //holds converted value for analysis

    converted = source - 48;  //ASCII conversion

    if(converted < 0 || converted > 9)
        return -1;
    else
        return converted;

}

//-1 is zero equivalent value for time_clock
time_clock::time_clock():clock(-1){}

//copy constructor
time_clock::time_clock(const time_clock &source):clock(source.clock){}

//takes in a time_clock object and returns the difference in minutes
//A positive value indicates the calling object is later
//returns 1500 on error
int time_clock::compare(const time_clock &comparator) const
{

    if(clock <= -1 || clock >= 1440 || comparator.clock <= -1 || comparator.clock >= 1440)
        return 1500;
    else
        return comparator.clock - clock;

}

//converts internal int into a readible clock output
void time_clock::display() const
{

    using std::cout;
    using std::endl;

    if(clock == -1)
        cout << "Clock has not been set" << endl;
    else if(clock <= -2 || clock >= 1440)
        cout << "Clock error - out of bounds" << endl;
    else
    {

        if(clock / 60 <= 9)
            cout << 0;

        cout << clock / 60 << ":";

        if(clock % 60 <= 9)
            cout << 0;
            
        cout << clock % 60;

    }

    return;

}

//calls for user input and parses entry for conversion to internal int form
//Would have modified to accept time without a leading zero, but ran short on time
void time_clock::set()
{

    using std::cin;
    using std::cout;
    using std::endl;

    const int Limit = 6; //max size for input char array
    int clock_temp = 0;  //clock value is held separately until all input is validated
    char input[Limit] = {};  //char array for input from user
    int next = 0;  //holder for next digit in the char array
    int hour_tens = 0;  //need special holder for the hour tens - if 2, then the next variable must be 3 or smaller
    bool l_continue = false;  //controls while loop.  Starts false to initiate loop the first time

    while(!l_continue)
    {
        l_continue = true;  //assume loop runs once unless one of the char conversions fail

        cin.get(input, Limit);

        //verify input is the right length, set l_continue to false if not
        if(strlen(input) != 5)
            l_continue = false;

        hour_tens = convert_int(input[0]);

        //all the following char checks will set l_continue to false if they fail, causing the user to re-enter until valid
        //verifies first char is a valid clock entry
        if(l_continue && hour_tens <= 2 && hour_tens >= 0)
            clock_temp = hour_tens * 600;
        else
            l_continue = false;

        next = convert_int(input[1]);

        //checks second char, if first char was 2 then limited to 3 max
        if(l_continue && next != -1 && hour_tens <= 1 && hour_tens >= 0)
            clock_temp = clock_temp + (next * 60);
        else if(l_continue && next >= 0 && next <= 3 && hour_tens == 2)
            clock_temp = clock_temp + (next * 60);
        else
            l_continue = false;

        //makes sure third char is :
        if(l_continue && input[2] != 58)
            l_continue = false;

        next = convert_int(input[3]);

        //tens of minutes maxes out at 5
        if(l_continue && next >= 0 && next <=5)
            clock_temp = clock_temp + (next * 10);
        else
            l_continue = false;

        next = convert_int(input[4]);

        if(l_continue && next != -1)
            clock_temp = clock_temp + next;
        else
            l_continue = false;

        //output error if a check failed
        if(!l_continue)
            cout << endl << "Expecting 5 characters in format 00:00 from 00:00 to 23:59" << endl << "Please try again: ";

        //clear the buffer!
        cin.clear();
        cin.ignore(255, '\n');

    }

    clock = clock_temp;

    return;

}

//duplicates settings from another time_clock object
void time_clock::set(const time_clock &source)
{

    clock = source.clock;

    return;

}

//Simply makes sure clock is not in error state, nor at zero equivalent
bool time_clock::is_set() const
{

    if(clock <= -1 || clock >= 1440)
        return false;
    else
        return true;

}

//reset clock to zero equivalent
void time_clock::clear()
{

    clock = -1;

    return;

}

activity::activity():max(0), min(0){}

activity::activity(int min_input, int max_input):max(max_input), min(min_input){}

activity::activity(const activity &source):max(source.max), min(source.min), start(source.start), end(source.end){}

//Display start and end times, using functions from time_clock
void activity::display() const
{

    using std::cout;
    using std::endl;

    cout << "Start time: ";
    start.display();
    cout << endl << "End time: ";
    end.display();
    cout << endl;

    return;

}

//Takes in time_clock object and outputs in minutes how much time until the activity relative to the argument
int activity::time_until(const time_clock &comparator) const
{

    return comparator.compare(start);

}

//Takes in time_clock object and outputs in minutes how much time after the activity relative to the argument
int activity::time_after(const time_clock &comparator) const
{

    return end.compare(comparator);

}

//Returns total duration of activity in minutes
int activity::duration() const
{

    return start.compare(end);

}

//Base activity that takes in a time_clock and two activity objects
//Runs a series of checks to insure there are no interferences between the proposed start time and the two activities
//Returns false if interference is found, otherwise returns true and changes the start time
//Intended to be used with a list, where the activities before and after are passed as arguments
bool activity::change_start(const time_clock &new_time, activity &activity_a, activity &activity_b)
{

    if(!new_time.is_set())
        return false;

    //Want to make sure activity_a is set, else rest of check must be skipped or the math won't work
    //Also needs to make sure the end is set, or it's impossible to calculate the span
    if(activity_a.is_set() && activity_a.time_after(new_time) < 0 && activity_a.time_after(end) > 0 && end.is_set())
        return false;

    if(activity_b.is_set() && activity_b.time_after(new_time) < 0 && activity_a.time_after(end) > 0 && end.is_set())
        return false;

    //if end time is not set, need to make sure new start isn't in the middle of the activity
    if(activity_a.is_set() && !end.is_set() && activity_a.is_during(new_time))
        return false;

    if(activity_b.is_set() && !end.is_set() && activity_b.is_during(new_time))
        return false;

    //can set start if above checks are passed, and end is not set
    if(!end.is_set())
    {

        start.set(new_time);
        return true;

    }

    //If min/max times are not specified, just make sure start is before end
    if(min == 0 && max == 0 && new_time.compare(end) > 0 )
    {

        start.set(new_time);
        return true;

    }

    //if min/max times ARE specified, also check to be sure that span is met
    if(new_time.compare(end) > 0 && new_time.compare(end) >= min && new_time.compare(end) <= max)
    {

        start.set(new_time);
        return true;

    }

    return false;

}

//Built in function to set up and call the previous change_start function
//Takes in two activites to compare against.  All other input is from user.
void activity::change_start(activity activity_a, activity activity_b)
{

    using std::cout;
    using std::endl;

    time_clock new_time;  //Object to solicit user input and run comparisons against

    cout << "What would you like your new start time to be? ";
    new_time.set();

    //Run the previous change_start function and call out if an error has occurred
    //Repeat until user input is good
    while(!change_start(new_time, activity_a, activity_b))
    {

        cout << "Time invalid - cannot be after the end time, or interfere with another activity" << endl;
 
        if(min != 0 || max != 0)
        {

            cout << "Also verify time is within minimum and maximum allotted" << endl;
            cout << "Minimum time allowed is " << min / 60 << " hours and " << min % 60 << " minutes" << endl;
            cout << "Maximum time allowed is " << max / 60 << " hours and " << max % 60 << " minutes" << endl;

        }

        cout << "Current activity time" << endl;
        display();
        cout << endl << endl << "Activity A time:" << endl;
        activity_a.display();
        cout << endl << endl << "Activity B time:" << endl;
        activity_b.display();
        cout << endl << "Please enter a new time" << endl;
        cout << "What would you like your new start time to be? ";

        new_time.set();

    }

    return;

}

//Same logic and intention of change_start, except setting the end time
bool activity::change_end(const time_clock &new_time, activity &activity_a, activity &activity_b)
{

    if(!new_time.is_set())
        return false;

    if(activity_a.is_set() && activity_a.time_until(new_time) < 0 && activity_a.time_until(start) > 0 && start.is_set())
        return false;

    if(activity_b.is_set() && activity_b.time_until(new_time) < 0 && activity_b.time_until(start) > 0 && start.is_set())
        return false;

    if(activity_a.is_set() && !start.is_set() && activity_a.is_during(new_time))
        return false;

    if(activity_b.is_set() && !start.is_set() && activity_b.is_during(new_time))
        return false;

    if(!start.is_set())
    {

        end.set(new_time);
        return true;

    }

    if(min == 0 && max == 0 && start.compare(new_time) > 0)
    {

        end.set(new_time);
        return true;

    }

    if(start.compare(new_time) > 0 && start.compare(new_time) >= min && start.compare(new_time) <= max)
    {

        end.set(new_time);
        return true;

    }

    return false;

}

//Same logic and intention of change_start, except setting the end time
void activity::change_end(activity activity_a, activity activity_b)
{

    using std::cout;
    using std::endl;

    time_clock new_time;

    cout << "What would you like your new end time to be? ";
    new_time.set();

    while(!change_end(new_time, activity_a, activity_b))
    {

        cout << "Time invalid - cannot be after the end time, or interfere with another activity" << endl;

        if(min != 0 || max != 0)
        {

            cout << "Also verify time is within minimum and maximum allotted" << endl;
            cout << "Minimum time allowed is " << min / 60 << " hours and " << min % 60 << " minutes" << endl;
            cout << "Maximum time allowed is " << max / 60 << " hours and " << max % 60 << " minutes" << endl;

        }

        cout << "Current activity time" << endl;
        display();
        cout << endl << endl << "Activity A time:" << endl;
        activity_a.display();
        cout << endl << endl << "Activity B time:" << endl;
        activity_b.display();
        cout << endl << "Please enter a new time" << endl;
        cout << "What would you like your new end time to be? ";

        new_time.set();

    }

    return;

}

//Take in a time_clock object, check against start and end time
//Return true if argument is between start and end, false if otherwise
bool activity::is_during(const time_clock &comparator) const
{

    if(!comparator.is_set())
        return false;

    if(start.compare(comparator) > 0 && end.compare(comparator) < 0)
        return true;
    else
        return false;

}

//Verifies both start and end time_clock objects are set
bool activity::is_set() const
{

    if(start.is_set() && end.is_set())
        return true;

    return false;

}

//Reset start and end to zero equivalent
void activity::clear()
{

    start.clear();
    end.clear();

    return;

}

//Default constructor sets the min and max times to the statics from the .h file
work::work(): activity(WORK_MIN, WORK_MAX), employer(NULL), wages(0){}

work::work(const work &source):activity(source)
{

    wages = source.wages;

    if(!source.employer)
        employer = NULL;
    else
    {

        employer = new char[strlen(source.employer + 1)];
        strcpy(source.employer, employer);

    }

}

work::~work()
{

    if(employer)
        delete[] employer;

}

//output work-specific data, then call base display function
void work::display() const
{

    using std::cout;
    using std::endl;

    //Makes sure there's data to display first
    if(!is_set())
    {

        cout << "Work data not set" << endl;
        return;

    }

    cout << "Employer: " << employer << endl;

    activity::display();

    cout << endl << "You are making $" << wages << " per hour." << endl;

    return;

}

//Gather user input for work-specific fields
void work::set()
{

    using std::cout;
    using std::cin;
    using std::endl;

    char *employer_check = new char[MAX_INPUT_SIZE + 1];  //temporary holder, used in case of failure to protect class data.  Additionally used to right-size data in class data member
    int wages_check = 0;  //Temporary holder, used in case of failure to protect class data

    cout << "What is your employer's name? ";

    cin.get(employer_check, MAX_INPUT_SIZE);

    cin.clear();
    cin.ignore(255, '\n');

    cout << "What will you be making per hour? Whole dollars only: ";

    cin >> wages_check;

    while(cin.fail())
    {

        cin.clear();
        cin.ignore(255, '\n');

        cout << "Bad input - expecting whole number with no symbols, please try again" << endl;
        cout << "What will you be making per hour? Whole dollars only: ";

        cin >> wages_check;

    }

    //Resize class data member to user input size
    employer = new char[strlen(employer_check) + 1];
    strcpy(employer, employer_check);
    wages = wages_check;

    //delete temporary char array
    delete[] employer_check;

    return;

}

//Reset work to zero equivalent
void work::clear()
{

   if(employer)
   {

       delete[] employer;
       employer = NULL;

   }

   wages = 0;

   activity::clear();

}

//User interface for all functions needed to interact with the work class object
void work::menu(activity &activity_a, activity &activity_b)
{

    using std::cout;
    using std::cin;
    using std::endl;

    int input = 7;


    while(input != 0)
    {

        cout << endl << "1.  Display work" << endl;
        cout << "2.  Show income of work shift" << endl;
        cout << "3.  Set all" << endl;
        cout << "4.  Set new start" << endl;
        cout << "5.  Set new end" << endl;
        cout << "6.  Set new employer and income" << endl;
        cout << "0.  Go back" << endl;
        cout << "Please choose an option number: ";

        cin >> input;

        if(!cin.good() || input < 0 || input > 6)
        {

            cout << endl << "I'm sorry, I don't understand - please try again" << endl;
            input = 7;

        }

        cin.clear();
        cin.ignore(255, '\n');

        switch(input)
        {

            case 1:
                display();
                break;

            case 2:
                cout << endl << "You are earning $" << income() << " this shift" << endl;
                break;

            case 3:
                change_start(activity_a, activity_b);
                change_end(activity_a, activity_b);
                set();
                break;

            case 4:
                change_start(activity_a, activity_b);
                break;

            case 5:
                change_end(activity_a, activity_b);
                break;

            case 6:
                set();
                break;

        }

    }

    return;
    
}

//Call duration and multiply by class wage, divided by 60 to make this hourly
//Returns int, so will have trunc errors - did not wish to work with float in the scope of this project
int work::income() const
{

    return wages * activity::duration() / 60;

}

//Verify work has all data members filled
bool work::is_set() const
{

    if(activity::is_set() && employer)
        return true;

    return false;

}

reading::reading():book(NULL), author(NULL), pages(0){}

reading::reading(const reading &source):activity(source)
{

    if(source.book)
    {

        book = new char[strlen(source.book) + 1];
        strcpy(book, source.book);

    }else
        book = NULL;

    if(source.author)
    {

        author = new char[strlen(source.author) + 1];
        strcpy(author, source.author);

    }else
        book = NULL;

    pages = source.pages;

}

reading::~reading()
{

    if(book)
        delete[] book;

    if(author)
        delete[] author;

}

//Show user information relevant to reading class, also call base class display
void reading::display() const
{

    using std::cout;
    using std::endl;

    if(!is_set())
    {

        cout << "Reading is not set" << endl;
        return;

    }

    cout << "You are reading the book " << book;
    cout << " by " << author << endl;
    cout << "It is " << pages << " pages long." << endl;

    activity::display();

    return;

}

//Gather user input and set reading-specific data
void reading::set()
{

    using std::cout;
    using std::cin;
    using std::endl;

    char *book_check = new char[MAX_INPUT_SIZE + 1];  //Temporary holder for user input, used to rightsize data members, and prevent bad data from escaping
    char *author_check = new char[MAX_INPUT_SIZE + 1];  //Temporary holder for user input, used to rightsize data members, and prevent bad data from escaping
    int pages_check = 0;  //Temporary holder to prevent bad data from escaping

    cout << "What book are you reading? ";

    cin.get(book_check, MAX_INPUT_SIZE);
    cin.clear();
    cin.ignore(255, '\n');

    cout << "Who wrote it? ";

    cin.get(author_check, MAX_INPUT_SIZE);
    cin.clear();
    cin.ignore(255, '\n');

    cout << "How many pages is it? ";

    cin >> pages_check;

    while(!cin.good() || pages_check <= 0)
    {

        cin.clear();
        cin.ignore(255, '\n');

        cout << "I'm sorry, I'm expecting a positive number" << endl;
        cout << "How many pages is it? ";

        cin >> pages_check;

    }

    book = new char[strlen(book_check) + 1];
    strcpy(book, book_check);
    delete[] book_check;
    author = new char[strlen(author_check) + 1];
    strcpy(author, author_check);
    delete[] author_check;
    pages = pages_check;

    return;

}

//Reset reading to zero equivalent
void reading::clear()
{

    if(book)
    {

        delete[] book;
        book = NULL;

    }

    if(author)
    {

        delete[] author;
        author = NULL;

    }

    pages = 0;

    activity::clear();

    return;

}

//Provide user interface for interacting with reading functions
void reading::menu(activity &activity_a, activity &activity_b)
{

    using std::cout;
    using std::cin;
    using std::endl;

    int input = 7;


    while(input != 0)
    {

        cout << endl << "1.  Display reading" << endl;
        cout << "2.  Set all" << endl;
        cout << "3.  Set new start" << endl;
        cout << "4.  Set new end" << endl;
        cout << "5.  Set new book" << endl;
        cout << "0.  Go back" << endl;
        cout << "Please choose an option number: ";

        cin >> input;

        if(!cin.good() || input < 0 || input > 5)
        {

            cout << endl << "I'm sorry, I don't understand - please try again" << endl;
            input = 7;

        }

        cin.clear();
        cin.ignore(255, '\n');

        switch(input)
        {

            case 1:
                display();
                break;

            case 2:
                change_start(activity_a, activity_b);
                change_end(activity_a, activity_b);
                set();
                break;

            case 3:
                change_start(activity_a, activity_b);
                break;

            case 4:
                change_end(activity_a, activity_b);
                break;

            case 5:
                set();
                break;

        }

    }

    return;

}

//Check to see if reading object has data
bool reading::is_set() const
{

    if(activity::is_set() && book && author && pages > 0)
        return true;

    return false;

}

shopping::shopping():store(NULL),grocery_list(NULL){}

shopping::shopping(const shopping &source):activity(source)
{

    if(source.store)
    {

        store = new char[strlen(source.store) + 1];
        strcpy(store, source.store);

    }else
        store = NULL;

    if(source.grocery_list)
    {

        grocery_list = new char[strlen(source.grocery_list) + 1];
        strcpy(grocery_list, source.grocery_list);

    }else
        grocery_list = NULL;

}

shopping::~shopping()
{

    if(store)
        delete[] store;

    if(grocery_list)
        delete[] grocery_list;

}

void shopping::display() const
{

    using std::cout;
    using std::endl;

    if(!is_set())
    {

        cout << "Shopping is not set" << endl;
        return;

    }

    cout << "You are shopping at: " << store << endl;
    
    if(grocery_list)
    {

        cout << "Your grocery list is:" << endl;
        cout << grocery_list << endl;

    }else
        cout << "You have not created a grocery list" << endl;

    activity::display();

    return;

}

void shopping::set()
{

    using std::cout;
    using std::endl;
    using std::cin;

    char *store_check = new char[MAX_INPUT_SIZE + 1];
    char in_check;

    cout << "What store will you be shopping at? ";

    cin.get(store_check, MAX_INPUT_SIZE);
    cin.clear();
    cin.ignore(255, '\n');

    cout << "Do you wish to add a grocery list? (y/n): ";
    cin >> in_check;
    cin.clear();
    cin.ignore(255, '\n');

    in_check = toupper(in_check);

    while(in_check != 'Y' && in_check != 'N')
    {

        cout << "Invalid input" << endl;
        cout << "Do you wish to add a grocery list? (y/n): ";
        cin >> in_check;
        cin.clear();
        cin.ignore(255, '\n');

        in_check = toupper(in_check);

    }

    store = new char[strlen(store_check) + 1];
    strcpy(store, store_check);
    delete[] store_check;

    if(in_check == 'Y')
        make_list();

    return;

}

void shopping::clear()
{

    if(store)
    {

        delete[] store;
        store = NULL;

    }

    if(grocery_list)
    {

        delete[] grocery_list;
        grocery_list = NULL;

    }

    activity::clear();

    return;

}

void shopping::menu(activity &activity_a, activity &activity_b)
{

    using std::cout;
    using std::cin;
    using std::endl;

    int input = 8;


    while(input != 0)
    {

        cout << endl << "1.  Display reading" << endl;
        cout << "2.  Set all" << endl;
        cout << "3.  Set new start" << endl;
        cout << "4.  Set new end" << endl;
        cout << "5.  Add grocery list (replaces existing list)" << endl;
        cout << "6.  Delete grocery list" << endl;
        cout << "0.  Go back" << endl;
        cout << "Please choose an option number: ";

        cin >> input;

        if(!cin.good() || input < 0 || input > 6)
        {

            cout << endl << "I'm sorry, I don't understand - please try again" << endl;
            input = 8;

        }

        cin.clear();
        cin.ignore(255, '\n');

        switch(input)
        {

            case 1:
                display();
                break;

            case 2:
                change_start(activity_a, activity_b);
                change_end(activity_a, activity_b);
                set();
                break;

            case 3:
                change_start(activity_a, activity_b);
                break;

            case 4:
                change_end(activity_a, activity_b);
                break;

            case 5:
                make_list();
                break;

            case 6:
                delete_list();
                break;

        }

    }

    return;

}

void shopping::make_list()
{

    using std::cout;
    using std::endl;
    using std::cin;

    char *grocery_list_check = new char[MAX_INPUT_SIZE + 1];

    cout << "Please enter a grocery list (500 character max): ";

    cin.get(grocery_list_check, MAX_INPUT_SIZE);
    cin.clear();
    cin.ignore(255, '\n');

    grocery_list = new char[strlen(grocery_list_check) + 1];
    strcpy(grocery_list, grocery_list_check);
    delete[] grocery_list_check;
    
    return;
}

void shopping::delete_list()
{

    if(grocery_list)
    {

        delete[] grocery_list;
        grocery_list = NULL;

    }

    return;

}

bool shopping::is_set() const
{

    if(activity::is_set() && store)
        return true;

    return false;

}
