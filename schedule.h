/*
 * Sawyer Norquist
 * CS202
 * Program 1
 * schedule.h
 * 7/8/2020
 *
 * This header contains prototypes for the day, day node, and schedule classes.  The day class contains all three activity data members,
 * as well as functions useful for working in them - largely built around a single user interface.  The day node is simply a derived function
 * to allow day to be in a doubly-linked list.  Schedule acts as both the list and primary interface for working with the list of day nodes.
 * Altogether this acts as a program to manage three activities per day in a list of days.
 *
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "activities.h"

class day
{

    public:

        day();
        day(const day &source);
        void display() const;  //Display data from all contained activity classes
        void set();  //Sets all activities, not used
        void clear();  //Resets all activities to zero equivalent
        void menu();  //User interface for working with a day

    private:

        work work_a;
        reading reading_a;
        shopping shopping_a;

};

class day_node: public day
{

    public:

        day_node();
        day_node(const day_node &source);
        day_node(int new_index);
        day_node *&get_next();
        day_node *&get_previous();
        void set_next(day_node *set_node);
        void set_previous(day_node *set_node);
        void set_index(int new_index);  //Node wrapper also uses an index to help the user keep track of which day the user is on, maintained by the calling class
        int &get_index();  //Fetches index

    private:

        day_node *next;
        day_node *previous;
        int index;

};

class schedule
{

    public:

        schedule();
        schedule(int starting_schedules);
        schedule(const schedule &source);
        ~schedule();
        void display_schedule();  //Displays data from all days on current array index - not used
        void new_day();  //Adds a new day after current pointer
        void delete_day();  //Removed day at current pointer
        void next_day();  //Navigate current forward
        void previous_day();  //Navigate current back
        void next_schedule();  //Navigate forward one on schedule index
        void previous_schedule();  //Navigate back one on schedule index
        void menu();  //User interface for navigating the high level schedule

    private:
        day_node **head;
        day_node *current;
        int index;
        const int array_size;
        void copy_schedule(day_node **source, day_node **&new_copy, int current_size) const;  //recursion function to assist copy constructor, navigates the array
        void copy_schedule_rec(day_node *source, day_node *&last_node) const;  //recursive function to assist copy constructor, navigates nodes
        void delete_schedule(day_node *to_delete[], int current_size);  //Helps destructor, recursive function to navigate array
        void delete_schedule_rec(day_node *to_delete);  //Helps destructor, recursive function to navigate nodes
        void display_schedule(day_node *current);  //Recursive function to help the main display function
        void fix_index(day_node *current, int new_index);  //Function to help insert/delete day maintain day indexes

};
