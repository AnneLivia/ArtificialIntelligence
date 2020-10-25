#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

/*
    Algorithm PseudoCode:
        Start
            - create population
            - compute fitness
            - repeat
                - selection
                - crossover
                - mutation
                - compute fitness
            - until population has converged
    stop

*/

int fitness(const string& a1, const string& target) {
    int cont = 0;
    for (int i = 0; i < (int)target.size(); i++)
        cont+= a1[i] == target[i] ? 1 : 0;
    return cont;
}

int getFittest(const vector<string>& population, const string& target) {
    int best_score = -1;
    int index = 0;
    for (int i = 0; i < (int)population.size(); i++) {
        int score = fitness(population[i], target);
        if (score > best_score) {
            best_score = score;
            index = i;
        }
    }
    return index;
}

void mutate(string& chromosome, const string& letters) {
    // mutation changes one gene from the chromosome
    int random_index = rand() % (int)chromosome.size();
    int random_letter = 0;
    do {
        random_letter = rand() % (int)letters.size();

        // avoid getting repeated letter
    } while(letters[random_letter] == chromosome[random_index]);

    chromosome[random_index] = letters[random_letter];
}

// crossover between two chromosomes to generate the offspring using single_point technique
string crossover(const string& parent1, const string& parent2) {
    // Using the single-point crossover technique by getting a random point
    // From 0 to the number of genes
    /*
        One Point Crossover. In this one-point crossover,
        a random crossover point is selected and the tails
        of its two parents are swapped to get new off-springs.
        there are many other techniques like two-pointer and uniform crossover (research later)
    */
    int point_division = rand() % (int)parent1.size();
    string offspring = "";
    for (int i = 0; i <= point_division; i++) {
        offspring+= parent1[i];
    }

    for (int i = point_division+1; i < (int)parent2.size(); i++) {
        offspring += parent2[i];
    }

    return offspring;
}

int main()
{
    srand(time(NULL));

    // using genetic algorithm to guess the password (without space)
    string target = "passwordpasspasspass";
    string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // need to generate chromosomes of random words
    /*
        n genetic algorithms, a chromosome (also sometimes called a genotype)
        is a set of parameters which define a proposed solution to the problem
        that the genetic algorithm is trying to solve. The set of all solutions
        is known as the population.
    */

    int number_population = 200;
    vector<string>population;
    while((int)population.size() < number_population) {
        // generating population
        string aux = "";
        for (int i = 0; i < (int)target.size(); i++) {
            aux+=letters[rand() % (int)letters.size()];
        }
        population.push_back(aux);
    }

    // the algorithm will run until it reach the number of generations or it converges (find the best solution)
    int number_of_generations = 300, number_of_tournament = 100;

    // variables used to save best chromosome
    string best_chromosome = "";
    int best_score = 0;

    /*

    */

    double probability_crossover = 0.3, probability_mutation = 0.8;

    for (int generation = 0; generation < number_of_generations; generation++) {

        /*
            Tournament Selection is a Selection Strategy used for selecting the
            fittest candidates from the current generation in a Genetic Algorithm.
            These selected candidates are then passed on to the next generation.
            In a K-way tournament selection, we select k-individuals and run
            a tournament among them. Only the fittest candidate amongst
            those selected candidates is chosen and is passed on to the
            next generation.

        */

        for (int tournament = 0; tournament < number_of_tournament; tournament++) {

            // Generating a random number between 0 and 1 to check the probability of a crossover to happen
            double probability_cross = ((rand() % 100) + 1) / 100.0;
            double probability_mut = ((rand() % 100) + 1) / 100.0;

            if (probability_cross < probability_crossover) {
                // select two parents randomly
                int parent1, parent2;
                do {
                    parent1 = rand() % number_population;
                    parent2 = rand() % number_population;
                } while (parent1 == parent2);

                 /*
                    In genetic algorithms and evolutionary computation,
                    crossover, also called recombination, is a genetic
                    operator used to combine the genetic information of two
                    parents to generate new offspring.
                */
                string offspring = crossover(population[parent1], population[parent2]);

                // check probability of mutation
                if (probability_mut < probability_mutation) {
                    mutate(offspring, letters);
                }

                // substitute the parent with the smaller score
                int scoreP1 = fitness(population[parent1], target);
                int scoreP2 = fitness(population[parent2], target);
                int scoreOffspring = fitness(offspring, target);

                // in order to avoid repeat code (substitute only first parent)
                if (scoreP1 > scoreP2) {
                    swap(scoreP1, scoreP2);
                    swap(parent1, parent2);
                }
                // only change if score of offspring is greater
                if (scoreOffspring > scoreP1) {
                    population[parent1] = offspring;
                }
            }
        }

        /*
            The fitness value the genetic algorithm provides is the only feedback the engine gets to guide it toward a solution. In
            this project the fitness value is the total number of letters in the guess that match the letter in the same position of the
            password.
        */

        int index = getFittest(population, target);
        best_score = fitness(population[index], target);
        best_chromosome = population[index];

        cout << "Generation: " << generation << endl;
        cout << "Optimal solution: " << best_chromosome << endl;
        cout << "Score: " << best_score << "\n\n";

        this_thread::sleep_for(chrono::milliseconds(300));
        if (best_score == (int)target.size())
            break;
    }


    return 0;
}
