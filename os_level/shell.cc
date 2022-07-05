#include "shell.hh"

int main()
{ std::string input;
  std::string configbestand_naam = "config";
  const char* charconfig= configbestand_naam.c_str();

  int configbestand = syscall(SYS_open, charconfig, O_RDONLY,  0755);
  
  std::string bericht = "";
  
  char byte[1];
  while(syscall(SYS_read, configbestand, byte, 1)){
    bericht +=byte;
  }
  bericht.pop_back();

  // ToDo: Vervang onderstaande regel: Laad prompt uit bestand
  std::string prompt = bericht;


  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } 
}      // EOF is een exit

void new_file(){ 
  std::string naam_bestand = "";
  std::cout << "Naam bestand: "<< "\n";
  std::cin >> naam_bestand;
  std::cin.ignore(1,'\n');

  const char* char_naam = naam_bestand.c_str();

  syscall(SYS_creat, char_naam, 0755);
  int bestand = syscall(SYS_open, char_naam, O_WRONLY, 0755);


  std::cout << "Voer tekst in: \n";
  std::string tekst = "";
  while (true){
    std::getline(std::cin, tekst);
    if (tekst == "<EOF>"){
      break;
    }
    tekst += '\n';
    const char* char_tekst = tekst.c_str();
    syscall(SYS_write, bestand, char_tekst, strlen(char_tekst));
  }
  syscall(SYS_close, bestand);
}

void list(){ 
  int pid = syscall(SYS_fork);

  if (pid == 0){
    const char* arg[] = {"/bin/ls", "-la", NULL};
    syscall(SYS_execve, arg[0], arg, NULL);
  }else{
    syscall(SYS_wait4, pid, NULL, NULL, NULL);
  }
}

void find(){ 
  std::string zoeken = "";
  std::cout << "Geef de string die gezocht moet worden: ";
  std::cin >> zoeken;
  std::cin.ignore(1, '\n');

  const char* char_zoeken = zoeken.c_str();

  int fd[2];
  syscall(SYS_pipe, &fd);

  int pid = syscall(SYS_fork);
  if (pid == 0){
    syscall(SYS_close, fd[0]);
    syscall(SYS_dup2, fd[1], 1);

    const char* arg[] = {"/usr/bin/find", ".", NULL};
    syscall(SYS_execve, arg[0], arg, NULL);
  }else{
    int pid_2 = syscall(SYS_fork);
    if(pid_2 == 0){
      syscall(SYS_close, fd[1]);
      syscall(SYS_dup2, fd[0], 0);
      const char* arg[] = {"/bin/grep", char_zoeken, NULL};
      syscall(SYS_execve, arg[0], arg, NULL);
    }else{
      syscall(SYS_close, fd[0]);
      syscall(SYS_close, fd[1]);

      syscall(SYS_wait4, pid, NULL, NULL, NULL);
      syscall(SYS_wait4, pid_2, NULL, NULL, NULL);
    }
  } 
}

void seek(){ 
  int pid = syscall(SYS_fork);
  if (pid == 0){
    std::string zoeken = "seek";
    const char* char_zoeken = zoeken.c_str();

    int bestand_zoeken = syscall(SYS_creat, char_zoeken, 0755);
    int bestand = syscall(SYS_open, char_zoeken, O_WRONLY, 0755);

    std::string x = "x";
    const char* char_x = x.c_str();

    syscall(SYS_write, bestand_zoeken, char_x, strlen(char_x));
    syscall(SYS_lseek, bestand_zoeken, 5000000, SEEK_CUR);
    syscall(SYS_write, bestand_zoeken, char_x, strlen(char_x));

    syscall(SYS_close, bestand_zoeken);
  }else{
    int pid_2 = syscall(SYS_fork);
    if (pid_2 ==0){
      std::string loop = "loop";
      const char* char_loop = loop.c_str();

      syscall(SYS_creat, char_loop, 0755);
      int bestand_loop = syscall(SYS_open, char_loop, O_WRONLY, 0755);

      std::string x = "x";
      const char* char_x = x.c_str();
      syscall(SYS_write, bestand_loop, char_x, strlen(char_x));

      for (unsigned int i = 0; i < 5000000; i++){
        syscall(SYS_write, bestand_loop, "\0", 1);
      }
      syscall(SYS_write, bestand_loop, char_x, strlen(char_x));
      syscall(SYS_close, bestand_loop);
    }else{
      syscall(SYS_wait4, pid, NULL, NULL, NULL);
      syscall(SYS_wait4, pid_2, NULL, NULL, NULL);
    }
  }
  
  
  
  std::cout << "ToDo: Implementeer hier seek()" << std::endl; 
}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
