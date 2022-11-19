/*
 * Sawyer Norquist
 * CS202
 * Program 1
 * activities.h
 * 7/8/2020
 *
 * This code governs time of day and individual activities.  The time class manages time as a 24 hour clock, reading in from the user,
 * displaying properly formatted output, and comparing to different time class objects.  The activity base class simply holds two time
 * class objects, allowing for various comparison functions to occur between them.  The three derived classes add basic functionality
 * to the activity class.  work adds employer name and your hourly wages, and will output total value of the shift if requested.  reading
 * holds essential book information.  shopping provides space and functions to work with the intended store and grocery list.
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>

static const int WORK_MIN = 360;
static const int WORK_MAX = 480;
static const int MAX_INPUT_SIZE = 500;

class time_clock
{

    public:

        time_clock();
        time_clock(const time_clock &source);
        int compare(const time_clock &comparator) const;  //outputs difference of time in minutes, positive indicates input is after, negative before
        void set();  //gather user input - no output prompt, relies on calling class
        void set(const time_clock &source);  //set time to source
        void display() const;  //couts time in proper format
        bool is_set() const; //true if int has a valid value, false otherwise
        void clear();  //Reset clock to zero equivalent

    private:

        int clock;
        int convert_int(char source) const;

};


class activity
{

    private:

        const int max;
        const int min;
        time_clock start;
        time_clock end;

    public:

        activity();
        activity(int min_input, int max_input);
        activity(const activity &source);
        void display() const;  //displays activity start and end times
        int time_until(const time_clock &comparator) const;  //returns minutes leading up to beginning of activity, if negative it's after start
        int time_after(const time_clock &comparator) const;  //returns minutes after end of activity, if negative it's before end
        int duration() const;  //returns duration of activity in minutes
        bool is_during(const time_clock &comparator) const;  //returns true of comparator is during activity
        bool is_set() const;  //returns true if both clocks in the activity are set
        void clear();  //resets clock to zero equivalent
        bool change_start(const time_clock &new_time, activity &activity_a, activity &activity_b);  //set start to new time, checks to make sure it's not after end, returns false if illegal
        void change_start(activity activity_a, activity activity_b);  //Contains logic to initiate the time_clock object to feed into sister function
        bool change_end(const time_clock &new_time, activity &activity_a, activity &activity_b);  //set end to new time, checks to make sure it's not before beginning, returns false if illegal
        void change_end(activity activity_a, activity activity_b);  //Contains logic to initiate the time_clock object to feed into sister function

};

class work: public activity
{

    private:

        char* employer;
        int wages;

    public:

        work();
        work(const work &source);
        ~work();
        void display() const;  //display activity info
        void set();  //set activity information
        void clear();  //reset activity to zero equivalent
        void menu(activity &activity_a, activity &activity_b);  //user interace for working with basic functionality
        int income() const;  //returns total amount earned during this activity
        bool is_set() const;  //returns true if all activity values have data, though wages can remain zero

};

class reading: public activity
{

    private:

        char* book;
        char* author;
        int pages;

    public:

        reading();
        reading(const reading &source);
        ~reading();
        void display() const;  //display activity info
        void set();  //set activity information
        void clear();  //reset activity to zero equivalent
        void menu(activity &activity_a, activity &activity_b);  //user interface for working with basic functionality
        bool is_set() const;  //Returns true if all activity values have data

};

class shopping: public activity
{

    private:

        char* store;
        char* grocery_list;

    public:

        shopping();
        shopping(const shopping &source);
        ~shopping();
        void display() const;  //display activity info
        void set();  //set activity information
        void clear();  //reset actvity to zero equivalent
        void menu(activity &activity_a, activity &activity_b);  //user interface for working with basic functionality
        void make_list();  //create shopping list
        void delete_list();  //delete shopping list
        bool is_set() const;  //returns true if all activity values have data, list can be empty

};
