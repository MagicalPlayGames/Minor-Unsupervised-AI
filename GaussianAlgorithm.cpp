#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <unordered_map>

/*         Summary
<<<<<<<<<<<<<<>>>>>>>>>>>>>>
This script uses Gaussians Distrubution Algorithms.

Gaussians distribution: 1/(sqrt(2*pi)*SDi = probablity. 
The probablity that Attribute i (Xi) leads to the correct outcome k (Y=Yk)
FOR CONTINOUS VALUES
*/


using namespace std;

void read_file(string fileName, bool training);
//Gaussians distribution for probablities
void findMeans();
void findSDs();
//Finds the probablity of input to output outcomes
void findProbs();
//Prints the value of each probablity for each input
void printIndividualProbs();

struct valuesOfY
{
    unordered_map<int,double> mapForX;
};
     //Where lineNum[int] X<int> = <double>
    //Y=-1 = structs 0-13, Y=1 = structs 14-27 where
   /*(Y = -1 X1 = forEachX[0], X2 = forEachX[1]....) and (Y = 1 X1 = forEachX[0], X2 = forEachX[1])*/
    valuesOfY forEachX[28];

struct UniqueXs
{
    unordered_map<double, int> mapOfUniques;
};
     //Where value X[int] = <double>, <int> # of times
    //Y=-1 = structs 0-13, Y=1 = structs 14-27
    UniqueXs UniqueX[28];
    int uniqueTotals[28];

    struct ProbPerX
    {
        unordered_map<double, double> Probiblity;
    };
      //P(X[int] = <double> Y[same int]=<double>
     //Y=-1 = structs 0-13, Y=1 = structs 14-27
    //Store probablities of each value for each row to predict the outcome
    ProbPerX Problist[28];

    //For Gaussians
    double SDs[28];
    double means[28];

    double Probs[28];

    //how many times does Y=1
    double y1Lines = 0;
    //how many times does Y=-1
    double y2Lines = 0;

    int success = 0;
    int fail = 0;

int main() {

    for (int i = 0; i < 28; i++)
    {
        uniqueTotals[i] = 0;
    }

    read_file("TrainData.csv",true);
    findProbs();
    read_file("TestData.csv", false);
    cout << "Successes: " << success << " Fails: " << fail << endl;
    cout << "Accuracy: " << ((double)(success)) / ((double)(fail + success)) << endl;
    //printIndividualProbs();
    return 0;
}

//Finds the means for each outcome from the training set
void findMeans()
{
    double totals[28];
    for (int i = 0; i < 28; i++)
    {
        double num = 0;
        totals[i] = 0.0f;
        //Continous values
        if (i == 0 || i == 2 || i == 4 || i == 10 || i == 11 || i == 12 ||
            i == 14 || i ==16 || i ==18 || i == 24 || i == 25 || i == 26)
        {
            //X<int> =  <double)>
            unordered_map<int, double>::iterator itr;
            unordered_map<int, double> m = forEachX[i].mapForX;
            for (itr = m.begin(); itr != m.end(); itr++)
            {
                num++;
                totals[i] += itr->second;
            }
            means[i] = totals[i] / num;
        }
    }
}

//Finds the standard deviation for each outcome from the training set
void findSDs()
{
    cout << endl << endl;
    double totals[28];
    for (int i = 0; i < 28; i++)
    {
        double num = 0;
        totals[i] = 0.0f;
        //Continous values
        if (i == 0 || i == 2 || i == 4 || i == 10 || i == 11 || i == 12 ||
            i == 14 || i == 16 || i == 18 || i == 24 || i == 25 || i == 26)
        {
            //X<int> =  <double)>
            unordered_map<int, double>::iterator itr;
            unordered_map<int, double> m = forEachX[i].mapForX;
            for (itr = m.begin(); itr != m.end(); itr++)
            {
                num++;
                totals[i] += pow(itr->second - means[i],2);
            }
            SDs[i] = sqrt(totals[i] / num);
        }
    }
}

//Putting all the probablities together
void findProbs()
{
    cout << endl << endl;
    findMeans();
    findSDs();
    double totals[28];
    for (int i = 0; i < 28; i++)
    {
        double num = 0;
        totals[i] = 0.0f;

        unordered_map<int, double>::iterator itr;
        unordered_map<int, double> m = forEachX[i].mapForX;//For X[i] #<int>(itr first) = <double>(itr second), For Xi = second, Y=1,-1 first num of times
        for (itr = m.begin(); itr != m.end(); itr++)
        {

            //Continous values
            if (i == 0 || i == 2 || i == 4 || i == 10 || i == 11 || i == 12 ||
                i == 14 || i == 16 || i == 18 || i == 24 || i == 25 || i == 26)
            {
                double expo = exp(-0.5 * pow((itr->second - means[i]), 2) / pow(SDs[i], 2));
                double finalP = (1.0f / (sqrt(2.0f * 3.14)*SDs[i])) * expo;

                if (Problist[i].Probiblity.find(itr->second) == Problist[i].Probiblity.end())
                    Problist[i].Probiblity.insert(make_pair(itr->second, finalP));

            }
            //Discrete values
            else
            {
                if(i<14)
                    Problist[i].Probiblity.insert(make_pair(itr->second,UniqueX[i].mapOfUniques.at(itr->second) / y2Lines));
                else
                    Problist[i].Probiblity.insert(make_pair(itr->second, UniqueX[i].mapOfUniques.at(itr->second) / y1Lines));
            }
        }
    }
}


void printIndividualProbs()
{
    for (int i = 0; i < 28; i++)
    {
            unordered_map<double, double>::iterator itr;
            unordered_map<double, double> m = Problist[i].Probiblity;
            for (itr = m.begin(); itr != m.end(); itr++)
            {
                if (i >= 14)
                {
                    cout << "When Y = +1 ";
                }
                else
                {
                    cout << "When Y = -1 ";
                }
                cout << "The probability of \tX" << i % 14 << " =" << itr->first << " is:\t" << itr->second << endl;
            }
    }
}

void read_file(string fileName, bool training)
{
    ifstream file(fileName);
    if (!file.is_open()) throw runtime_error("Could not open file");
    string line;
    getline(file, line);// Skip the first line in the file

    int lineNum = 0;
    while (getline(file, line)) //Each line containing x1, x2,...x14 y, in order
    {

        string str = line;
        vector<double> values;

        stringstream ss(str);

        for (double i; ss >> i;) {
            values.push_back(i);
            if (ss.peek() == ',')
                ss.ignore();
        }
        if (training)
        {
            if (values[14] == -1)
            {
                y2Lines++;
            }
            else
            {
                y1Lines++;
            }
            for (int i = 0; i < 14; i++)
            {
                int YXloc = 0;
                if (values[14] == -1)
                {
                    YXloc = 0 + i;
                }
                else
                {
                    YXloc = 14 + i;
                }
                forEachX[YXloc].mapForX.insert(make_pair(lineNum, values[i]));


                //For Y=<int>,X<same int>,=values[double] is in the mapOfUniques
                if (UniqueX[YXloc].mapOfUniques.find(values[i]) == UniqueX[YXloc].mapOfUniques.end())
                {
                    UniqueX[YXloc].mapOfUniques.insert(make_pair(values[i], 1));
                }
                else
                {
                    UniqueX[YXloc].mapOfUniques[values[i]]++;
                }

            }
        }
        //Find the probablitiy and the accuracy
        else
        {
            double predictedY1 = 1.0f;
            double predictedY2 = 1.0f;
            for (int i = 0; i < 28; i++)
            {
                //Y=-1
                if (i < 14)
                {
                    if (Problist[i].Probiblity.find(values[i]) != Problist[i].Probiblity.end())
                        predictedY2 *= Problist[i].Probiblity[values[i]];
                }
                //Y=1
                else
                {
                    if (Problist[i].Probiblity.find(values[i-14]) != Problist[i].Probiblity.end())
                        predictedY1 *= Problist[i].Probiblity[values[i - 14]];
                }
            }
            if (predictedY1 >= predictedY2)
            {
                if (values[14] == 1)
                    success++;
                else
                    fail++;
            }
            else
            {
                if (values[14] == -1)
                    success++;
                else
                    fail++;
            }

        }
        lineNum++;
    }
}
