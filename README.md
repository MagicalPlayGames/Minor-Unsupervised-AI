This script is a Minor Upsupervised AI using the Gaussian Algorithm.
It contains the following methods with the following uses:

//Reads the training file when training == true, and read the testing file when testing == false
void read_file(string fileName, bool training);

//Uses Gaussians distribution for probablities
void findMeans();
void findSDs();

//Finds the probablity of input to output outcomes
void findProbs();

//Prints the value of each probablity for each input
void printIndividualProbs();
