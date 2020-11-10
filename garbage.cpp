#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

/*
Here, ofstream is an “output file stream”.
Since it’s derived from ostream, we can treat it just like cout (which is also derived from ostream).
The result of executing this program is that we get a file called foo.csv in the same directory as our executable.
 
*/
int write_to_csv0() {
    // Create an output filestream object
    std::ofstream myFile("foo.csv");
    
    // Send data to the stream
    myFile << "Foo\n";
    myFile << "1\n";
    myFile << "2\n";
    myFile << "3\n";
    
    // Close the file
    myFile.close();
    
    return 0;
}
//Let’s wrap this into a write_csv() function that’s a little more dynamic
void write_csv00(std::string filename, std::string colname, std::vector<int> vals){
    // Make a CSV file with one column of integer values
    // filename - the name of the file
    // colname - the name of the one and only column
    // vals - an integer vector of values
    
    // Create an output filestream object
    std::ofstream myFile(filename);
    
    // Send the column name to the stream
    myFile << colname << "\n";
    
    // Send data to the stream
    for(int i = 0; i < vals.size(); ++i)
    {
        myFile << vals.at(i) << "\n";
    }
    
    // Close the file
    myFile.close();
}
//Let’s expand on this to support multiple vectors of integers and corresponding column names.
void write_csv(std::string filename, std::vector<std::pair<std::string, std::vector<int>>> dataset){
    // Make a CSV file with one or more columns of integer values
    // Each column of data is represented by the pair <column name, column data>
    //   as std::pair<std::string, std::vector<int>>
    // The dataset is represented as a vector of these columns
    // Note that all columns should be the same size
    
    // Create an output filestream object
    std::ofstream myFile(filename);
    
    // Send column names to the stream
    for(int j = 0; j < dataset.size(); ++j)
    {
        myFile << dataset.at(j).first;
        if(j != dataset.size() - 1) myFile << ","; // No comma at end of line
    }
    myFile << "\n";
    
    // Send data to the stream
    for(int i = 0; i < dataset.at(0).second.size(); ++i)
    {
        for(int j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset.at(j).second.at(i);
            if(j != dataset.size() - 1) myFile << ","; // No comma at end of line
        }
        if (i != dataset.at(0).second.size() -1) myFile << "\n"; // don't \n after last line.
        
    }
    
    // Close the file
    myFile.close();
}



std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename){
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    std::vector<std::pair<std::string, std::vector<int>>> result;

    // Create an input filestream
    std::ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;
    int val;

    // Read the column names
    if(myFile.good())
    {
        // Extract the first line in the file
        std::getline(myFile, line);

        // Create a stringstream from line
        std::stringstream ss(line);

        // Extract each column name
        while(std::getline(ss, colname, ',')){
            
            // Initialize and add <colname, int vector> pairs to result
            result.push_back({colname, std::vector<int> {}});
        }
    }

    // Read data, line by line
    while(std::getline(myFile, line))
    {
        // Create a stringstream of the current line
        std::stringstream ss(line);
        
        // Keep track of the current column index
        int colIdx = 0;
        
        // Extract each integer
        while(ss >> val){
            
            // Add the current integer to the 'colIdx' column's values vector
            result.at(colIdx).second.push_back(val);
            
            // If the next token is a comma, ignore it and move on
            if(ss.peek() == ',') ss.ignore();
            
            // Increment the column index
            colIdx++;
        }
    }

    // Close file
    myFile.close();

    return result;
}


int main() {
    // Make three vectors, each of length 100 filled with 1s, 2s, and 3s
    std::vector<int> vec1(100, 1);
    std::vector<int> vec2(100, 2);
    std::vector<int> vec3(100, 3);
    
    // Wrap into a vector
    std::vector<std::pair<std::string, std::vector<int>>> vals = {{"One", vec1}, {"Two", vec2}, {"Three", vec3}};
    
    // Write the vector to CSV
    write_csv("three_cols.csv", vals);
    
  std::vector<std::pair<std::string, std::vector<int>>> three_cols = read_csv("three_cols.csv");
    std::vector<std::pair<std::string, std::vector<int>>> ones = read_csv("ones.csv");

    
    write_csv("ones_copy.csv", ones);
    
    return 0;
}