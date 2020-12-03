/***********************
 *--- Day 3: Toboggan Trajectory ---

With the toboggan login problems resolved, you set off toward the airport. While travel by toboggan might be easy, it's certainly not safe: there's very minimal steering and the area is covered in trees. You'll need to see which angles will take you near the fewest trees.

Due to the local geology, trees in this area only grow on exact integer coordinates in a grid. You make a map (your puzzle input) of the open squares (.) and trees (#) you can see. For example:

..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#

These aren't the only trees, though; due to something you read about once involving arboreal genetics and biome stability, the same pattern repeats to the right many times:

..##.........##.........##.........##.........##.........##.......  --->
#...#...#..#...#...#..#...#...#..#...#...#..#...#...#..#...#...#..
.#....#..#..#....#..#..#....#..#..#....#..#..#....#..#..#....#..#.
..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#
.#...##..#..#...##..#..#...##..#..#...##..#..#...##..#..#...##..#.
..#.##.......#.##.......#.##.......#.##.......#.##.......#.##.....  --->
.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#
.#........#.#........#.#........#.#........#.#........#.#........#
#.##...#...#.##...#...#.##...#...#.##...#...#.##...#...#.##...#...
#...##....##...##....##...##....##...##....##...##....##...##....#
.#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#.#..#...#.#  --->

You start on the open square (.) in the top-left corner and need to reach the bottom (below the bottom-most row on your map).

The toboggan can only follow a few specific slopes (you opted for a cheaper model that prefers rational numbers); start by counting all the trees you would encounter for the slope right 3, down 1:

From your starting position at the top-left, check the position that is right 3 and down 1. Then, check the position that is right 3 and down 1 from there, and so on until you go past the bottom of the map.

The locations you'd check in the above example are marked here with O where there was an open square and X where there was a tree:

..##.........##.........##.........##.........##.........##.......  --->
#..O#...#..#...#...#..#...#...#..#...#...#..#...#...#..#...#...#..
.#....X..#..#....#..#..#....#..#..#....#..#..#....#..#..#....#..#.
..#.#...#O#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#..#.#...#.#
.#...##..#..X...##..#..#...##..#..#...##..#..#...##..#..#...##..#.
..#.##.......#.X#.......#.##.......#.##.......#.##.......#.##.....  --->
.#.#.#....#.#.#.#.O..#.#.#.#....#.#.#.#....#.#.#.#....#.#.#.#....#
.#........#.#........X.#........#.#........#.#........#.#........#
#.##...#...#.##...#...#.X#...#...#.##...#...#.##...#...#.##...#...
#...##....##...##....##...#X....##...##....##...##....##...##....#
.#..#...#.#.#..#...#.#.#..#...X.#.#..#...#.#.#..#...#.#.#..#...#.#  --->

In this example, traversing the map using this slope would cause you to encounter 7 trees.

Starting at the top-left corner of your map and following a slope of right 3 and down 1, how many trees would you encounter?

--- Part Two ---

Time to check the rest of the slopes - you need to minimize the probability of a sudden arboreal stop, after all.

Determine the number of trees you would encounter if, for each of the following slopes, you start at the top-left corner and traverse the map all the way to the bottom:

    Right 1, down 1.
    Right 3, down 1. (This is the slope you already checked.)
    Right 5, down 1.
    Right 7, down 1.
    Right 1, down 2.

In the above example, these slopes would find 2, 7, 3, 4, and 2 tree(s) respectively; multiplied together, these produce the answer 336.

What do you get if you multiply together the number of trees encountered on each of the listed slopes?
******************************************************/

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <string_view>

size_t traverse(char *map, size_t width, size_t length, size_t step_h, size_t step_v)
{

    // Starting at the top left, map through the array.
    size_t pos_x = 0; // 0 == left
    size_t pos_y = 0; // 0 == top

    size_t trees = 0;
    size_t spaces = 0;
    while (pos_y < length)
    {
        // Extract current character
        char cc = *(map + (pos_x) + (width * pos_y));
        std::cout << cc;
        if (cc == '#')
            trees++;
        if (cc == '.')
            spaces++;

        // Move right 3, down 1
        pos_x += step_h;
        pos_x %= width;
        pos_y += step_v;
    }
    std::cout << std::endl
              << "Slope " << step_h << "x" << step_v << " has " << trees << " trees and " << spaces << " spaces" << std::endl;

    return trees;
}

int main(int argc, char **argv)
{
    std::string inFilePath("input.txt");

    if (argc == 2)
    {
        inFilePath = argv[1];
    }
    else if (argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " [input file (default ./input.txt)]" << std::endl;
        return 1;
    }

    std::cout << "Populating string vector from \"" << inFilePath << "\"" << std::endl;

    std::ifstream inFile(inFilePath);
    // Get width
    size_t width = 0;
    while (inFile.get() != '\n')
    {
        ++width;
    }
    // Get length
    inFile.clear();
    inFile.seekg(0, std::ios::beg);
    std::string buf;
    size_t length = 0;
    while (std::getline(inFile, buf))
    {
        ++length;
    }
    inFile.clear();
    inFile.seekg(0, std::ios::beg);

    std::cout << "Map is " << width << " x " << length << std::endl;

    // Make a lovely array
    char treeMap[length][width];

    for (size_t i = 0; i < length; i++)
    {
        std::getline(inFile, buf);
        strncpy(treeMap[i], buf.c_str(), width);
    }

    std::cout << "========== DAY 3 PART 1 =========" << std::endl;
    traverse(reinterpret_cast<char*>(treeMap), width, length, 3, 1);

    std::cout << "========== DAY 3 PART 2 =========" << std::endl;
    size_t product = traverse(reinterpret_cast<char*>(treeMap), width, length, 1, 1);
    product *= traverse(reinterpret_cast<char*>(treeMap), width, length, 3, 1);
    product *= traverse(reinterpret_cast<char*>(treeMap), width, length, 5, 1);
    product *= traverse(reinterpret_cast<char*>(treeMap), width, length, 7, 1);
    product *= traverse(reinterpret_cast<char*>(treeMap), width, length, 1, 2);

    std::cout << "Product of trees: " << product << std::endl;
}
