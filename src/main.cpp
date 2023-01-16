#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

#define WILD -1

class cBlock
{
public:
    int x, y, z;
    int count;

    cBlock(int ix, int iy, int iz, int ic)
        : x(ix), y(iy), z(iz), count(ic)
    {
    }
    void text() const
    {
        std::cout << "( " << x << ", " << y << ", " << z << ") * " << count << "\n";
    }

    bool match(cBlock &other);
};

typedef std::vector<cBlock> vBlock;

vBlock vb[8];

bool cBlock::match( cBlock &other)
{
    // check for already matched blocks
    if( count == -1 || other.count == -1 )
        return false;

    if (x != -1 && other.x != -1 && x != other.x)
        return false;
    if (y != -1 && other.y != -1 && y != other.y)
        return false;
    if (z != -1 && other.z != -1 && z != other.z)
        return false;

    std::cout << "\nmatch\n";
    text();
    other.text();

    count += other.count; // sum the occurences

    other.count = -1; // delete the matched block

    // collapse wild values
    if (x == WILD && other.x != WILD)
        x = other.x;
    if (y == WILD && other.y != WILD)
        y = other.y;
    if (z == WILD && other.z != WILD)
        z = other.z;

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

void domatching()
{
    int c1, c2;
    c1 = 1;
    c2 = 6;
    for (auto &b1 : vb[c1])
    {
        for (auto &b2 : vb[c2])
        {
            if( b1.match(b2) )
                {
                    // succesful match
                    // move to 7 occurence container
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
    domatching();
    std::cout << "\nresults:\n";
    display();

    return 0;
}
