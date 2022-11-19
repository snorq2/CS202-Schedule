/*
 * Sawyer Norquist
 * CS202
 * Program 1
 * schedule.cpp
 * 7/8/2020
 *
 * Implimentation for classes found in schedule.h
 *
 */

#include "schedule.h"

day::day(){}

day::day(const day &source):work_a(source.work_a), reading_a(source.reading_a), shopping_a(source.shopping_a){}

//Iterate through all contained activity display functions
void day::display() const
{

    using std::cout;
    using std::endl;

    work_a.display();
    cout << endl;
    reading_a.display();
    cout << endl;
    shopping_a.display();

    return;

}

//clear all contained activity functions
void day::clear()
{

    work_a.clear();
    reading_a.clear();
    shopping_a.clear();

}

//user interface to reach activities
void day::menu()
{

    using std::cout;
    using std::cin;
    using std::endl;

    int input = 7;  //Holds user input for identifying switch case

    while(input != 0)
    {

        cout << "Working with Day" << endl << endl;
        cout << "1. Display day" << endl;
        cout << "2. Work" << endl;
        cout << "3. Reading" << endl;
        cout << "4. Shopping" << endl;
        cout << "5. Reset day" << endl;
        cout << "0. Go back" << endl;
        cout << "Please choose option number: ";

        cin >> input;

        if(!cin.good() || input < 0 || input > 5)
        {

            cout << "I'm sorry, I don't understand" << endl << endl;
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
                work_a.menu(reading_a, shopping_a);
                break;

            case 3:
                reading_a.menu(work_a, shopping_a);
                break;

            case 4:
                shopping_a.menu(work_a, reading_a);
                break;

            case 5:
                clear();
                break;

        }

    }

    return;

}

day_node::day_node():next(NULL), previous(NULL), index(0){}

day_node::day_node(const day_node &source):day(source),next(source.next), previous(source.previous), index(source.index){}

day_node::day_node(int new_index): next(NULL), previous(NULL), index(new_index){}

//Returns next pointer
day_node *&day_node::get_next()
{

    return next;

}

//Returns previous pointer
day_node *&day_node::get_previous()
{

    return previous;

}

//sets next pointer
void day_node::set_next(day_node *set_node)
{

    next = set_node;

    return;

}

//sets previous pointer
void day_node::set_previous(day_node *set_node)
{

    previous = set_node;

    return;

}

//fetch day index
int &day_node::get_index()
{

    return index;

}

//sets day index
void day_node::set_index(int new_index)
{

    index = new_index;
    return;

}

//Default constructor, sets up two schedules by default
schedule::schedule():array_size(2)
{

    head = new day_node*[array_size];

    for(int i = array_size - 1; i >= 0; --i)
    {

        head[i] = new day_node;

    }

    current = head[0];

    index = 0;

}

//Constructor to allow a custom number of starting schedules to be created
schedule::schedule(int starting_schedules):array_size(starting_schedules)
{

    head = new day_node*[starting_schedules];

    for(int i = starting_schedules - 1; i >= 0; --i)
    {

        head[i] = new day_node;

    }

    current = head[0];

    index = 0;

}

//copy constructor
schedule::schedule(const schedule &source):current(source.current), index(source.index), array_size(source.array_size)
{

    index = source.index;

    head = new day_node*[source.array_size];

    copy_schedule(source.head, head, array_size);

}

schedule::~schedule()
{

    delete_schedule(head, array_size);

    delete[] head;

    head = NULL;

}

//Copy function to navigate array portion
//First argument is source, second is destination.  Last argument is used to make sure array does not go out of bounds
void schedule::copy_schedule(day_node **source, day_node **&new_copy, int current_size) const
{

    //base case
    if(current_size == 0)
        return;

    //If not base but head is null, call next recursion
    if(*head == NULL)
    {

        *new_copy = NULL;
        copy_schedule(source+1, ++new_copy, current_size-1);
        return;

    }

    //if we're here, we've got a node to copy
    //Kicks off first node then sends to recursive copy function for nodes
    day_node *new_node = new day_node(**source);

    new_node->set_next(NULL);
    new_node->set_previous(NULL);

    *new_copy = new_node;

    copy_schedule_rec((*source)->get_next(), *new_copy);
    copy_schedule(source+1, ++new_copy, current_size-1);

    return;

}

//Copy function to navigate list of nodes
//First argument is source, second destination
void schedule::copy_schedule_rec(day_node *source, day_node *&last_node) const
{

    //base case - nothing more to copy
    if(!source)
        return;

    //make a new node
    day_node *new_node = new day_node(*source);

    new_node->set_next(NULL);
    new_node->set_previous(last_node);
    last_node->set_next(new_node);

    copy_schedule_rec(source->get_next(), new_node);

    return;

}

//Recursive delete function to navigate array
//First argument is array to delete, second is indexer to make sure we don't go out of bounds
void schedule::delete_schedule(day_node *to_delete[], int current_size)
{

    //base case
    if(current_size == 0)
        return;

    //Kick off node delete function and next recursion of this function
    delete_schedule_rec(*to_delete);
    delete_schedule(to_delete+1, current_size-1);

    return;

}

//Recursive function to navigate list of nodes
//Argument is the node to be deleted
void schedule::delete_schedule_rec(day_node *to_delete)
{

    //base case
    if(!to_delete)
        return;

    //call next recursion
    delete_schedule_rec(to_delete->get_next());

    //Delete as stack unwinds
    delete to_delete;
    to_delete = NULL;

    return;
    
}

//Displays all days on schedule
//Not used
void schedule::display_schedule()
{

    using std::cout;
    using std::endl;

    //If head on current array index is null, message user and quit function
    if(!head[index])
    {

        cout << "No days on current schedule";
        return;

    }

    //kick off recursive function
    display_schedule(head[index]);

    return;

}

//recursive display function kicked off by previous function
//not used
void schedule::display_schedule(day_node *current)
{

    if(!current)
        return;

    current->display();

    display_schedule(current->get_next());

    return;

}

//Insert function
//Adds day after current, and navigates current forward
void schedule::new_day()
{

    //If head is empty, just add a new day at head and set current
    if(!current && !head[index])
    {

        current = new day_node;
        head[index] = current;

        return;

    }

    //Otherwise make a new node and insert it after current
    day_node *new_node = new day_node(current->get_index() + 1);

    //Make sure you're not dereferencing a null pointer!
    //Don't need to set the new node's pointers to null as that's taken care of by the node constructor
    if(current->get_next())
    {

        new_node->set_next(current->get_next());
        new_node->get_next()->set_previous(new_node);

    }

    new_node->set_previous(current);
    current->set_next(new_node);

    current = new_node;

    //gotta fix the index numbers of the rest of the days on the list
    fix_index(current, current->get_index());

    return;

}

//delete day at current
void schedule::delete_day()
{

 
    //If current is null, nothing to do
    if(current == NULL)
        return;

    //if current is head, need to set head to next pointer
    if(current == head[index])
    {

        head[index] = current->get_next();
        delete current;
        current = head[index];

        //If there was only one day, current would be null - need to block dereference in that case
        if(current)
            current->set_previous(NULL);

        return;

    }

    //if end of list, just need to do a delete and set to null
    if(!current->get_next())
    {

        current = current->get_previous();
        delete current->get_next();
        current->set_next(NULL);

        return;

    }

    //If we're here, we need to execute code to delete a day and reconnect the neighboring days
    day_node *to_delete = current;

    current->get_previous()->set_next(current->get_next());
    current->get_next()->set_previous(current->get_previous());
    current = current->get_next();
    delete to_delete;

    fix_index(current, current->get_index());

    return;

}

//Navigate to next day, prompt user for new day if one does not exist
void schedule::next_day()
{

    using std::cout;
    using std::cin;
    using std::endl;

    char input = 'a';  //Get input from the user

    //Checks if at end of list, or if no days exist to prevent dereferencing a null pointer
    if(!current || !current->get_next())
    {

        cout << endl << "You are already at the end of the list - do you want to add a new day? (y/n): ";
        cin >> input;

        cin.clear();
        cin.ignore(255, '\n');

        input = toupper(input);

        while(input != 'Y' && input != 'N')
        {

            cout << "I'm sorry, I don't understand" << endl;
            cout << "You are already at the end of the list - do you want to add a new day? (y/n): ";
            cin >> input;
            
            cin.clear();
            cin.ignore(255, '\n');

        }

        if(input == 'N')
            return;

        new_day();

        return;

    }

    current = current->get_next();

    return;

}

//Navigate to previous day
//No new day will be made at beginning of list
void schedule::previous_day()
{

    using std::cout;
    using std::endl;

    //If no days exist, don't dereference a null pointer!
    if(!current)
    {

        cout << endl << "No days exist" << endl;
        return;

    }

    if(!current->get_previous())
    {

        cout << endl << "No previous day exists" << endl;
        return;

    }

    current = current->get_previous();

    return;

}

//Move one forward on the array index
//Alert user if at end of array
void schedule::next_schedule()
{

    using std::cout;
    using std::endl;

    if(index + 1 > array_size - 1)
    {

        cout << endl << "Next schedule does not exist" << endl;
        return;

    }

    index = index + 1;

    current = head[index];

    return;

}

//Move one back on the array index
//Alert user if at beginning of array
void schedule::previous_schedule()
{

    using std::cout;
    using std::endl;

    if(index - 1 < 0)
    {

        cout << endl << "Previous schedule does not exist" << endl;
        return;

    }

    index = index - 1;

    current = head[index];

    return;

}

//User interface for working with days and schedules
void schedule::menu()
{

    using std::cout;
    using std::cin;
    using std::endl;

    int input = 7;

    while(input != 0)
    {

        cout << "Schedule #" << index << endl;
        
        if(!current)
            cout << "No day exist - please add a new one before viewing or editing" << endl << endl;
        else
            cout << "Day " << current->get_index() << endl << endl;
        
        cout << "1.  View day" << endl;
        cout << "2.  Next day" << endl;
        cout << "3.  Previous day" << endl;
        cout << "4.  New day" << endl;
        cout << "5.  Next schedule" << endl;
        cout << "6.  Previous schedule" << endl;
        cout << "0.  Quit" << endl;
        cout << "Choose option number: ";

        cin >> input;

        if(!cin.good() || input < 0 || input > 6)
        {

            cout << "I'm sorry, please try again" << endl << endl << endl;
            input = 7;

        }

        cin.clear();
        cin.ignore(255 , '\n');

        switch(input)
        {

            case 1:
                if(!current)
                    cout << "No day exists" << endl << endl;
                else
                    current->menu();

                break;
            
            case 2:
                next_day();
                break;

            case 3:
                previous_day();
                break;

            case 4:
                new_day();
                break;

            case 5:
                next_schedule();
                break;

            case 6:
                previous_schedule();
                break;

        }

    }

    return;

}

//Helper to keep day indexes straight
//First argument is the first node to fix (usually starts in middle of list)
//Second argument is the new index number for the first argument
void schedule::fix_index(day_node *current, int new_index)
{

    //base case
    if(!current)
        return;

    //New index!
    current->set_index(new_index);

    //Start next recursion and increment the index number
    fix_index(current->get_next(), new_index + 1);

    return;

}
