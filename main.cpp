/*
 * Assignment 10 Bonus
 * Paolo Scattolin s1023775
 * Johan Urban s1024726
 * Answers follow the code.
 * */

#define NDEBUG
#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string.h>

using namespace std;

/*********************************************************************
*   type definitions and global array songs:
**********************************************************************/

struct Length
{
    int minutes;							// #minutes (0..)
    int seconds;							// #seconds (0..59)
};

struct Track
{
    string artist;                          // artist name
    string cd;                              // cd title
    int    year;                            // year of appearance
    int    track_no;						// track number
    string title;                           // track title
    string tags;                            // track tags (separated by ,)
    Length time;							// track length
    string country;                         // countr(y/ies) of artist (separated by ,)
};

double iteration = 0;

const int MAX_NO_OF_SONGS = 6000;           // NOTE: only required for *array*

vector<Track> songs ;

int MAX;

/************************************************************************
*   Ordering relations op Track:
*      define < and == for Length and Track yourself, the other
*      ordering relations (>, <=, >=, >) follow from these automatically.
************************************************************************/
bool operator<(const Length& a, const Length& b)
{
    if (a.minutes == b.minutes)
        return a.seconds < b.seconds;
    else return a.minutes < b.minutes;

}

bool operator==(const Length& a, const Length& b)
{
    return (a.minutes == b.minutes && a.seconds == b.seconds) ;   /* implement a correct == ordering on Track values */
}

bool operator>(const Length& a, const Length& b)
{
    return b < a ;
}

bool operator<=(const Length& a, const Length& b)
{
    return !(b < a) ;
}

bool operator>=(const Length& a, const Length& b)
{
    return b <= a ;
}

bool operator<(const Track& a, const Track& b)
{
    iteration++;
    /*if(a.artist == b.artist)
    {
        if(a.cd == b.cd)
        {
            if(a.year == b.year)
            {
                return a.track_no < b.track_no;
            }else return a.year < b.year;

        }else return a.cd < b.cd;

    }else return a.artist < b.artist ;
    */return a.time < b.time ;
}

bool operator==(const Track& a, const Track& b)
{
    iteration++;

    //return (a.artist == b.artist && a.cd == b.cd && a.year == b.year && a.track_no == b.track_no) ;
    return a.time == b.time;
}

bool operator>(const Track& a, const Track& b)
{
    return b < a ;
}

bool operator<=(const Track& a, const Track& b)
{
    return !(b < a) ;
}

bool operator>=(const Track& a, const Track& b)
{
    return b <= a ;
}


/************************************************************************
*   Reading Tracks.txt file:
************************************************************************/
istream& operator>> (istream& in, Length& length)
{// Pre-condition:
    assert (true) ;
/*  Post-condition:
    a value for length has been obtained from in: first minutes, followed by ':', followed by seconds.
*/
    char colon ;
    in >> length.minutes >> colon >> length.seconds ;
    return in ;
}

ostream& operator<< (ostream& out, const Length length)
{// Pre-condition:
    assert (true) ;
/*  Post-condition:
    length is sent to out as: minutes, ':', seconds (at least 2 digits)
*/
    out << length.minutes << ':';
    if (length.seconds < 10)
        out << '0';
    out << length.seconds;
    return out ;
}

void skip_to_next_newline (istream& infile)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    infile has been read up to and including the next newline character.
*/
    string remains ;
    getline (infile, remains) ;
}

istream& operator>> (istream& infile, Track& track)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    infile has been read up to and including all members of one Track entry, including the last separating newline.
    These members are stored in track.
*/
    getline(infile,track.artist);       // artist
    getline(infile,track.cd);           // cd
    infile >> track.year ;              // year
    skip_to_next_newline(infile) ;
    infile >> track.track_no ;          // track number
    skip_to_next_newline(infile) ;
    getline(infile,track.title);        // track title
    getline(infile,track.tags);         // track tags
    infile >> track.time ;              // track time
    skip_to_next_newline(infile) ;
    getline(infile,track.country);      // country
    skip_to_next_newline(infile) ;
    return infile ;
}

// not necessary for assignment (because of show_track function), but included anyway because of symmetry:
ostream& operator<< (ostream& out, const Track track)
{// pre-condition:
    assert (true) ;
/*  post-condition:
    out has emitted all members of track, including the last separating newline.
*/
    out << track.artist   << endl ;
    out << track.cd       << endl ;
    out << track.year     << endl ;
    out << track.track_no << endl ;
    out << track.title    << endl ;
    out << track.tags     << endl ;
    out << track.time     << endl ;
    out << track.country  << endl ;
    out << endl ;
    return out ;
}

int read_songs (ifstream& infile, vector <Track>& songs)
{// pre-condition:
    assert (infile.is_open ()) ;
/*  post-condition:
    All tracks in infile have been read and stored in the same order in songs.
    The result is the number of tracks that have been read.
*/
    int i = 0 ;
    do {
        Track song ;
        infile >> song ;
        if (infile)
        {
            songs.push_back(song);
            i++;
        }
    }
    while (infile && i < MAX_NO_OF_SONGS);

    return i ;
}

void swap (vector<Track>& songs, int first,int second)
{
    //preconditions:
    assert(first >= 0 && first < MAX);
    assert(second >= 0 && second < MAX);
    //postconditions: this function swaps the 2 elements of the array/vector.

    Track temp;

    temp = songs[first];
    songs[first] = songs[second];
    songs[second] = temp;
}

void push_up(vector<Track>& songs, int i)
{
    //preconditions:
    assert(i >= 0 && i < MAX);
    //postcondition: the elements is pushed to its highest position.

    while (songs[i] > songs[(i-1)/2])
    {
        swap (songs, i , (i-1)/2);
        i = (i-1)/2;
    }
}

void build_heap(vector<Track>& songs)
{
    //preconditions:
    assert(true);
    //postconditions: the function calls push_up for each el. of the array.

    for (int i = 1 ; i < MAX ; i ++)
    {
        push_up(songs,i);
    }
}

void push_down(vector<Track>& songs, int MAX_EL)
{
    // precondition:
    assert(MAX_EL >= 0);
    // postcondition:
    // A value is placed more and more toward the end of the vector being sorted,
    // until it is no longer larger than one of its children.
    while (MAX_EL > 0)
    {
        int i = 0;
        while (2*i+1 < MAX_EL)
        {
            if(2*i+2 < MAX_EL && (songs[2*i+1] >= songs[2*i+2]))
            {
                if (songs[i] <= songs[2*i+1])
                {
                    swap (songs, i , 2*i + 1);
                    i = 2*i + 1;
                }
            }
            else if (2*i+2 < MAX_EL && (songs[2*i+1] < songs[2*i+2]))
            {
                if (songs[i] <= songs[2*i+2])
                {
                    swap (songs, i , 2*i + 2);
                    i = 2*i + 2;
                }
            }
            else if (songs[i] <= songs[2*i+1])
            {
                swap (songs, i , 2*i + 1);
                i = 2*i + 1;
            }
            i++;
        }
        /*
             * for (int i = 0 ; i < MAX_EL; i++)
            {
                cout << songs[i] << " ";
            }
            cout << endl ;
         */

        MAX_EL--;
    }
}

void pick_heap(vector<Track>& songs)
{
    // precondition:
    assert(true);
    //postcondition:
    //the vector is being sorted by a cooperation of two implemented functions:
    // swap() and push_down().
    int MAX_EL = MAX;

    for (int i = 0; i < MAX ; i++)
    {
        swap (songs ,0, MAX_EL-1);
        /*cout<<"swapped: "<<endl;
        for (int j = 0 ; j< MAX; j++)
        {
            cout << songs[j] << " ";
        }
        cout << endl ; */
        MAX_EL--;
        push_down(songs ,MAX_EL);
    }
}

int read_file (string filename)
{// pre-condition:
    assert (songs.empty()) ;
/*  post-condition:
    If the result is -1, then no file could be opened, and songs is unchanged.
    Otherwise, the result is zero or positive, and songs contains all the found tracks in the
    file that corresponds with filename.
*/
    ifstream songsDBS (filename.c_str());
    if (!songsDBS)
    {
        cout << "Could not open '" << filename << "'." << endl;
        return -1;
    }
    cout << "Reading '" << filename << "'." << endl;
    const int NO_OF_SONGS = read_songs (songsDBS, songs);
    songsDBS.close();
    cout << "Read " << NO_OF_SONGS << " tracks." << endl;
    return NO_OF_SONGS;
}

// NOTE: this version uses *array* implementation, convert to vector yourself
void show_all_tracks (vector <Track> songs, int no_of_songs)
{// pre-condition:
    assert (no_of_songs < MAX_NO_OF_SONGS) ;
/*  post-condition:
    songs[0] ... songs[no_of_songs-1] have been displayed to cout.
*/
    for (int i = 0 ; i < no_of_songs ; i++)
    {
        cout << i+1 <<". "<< songs[i] << endl ;
    }
}

/************************************************************************
*   functions and data structures from lecture #9 IPC031:
************************************************************************/



void print (int index)
{
    //Precondition:
    assert(true);
    //Postcondition: Function calculates how many asterix-chars need to be printed and prints them,
    // followed by a dot for the remainder
    int stars = iteration/100000;
    cout << "After " << index << " element(s) sorted:";

    while (stars > 0 )
    {
        cout <<"*";
        stars--;
    }
    cout<<"."<< endl;
}


/************************************************************************
*   the main structure of the program:
*       - read file
*       - sort data with
*              and 'normal' order of Tracks
*              and increasing track length
************************************************************************/

int main()
{
    // Precondition:
    assert(true);
    // Postcondition: the contents of Tracks.txt will be sorted using heap sort.
    // Output is in the console.
    const int NO_OF_SONGS = read_file ("Tracks.txt");
    if (NO_OF_SONGS < 0)
    {
        cout << "Reading file failed. Program terminates." << endl;
        return NO_OF_SONGS;
    }

    MAX = songs.size();

    build_heap(songs);

    for (int i = 99; i  <= NO_OF_SONGS; i += 100)
    {
        //cout << "Tracks sorted." << endl;

        //show_all_tracks (songs,NO_OF_SONGS) ;
        cout << iteration << endl;
        print(i);
        iteration = 0 ;
        if (i == 5799)
            i=NO_OF_SONGS-100;
    }

    /*cout << "Heaped vector: " << endl;

    for (int i = 0; i < MAX; i++) {
        cout << songs[i] << " ";
    }
    cout << endl;
*/
    pick_heap(songs);

    out << "Sorted vector: " << endl;

    for (int i = 0; i < MAX; i++) {
        cout << songs[i] << " " << endl;
    }
    cout << iteration;
    cout << endl;

    return 0;
}

/* *
 * PART 2 - Counting comparisons
 *  Comparison by track: The algorithm does not finish within a reasonable time, hence no viable data is available.
 *  There seems to be an overflow of the integer type 'iteration' that is used to count the number of comparisons.
 *  Comparison by time: This time the algorithm finishes, although much later than the algorithms from the previous
 *  exercises. After changing the int type to a double type the counter displays: 6.28815e+09
 */
