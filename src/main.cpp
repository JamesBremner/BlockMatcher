#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */

// define wild value for x,y,z ( assumes  real values are always +ve )
#define WILD -1

class cBlock;
typedef std::vector<cBlock> vBlock_t;

class cBlock
{
public:
    int x, y, z;
    int count; // occurence count, -1 indicates a merged block to be ignored

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

std::vector<vBlock_t> Container(8);

bool cBlock::match(cBlock &other)
{
    // check for already matched blocks
    if (count == -1 || other.count == -1)
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
/**
 * @brief Match and store identical single occurence blocks
 * 
 * @param vb vector of blocks
 * 
 * The matched blocks, up to a maximum of 7
 * are stored in the containers
 */
void identy(vBlock_t &vb)
{
    for (auto &vb : Container)
        vb.clear();

    std::map<std::tuple<int, int, int>, int> M;
    for (auto &b : vb)
    {
        auto ret = M.insert(std::make_pair(
            std::make_tuple(b.x, b.y, b.z),
            0));
        if (!ret.second)
        {
            if (ret.first->second == 6)
            {
                // match of 7 blocks
                // store in 7th container
                b.count = 7;
                Container[7].push_back(b);

                // reset match to zero occurences
                ret.first->second = 0;
            }
            else
            {
                // match, increment occurence count
                ret.first->second++;
            }
        }
        else
        {
            ret.first->second = 1;
        }
    }

    // store matches of less than 7 in their containers
    for (auto &m : M)
    {
        if (!m.second)
            continue;
        Container[m.second].push_back(cBlock(
            std::get<0>(m.first),
            std::get<1>(m.first),
            std::get<2>(m.first),
            m.second));
    }
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

        Container[c].push_back(cBlock(
            vvinput[k][0], vvinput[k][1], vvinput[k][2], c));
    }
}
void display()
{
    for (int c = 1; c <= 7; c++)
        for (auto &b : Container[c])
        {
            if (b.count != -1)
                b.text();
        }
}

void matchContainers(int c1, int c2)
{
    // loop over blocks in container 1
    for (auto &b1 : Container[c1])
    {
        // loop over blocks in container 2
        for (auto &b2 : Container[c2])
        {
            // atempt match
            if (b1.match(b2))
            {
                // succesful match
                // move match to 7 occurence container
                Container[7].push_back(b1);
                b1.count = -1;
            }
        }
    }
}

void testIdentity()
{
    vBlock_t vb{
        cBlock(0, 0, 0, 1),
        cBlock(1, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),
        cBlock(0, 0, 0, 1),

    };
    identy(vb);
    std::cout << "\nidenty:\n";
    display();
}

main()
{
    createSampleProblem();
    display();

    matchContainers(1, 6);
    matchContainers(2, 5);
    matchContainers(3, 4);

    std::cout << "\nresults:\n";
    display();

    testIdentity();

    return 0;
}
