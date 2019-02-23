#include <iostream>
#include <vector>  // Used to store population
#include <cstdlib> // Used to generate random numbers
#include <ctime>   // Used to generate random numbers
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

/*
    Information taken from: https://www.sentient.ai/blog/ai-101-intro-evolutionary-algorithms/
    Evolutionary algorithms (EA):
        Evolutionary algorithms is inspired by the evolution theory of biology
        and use mechanisms that imitate the evolutionary concepts of:
            - reproduction;
            - mutation;
            - recombination;
            - selection;
        EA are used in a wide variety of applications such as image processing,
        vehicle routing, mobile communications optimization, software engineering,
        and even training Artificial Neural Networks (ANN).
        EAs are so robust that they can be used to solve almost any optimization problem.

        The common underlying idea that unifies these approaches is the same.
        Suppose you have a problem you wish to find the solution for, and suppose this
        solution is not apparent. Or perhaps you have a solution, but you are unsure
        if this solution is the most optimal solution to your problem. This is precisely
        the kind of situation that Evolutionary Algorithms are meant to address.

    Generic algorithms:
        Genetic algorithms belong to the larger class of evolutionary algorithms (EA).
        Generic algorithms is used to solve optimization problems.
        The Genetic Algorithm is an heuristic optimization method inspired by the procedures of natural evolution.
        Population corresponds to the set of Chromosomes.
        Chromosome corresponds to genes.
        Genes correspond to the values for each variable of the problem.

        There are five phases in generic algorithms:
            Creating an initial population:
                Create a set of n elements called population
                each element from the population is a solution to the problem

            Defining a fitness function:
                The fitness function determine how likely an individual is fit to be
                selected for reproduction, this is based on its fitness score.
                For example, let's say we have a word COUTE, and our target word is HOUSE,
                we could use here as a fitness score, the number of letters that matches with the
                target one, which in this case would be 3 (O, U and E).

            selecting the parents:
                The idea here is to select the fittest individuals and let them pass
                their genes to the next generation. two elements of the population is selected
                based on their fitness score.
                Tournament Selection is a Selection Strategy used for selecting the fittest candidates from the current generation in a Genetic Algorithm.

            making a crossover:
                It's the most important phase in a GA. in this step we reproduce a new population
                of n elements from the selected elements.
                Four ways techniques to make crossover:
                    - Single-point crossover:
                        [1,0,1,1,1,0] x [1,0,0,0,0,1]
                        pick randomly a point (crossover point) and bits to the right of that point
                        are swapped between the two parents. Suppose it was picked the point at the position 3.
                        it generates two offspring:
                        [1,0,1,1,0,1] and [1,0,0,0,1,0]
                    - Two point and k-point crossover:
                        [1,0,1,1,1,0] x [1,0,0,0,0,1]
                        two crossover points are picked randomly from the parent chromosomes.
                        The bits in between the two points are swapped between the parent organisms. Suppose it was
                        picked the points 2 and 4.
                        [1,0,1,0,1,0] and [1,0,0,1,0,1]
                    and others, such as: Crossover for ordered lists, uniform crossover, etc.

            mutation:
                There are chances that from the crossover phase, we might get a population which
                will not contribute to the evolution of a new diverse population and our algorithm
                will converge prematurely. So we need to alter the sequence of words from 1% of the
                newly created population to maintain this diversity. We can choose any sort of alteration.

            When does this process stop?
                Our population has a fixed size. As new elements are formed, old elements with low fitness score
                are removed. When the population has converged, i.e., no new elements are reproduced which are
                significantly different from the previous population, then we may say that the genetic algorithm
                has provided a set of solutions to our problem.

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

    The one max problem:
        From a specific gene, which is essentially a piece of text filled with random binary values (0 or 1),
        get in the target gene which is all genes as 1.
        Example:
        from 10101010000 to 11111111111
        The objective in this problem is to find a binary string that is identical to a specified target.

*/

ostream& operator << (ostream& stream, vector<int>& al) {
    for(int i = 0; i < (int)al.size(); i++) {
        if(i != 0)
            stream << " ";
        stream << al[i];
    }
    return stream;
}

class MaxOne {
private:
    int numberOfGenes;
    int numberOfPopulation;
    int numberOfGeneration;
    int numberOfTournament;
    double probabilityOfMultation, probabilityOfCrossOver;
    vector<vector<int> > population;
public:
    MaxOne(int numberOfGenes, int numberOfPopulation, int numberOfGeneration, int numberOfTournament, double probabilityOfMultation, double probabilityOfCrossOver) {
        this->numberOfGenes = numberOfGenes;
        this->numberOfGeneration = numberOfGeneration;
        this->numberOfPopulation = numberOfPopulation;
        this->numberOfTournament = numberOfTournament;
        this->probabilityOfCrossOver = probabilityOfCrossOver;
        this->probabilityOfMultation = probabilityOfMultation;

        // Initializing population of chromosomes with random genes varying in 1 or 0
        population.assign(this->numberOfPopulation, vector<int>(this->numberOfGenes, 0));
        for (int i = 0; i < this->numberOfPopulation; i++) {
            for(int j = 0; j < numberOfGenes; j++) {
                population[i][j] = rand() % 2; // it generates either 0 or 1
            }
        }
    }

    // Compute fitness score
    int fitnessScore(const vector<int>& chromosome) {
        int score = 0;
        for_each(chromosome.begin(), chromosome.end(), [&score](const int& n) {
            if(n == 1)
                score++;
        });

        return score;
    }

    // return the index of the chromosome with the best score
    int getFittest() {
        int bestScore = -1, index = 0;
        for (int i = 0; i < this->numberOfPopulation; i++) {
            int score = fitnessScore(this->population[i]);
            if(score > bestScore) {
                bestScore = score;
                index = i;
            }
        }
        return index;
    }

    // Selecting two random parents
    void selection(int& parent1, int& parent2) {
        // Getting two random parents
        do {
            parent1 = rand() % numberOfPopulation;
            parent2 = rand() % numberOfPopulation;
        } while(parent1 != parent2);
    }

    // crossover between two chromosomes to generate the offspring
    void crossover(vector<int>& parent1, vector<int>& parent2, vector<int>& offspring) {
        // Using the single-point crossover technique by getting a random point
        // From 0 to the number of genes
        int point = rand() % this->numberOfGenes;

        // From 0 to the position picked randomly, genes from parent1 goes to the offspring
        for (int i = 0; i < point; i++) {
            offspring[i] = parent1[i];
        }
        // From point to the maximum number of genes, genes from parent2 goes to the offspring
        for (int i = point; i < this->numberOfGenes; i++) {
            offspring[i] = parent2[i];
        }
    }

    // Mutation in a offspring
    void mutation(vector<int>& offspring) {
        // Choosing randomly one gene between 0 and the number of genes
        int gene = rand() % this->numberOfGenes;
        // Changing the gene in the gene position, if it's 1, it becomes 0, and vice versa
        offspring[gene] = !offspring[gene];
    }

    // method where the algorithm works, here all of the process of selection,
    // crossover, mutation, fitness score is going to happen until it converges
    void run() {
        int cont = 0, parent1, parent2, bestScore, bestIndex;
        double probability;
        do {
            // doing the selection of two parents using tournament selection strategy
            for (int i = 0; i < numberOfTournament; i++) {
                // Generating a random number between 0 and 1 to check the probability of a crossover to happen
                probability = ((rand() % 100) + 1) / 100.0;

                // If the probability is less than the probability of crossover,
                // then match the two parents picked randomly
                if(probability < probabilityOfCrossOver) {
                    // getting indexes for parent 1 and 2
                    selection(parent1, parent2);
                    // Vector to receive the genes from parent 1 and 2
                    vector<int>offspring(this->numberOfGenes, 0);
                    // crossover between parent 1 and 2
                    crossover(this->population[parent1], this->population[parent2], offspring);
                    // Getting the probability of the mutation from 0 to 1
                    probability = ((rand() % 100) + 1) / 100.0;
                    // if the probability picked randomly is less than the probability of mutation, then
                    // call the mutation method
                    if(probability < probabilityOfMultation) {
                        mutation(offspring);
                    }

                    // Generating a new individual to the population
                    int scoreParent1 = fitnessScore(this->population[parent1]);
                    int scoreParent2 = fitnessScore(this->population[parent2]);
                    int scoreOffspring = fitnessScore(offspring);
                    // Getting the parent if worst score between them and swap its position in case
                    // parent two is the worst in order to use only parent1 and scoreParent1 variables
                    if(scoreParent1 > scoreParent2) {
                        swap(scoreParent1, scoreParent2);
                        swap(parent1, parent2);
                    }

                    // Checking if generated chromosome is better than one of its parents, if it is, then update it
                    if(scoreOffspring > scoreParent1) {
                        // Overwritten its parent with its offspring
                        this->population[parent1] = offspring;
                    }
                }
            }
            cont++;
            // getting index of the chromosome with the best score
            bestIndex = this->getFittest();
            bestScore = fitnessScore(this->population[bestIndex]);
            cout << "Generation: " << cont << endl;
            cout << "Best chromosome: " << this->population[bestIndex] << endl;
            cout << "Best score: " << bestScore << "\n\n";
          //  this_thread::sleep_for(chrono::milliseconds(1000));
          // repeating loop until the maximum number of generation hasn't been achieved and the target solution
          // which contains the desirable score hasn't been achieved yet
        } while(cont < numberOfGeneration && bestScore != numberOfGenes);
    }

    // setter methods
    void setNumberOfGenes(int numberOfGenes) {
        this->numberOfGenes = numberOfGenes;
    }
    void setNumberOfGeneration(int numberOfGeneration) {
        this->numberOfGeneration = numberOfGeneration;
    }
    void setnumberOfPopulation(int numberOfPopulation) {
        this->numberOfPopulation = numberOfPopulation;
    }
    void setnumberOfTournament(int numberOfTournament) {
        this->numberOfTournament = numberOfTournament;
    }
    void setprobabilityOfCrossOver(double probabilityOfCrossOver) {
        this->probabilityOfCrossOver = probabilityOfCrossOver;
    }
    void setprobabilityOfMultation(double probabilityOfMultation) {
        this->probabilityOfMultation = probabilityOfMultation;
    }

    // Getter methods
    int setNumberOfGenes() const {
        return this->numberOfGenes;
    }
    int setNumberOfGeneration() const {
        return this->numberOfGeneration;
    }
    int setnumberOfPopulation() const {
        return this->numberOfPopulation;
    }
    int setnumberOfTournament() const {
        return this->numberOfTournament;
    }
    double setprobabilityOfCrossOver() const {
        return this->probabilityOfCrossOver;
    }
    double setprobabilityOfMultation() const {
        return this->probabilityOfMultation;
    }
};

int main()
{
    srand(time(NULL));

    MaxOne ga(20, 60, 110, 30, 0.3, 0.8);

    ga.run();

    return 0;
}
