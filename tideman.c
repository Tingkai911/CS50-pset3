#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);


        for (int x = 0; x<candidate_count; x++)
        {
            for (int y = 0; y<candidate_count; y++)
            {
               printf("%d, ", preferences[x][y]);
            }
            printf("\n");
        }


        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // DONE
    for (int i = 0; i<candidate_count; i++)
    {
        if(strcmp(name, candidates[i]) == 0)
        {
            // i is the index of the candidate
            // if Rank = 0+1 : candidate 3, ranks[0] = 3
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // DONE
    // preferences[i][j] is number of voters who prefer i over j
    for (int i = 0; i<candidate_count; i++)
    {
        for(int j = i + 1; j<candidate_count; j++)
        {
            // i and j are the rankings itself
            // ranks[i] is index of candidate i
            // ranks[i + j] is the index of candidate j
            preferences[ ranks[i] ][ ranks[j] ] ++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // DONE
    printf("Before Sort:\n");
    for (int i = 0; i<candidate_count; i++)
    {
        for(int j = 0 ; j<candidate_count; j++)
        {
            if(preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count ++;
                printf("winner: %d, loser: %d\n", i, j);
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // DONE
    // bubble sort

    int swap_counter = -1;

    printf("After Sort:\n");

    do
    {
        swap_counter = 0;

        for(int i = 0; i< pair_count; i++)
        {
            int i_term = preferences[ pairs[i].winner ][ pairs[i].loser ] - preferences[ pairs[i].loser ][ pairs[i].winner ];
            int i_next_term = preferences[ pairs[i+1].winner ][ pairs[i+1].loser ] - preferences[ pairs[i+1].loser ][ pairs[i+1].winner ];

            // put the larger term at the front
            if(i_term < i_next_term)
            {
                pair temp = pairs[i+1];
                pairs[i+1] =  pairs[i];
                pairs[i] = temp;
                swap_counter++;
            }
        }

    } while (swap_counter != 0);

    for(int i = 0; i< pair_count; i++)
    {
        printf("winner: %d, loser: %d\n", pairs[i].winner, pairs[i].loser);
    }

    return;
}

bool has_cycle_helper (int index, bool visited[])
{
    // base case
    if (visited[index] == true)
    {
        return true;
    }

    // recursive case
    // explanation: Suppose there is a loop from Alice => Bob => Charlie => Alice, and we start from Alice
    visited[index] = true; // to state that we have visited Alice
    for (int i = 0; i<candidate_count; i++)
    {
        if(locked[index][i] == true) // check if there is a there is a cycle between Alice and Bob
        {                            // if there is a path between Alice and Bob
            if (has_cycle_helper(i, visited)) // we go to Bob and see if there is a path between Bob and Charlie (recursive step)
            {                                 // if there is path between Bob and Charlie, we check if there is path between Charlie and Alice
                return true;                  // finally we return true if there is a loop as the whole visited[] array is true
            }
        }
    }

    return false; // return false if there is no loop
}

bool has_cycle (int starting_index)
{
    bool visited[candidate_count];
    for (int i = 0; i<candidate_count; i++)
    {
        visited[i] = false;
    }
    return has_cycle_helper(starting_index, visited);
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // locked[i][j] means i is locked in over j

    for (int i = 0; i<pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        // checks if there is any close cycle
        if(has_cycle(i) == true)
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    return;
}

bool is_source(int index)
{
    for(int i = 0; i<candidate_count; i++)
    {
        if(locked[i][index] == true)
        {
            return false; // if locked[charlie][alice] == true, then index = alice is not the source of charlie
        }
    }

    return true; // return true if no one is the source of the candidate of this index
}

// to figure out which candidate is the source
int get_source()
{
    for (int i = 0; i<candidate_count; i++)
    {
        if(is_source(i))
        {
            return i;
        }
    }

    return -1; // if there is no source found
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    int index = get_source();
    if(index != -1)
    {
        printf("%s\n", candidates[index]);
    }
    return;
}

