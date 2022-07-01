#include <iostream>
#include <string>

std::string translate(std::string line, int argument){ 
  std::string result = ""; // implementeer dit
  for (unsigned int i = 0; i < line.size(); i++){
    char temp = line[i] + argument;
    result.push_back(temp);
  }  
  return result; 
}

int main(int argc, char *argv[]){
  std::string line;
  if(argc != 2){ 
    std::cerr << "Deze functie heeft exact 1 argument nodig" << std::endl;
    return -1; 
  }

  while(std::getline(std::cin, line)){ 
    std::cout << translate(line, std::stoi(argv[1])) << std::endl; 
  } 

  return 0; 
}
