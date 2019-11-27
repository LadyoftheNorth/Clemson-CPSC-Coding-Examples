/*
* This program was created in Dr. Brian Dean's Algorithms class (CPSC 2120)
* 
* Candy.cpp uses the greedy, iterative refinement, and pruned exhaustive sorting
* algorithms to evenly sort candy into 4 bags, also known as the multiple knapsack 
* problem, by their weight and tasty value using the file candy.txt. The program
* utilizes the weight values to ensure all the bags have an even amount of candy 
* in them and then uses the ratio values to sort the candy by their tastiness.
*
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <algorithm>
using namespace std;

struct Candy
{
   int weight;
   int tasty_num;
   double ratio;
   int bag_num;// 0 is bag[3], 1 is bag[0], 2 is bag[1], 3 is bag[2]
   Candy() {weight = 0; tasty_num = 0; ratio = 0; bag_num = 0;}
   Candy(int w, int t) {weight = w, tasty_num = t; ratio = t / w;}
};

struct Bag
{
   int bag_weight;
   int curr_weight;
   int total_tasty;
   Bag() {bag_weight = 2000; curr_weight = 0; total_tasty = 0;}
};

const int N = 16;

Bag *bags = new Bag[4];
Candy *input = new Candy[N];

//reads the input into an array of candy structs
void read_in(int x, int y, int i)
{
    input[i].weight = x;
    input[i].tasty_num = y;
    input[i].ratio = ((double) y/ (double)x);
}

//resets the weight, tasty, and bag_num values in bags and candy
void reset(Bag *bags, Candy *candy)
{
    for(int i = 0; i < 4; i++)
    {
       bags[i].curr_weight = 0;
       bags[i].total_tasty = 0;
    }
    for(int i = 0; i < N; i++)
	candy[i].bag_num = 0;
}

//returns true if cand1 ratio > cand 2 ratio, false otherwise
//helps sort from best to worst tastiness
bool DescCompare(Candy cand1, Candy cand2)
{
   if(cand1.ratio > cand2.ratio)
        return true;
   else
        return false;
}

//returns true if the cand1 ratio < cand 2 ratio, false otherwise
//helps sort from worst to best tastiness
bool AscCompare(Candy cand1, Candy cand2)
{
   if(cand1.ratio < cand2.ratio)
        return true;
   else
        return false;
}

//greedily adds the candies into bags 1, 2, or 3
void add_to_bag(Candy *candy, Bag *bags)
{
   sort(candy, candy + N, DescCompare);

   for(int i = 0; i < N; i++)
   {
     if(bags[0].curr_weight + candy[i].weight <= bags[0].bag_weight && candy[i].bag_num == 0)
     {
        bags[0].curr_weight += candy[i].weight;
        bags[0].total_tasty += candy[i].tasty_num;
	
	candy[i].bag_num = 1;
     }
     else if(bags[1].curr_weight + candy[i].weight <= bags[1].bag_weight && candy[i].bag_num == 0)
     {
        bags[1].curr_weight += candy[i].weight;
        bags[1].total_tasty += candy[i].tasty_num;

	candy[i].bag_num = 2;
     }
     else if(bags[2].curr_weight + candy[i].weight <= bags[2].bag_weight && candy[i].bag_num == 0) 
     {
        bags[2].curr_weight += candy[i].weight;
        bags[2].total_tasty += candy[i].tasty_num;
	
	candy[i].bag_num = 3;
     }
   }
}

//Fixes the bag weight if a bag is overfilled with candy
void fix_weight(Candy *candy, Bag *bags)
{
   sort(candy, candy + N, AscCompare);

   for(int i = 0; i < 3; i++)
   {
     for(int j = 0; j < N; j++)
     {
       if(bags[i].curr_weight > bags[i].bag_weight && candy[j].bag_num == i + 1)
       {
	  candy[j].bag_num = 0;
	  bags[i].curr_weight -= candy[j].weight;
	  bags[i].total_tasty -= candy[j].tasty_num;
       }
     }
   }
} 

//swaps two candies and puts candy 1 into bag 2 and candy 2 into bag 1
void scramble(Candy *candy1, Candy *candy2)
{
   bags[candy1->bag_num - 1].curr_weight -= candy1->weight;
   bags[candy1->bag_num - 1].total_tasty -= candy1->tasty_num;

   bags[candy2->bag_num - 1].curr_weight -= candy2->weight;
   bags[candy2->bag_num - 1].total_tasty -= candy2->tasty_num;

   bags[candy2->bag_num - 1].curr_weight += candy1->weight;
   bags[candy2->bag_num - 1].total_tasty += candy1->tasty_num;
   int temp = candy1->bag_num;
   candy1->bag_num = candy2->bag_num;

   bags[temp - 1].curr_weight += candy2->weight;
   bags[temp - 1].total_tasty += candy2->tasty_num;
   candy2->bag_num = temp;
}

int best_pruned = 0;
int curr_total = 0;

//keep track of the best tasty solution out of all of the recursive tries
void exhaustive(Candy *candy, Bag *bag, int c_index)
{
    if(bag[0].curr_weight > 2000 || bag[1].curr_weight > 2000 || 
	bag[2].curr_weight > 2000)
            return;

    if(c_index == N)
    {
	curr_total = bag[0].total_tasty + bag[1].total_tasty + bag[2].total_tasty;
        if(curr_total > best_pruned) best_pruned = curr_total;

        return;
    }

    for(int i = 0; i < 4; i++)
    {
	candy[c_index].bag_num = i;
	bag[i].curr_weight += candy[c_index].weight;
        bag[i].total_tasty += candy[c_index].tasty_num;

	exhaustive(candy, bag, c_index + 1);

	candy[c_index].bag_num = 0;
	bag[i].curr_weight -= candy[c_index].weight;
	bag[i].total_tasty -= candy[c_index].tasty_num;
     }
}

int main(void)
{
  ifstream pgin;
  int weight, tasty;
  int count = 0;
  int total_tastiness = 0;

  pgin.open("candy.txt");

  while(pgin >> weight >> tasty)
  {
     read_in(weight, tasty, count);
     count++;
  }
  
  pgin.close();

  add_to_bag(input, bags);

  total_tastiness = bags[0].total_tasty + bags[1].total_tasty + bags[2].total_tasty;

  cout << "Greedy: " << total_tastiness << "\n";

  int best_overall = 0;
  int best_tasty = 0;
  int curr_tasty = 0;
 
  Candy *temp1 = new Candy[N]; //the copy of the array before swapping candies
  Candy *temp2 = new Candy[N]; //the copy of the array after swapping candies
  Candy *iter = new Candy[N];

  iter = temp1;
  for(int t = 0; t < 1000; t++)
  { 
    for(int j = 1; j < N; j++)
       swap(input[j], input[rand() % j]);

    bool rebagged = true;

    temp1 = input;

    while(rebagged)
    {
       rebagged = false;
       for(int i = 0; i < N; i++)
       {
	for(int j = i; j < N; j++)
	{
	  add_to_bag(iter, bags);

	  scramble(&iter[i], &iter[j]);

	  fix_weight(iter, bags);

          temp2 = iter;

	  curr_tasty = bags[0].total_tasty + bags[1].total_tasty + bags[2].total_tasty;
          
 	  if(curr_tasty > best_tasty)
	  {
	     iter = temp2;
	     best_tasty = curr_tasty;
	     rebagged = true;
	  }
	  else
	  {
	     iter = temp1;
          }
        }
       }
    }
    if(best_tasty > best_overall) best_overall = best_tasty;      
  }

  cout << "Refinement: " << best_overall << "\n";

  reset(bags, input);
 
  exhaustive(input, bags, 0);

  cout << "Exhaustive: " << best_pruned << "\n";

  return 0;

}
                
