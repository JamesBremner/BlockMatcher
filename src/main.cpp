#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

// define wild value for x,y,z ( assumes  real vales are always +ve )
#define WILD -1

class cBlock
{
public:
    int x, y, z;
    int count;      // occurence count, -1 indicates a merged block to be ignored

    cBlock(int ix, int iy, int iz, int ic)
        : x(ix), y(iy), z(iz), count(ic)
    {
    }
    void text() const
    {
        std::cout << "( " << x << ", " << y << ", " << z << ") * " << count << "\n";
    }

    /* @brief try matching with another block
       @param other block
       @return true if merge successful

       On merge the other block will be marked deleted
       and the attributes of this block updated to the merged block
    */

    bool match(cBlock &other);
};

/// @brief Block containers organized by occurence count
typedef std::vector<cBlock> vBlock;
vBlock vb[8];

bool cBlock::match( cBlock &other)
{
    // check for already matched blocks
    if( count == -1 || other.count == -1 )
        return false;

    // check if merge is possible
    if (x != WILD && other.x != WILD && x != other.x)
        return false;
    if (y != WILD && other.y != WILD && y != other.y)
        return false;
    if (z != WILD && other.z != WILD && z != other.z)
        return false;

    // debug display of matching blocks
    std::cout << "\nmatch\n";
    text();
    other.text();

    // do the merge

    count += other.count; // sum the occurences

    other.count = -1; // delete the matched block

    // collapse wild values
    if (x == WILD && other.x != WILD)
        x = other.x;
    if (y == WILD && other.y != WILD)
        y = other.y;
    if (z == WILD && other.z != WILD)
        z = other.z;

    // debug display of merge results
    std::cout << " => ";
    text();

    return true;
}

void createSampleProblem()
{
    /*
    (1, 5, n) * occurance
    (2, 5, 2) * occurance
    (2, 5, n) * occurance
    (1, n, n) * occurance
    (3, 1, 4) * occurance
    (4, 0, 0) * occurance
    (n, 2, n) * occurance
    (n, n, n) * occurance*/

    std::vector<std::vector<int>> vvinput{
        {1, 5, -1},
        {2, 5, 2},
        {2, 5, -1},
        {1, -1, -1},
        {3, 1, 4},
        {4, 0, 0},
        {-1, 2, -1},
        {-1, -1, -1}};

    /* initialize random seed: */
    srand(time(NULL));

    for (int k = 0; k < vvinput.size(); k++)
    {
        // int c = rand() % 6 + 1;

        int c = 1;
        if (k % 2)
            c = 6;

        vb[c].push_back(cBlock(
            vvinput[k][0], vvinput[k][1], vvinput[k][2], c));
    }
}
void display()
{
    for (int c = 1; c <= 7; c++)
        for (auto &b : vb[c]) {
            if( b.count != -1 )
                b.text();
        }
}

void matchContainers( int c1, int c2 )
{
    // loop over blocks in container 1
    for (auto &b1 : vb[c1])
    {
        // loop over blocks in container 2
        for (auto &b2 : vb[c2])
        {
            // atempt match
            if( b1.match(b2) )
                {
                    // succesful match
                    // move match to 7 occurence container
                    vb[7].push_back( b1 );
                    b1.count = -1;
                }
        }
    }
}

main()
{
    createSampleProblem();
    display();

    matchContainers( 1, 6 );
    matchContainers( 2, 5 );
    matchContainers( 3, 4 );

    std::cout << "\nresults:\n";
    display();

    return 0;
}
