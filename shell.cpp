//	Author 	 - Dylan Hartfield
//	Date	 - 10/11/2019
//	Function - This is a linux shell, it mimics all the commands and then a few more	

#include "shell.h"

/*
* Builds the shell
* @argv - list of command line input
* @argc - amount of command line input
*/
Shell::Shell(int argc, char **argv){
	
	//	save command line input
	this->argc = argc;
	this->argv = argv;
	
	//	populate the map that maps homework commands to real linux commands
	this->populateInputMap();
}

/**
* Sets up the map that converts homework commands into real commands
**/
void Shell::populateInputMap(){
	this->inputMap["clr"] = "clear";
	this->inputMap["dir"] = "ls";
	this->inputMap["environ"] = "printenv";
	this->inputMap["pause"] = "read -p \"press ENTER/RETURN to continue\"";
}

/**
* starts the shell, gets input, and runs commands
**/
void Shell::runShell(){
	
	//	if user supplies a batch file when running the shell
	if(this->argc>1){
		
		//	run all the commands in the batch file
		this->commandsFromFile();
		
		// exit the shell
		exit(0);
	}
	
	//	main input loop
	std::string input;	
	while(true){
		
		//	print cwd
		std::cout<<this->getcwdAsString()<<": ";
		
		//	get input from the user
		std::getline(std::cin,input);
		
		//	split process
		int pid  = fork();
		
		//	if process is parent
		if(pid>0){
			
			//	wait for child process to finish
			wait(NULL);
			
			//	go get more input
			continue;
		}
		else{
			
			//	send commands to be processed
			this->splitCommands(input);
		}
	}
}

/**
* Runs a list of commands from file if file is given when shell is called
**/
void Shell::commandsFromFile(){
	
	//	open the file
	std::ifstream file(this->argv[1]);
	
	//	for every line in the file
	std::string str;
	while (std::getline(file, str))
	{
		//	fix '\r' extra when getting lines from file
		std::replace(str.begin(),str.end(),'\r',' ');
		
		//	start command execution process
		this->checkForSpecial(str);
	}  
}

/**
* Helper function to make getting the current working directory less ugly
* @return - current working directory of shell
**/
std::string Shell::getcwdAsString() {
	char buff[100];
	getcwd( buff,100);
	std::string cwd(buff);
	return cwd;
}

/**
* Split the commands into separate commands using the ';' delimiter then sends them to be run
* @commands - string of commands with ';'s separating them
**/
void Shell::splitCommands(std::string commands){
	
	//	parse command list into individual commands
	std::vector<std::string> commandList = this->split(commands,';');
	
	for(int i=0;i<commandList.size();i++){
		checkForSpecial(commandList[i]);
	}
}

/**
* Check to see if this command needs to be handled as a special case (quit,directory change) 
* then pass the rest of the commands to be handled normally
* @command - command to be checked for special case
**/
void Shell::checkForSpecial(std::string command){
	
	//	split the command into individual arguments
	std::vector<std::string> argumentList = this->split(command,' ');
	
	//	get the name of the command
	std::string head = argumentList[0];
	
	//	kill the shell if the user said 'quit'
	if(head.compare("quit")==0){
		kill(getppid(), SIGKILL);
		exit(0);
	}
	
	//	change the directory if the user said 'cd'
	else if(head.compare("cd")==0){
		this->changeDirectory(command);
	}
	
	//	otherwise send the command to be handled normally
	else{
		this->parseAndReplace(command);
	}	
}

/**
* Changes the current working directory of the shell
* @command - name of directory you want to switch to
**/
void Shell::changeDirectory(std::string command){
	
	//	remove "cd ' from command
	command = command.substr(3,command.length()-3);
	
	//	change the directory
	int error = chdir(command.c_str());
	
	//	if the directory is not real let the user know
	if(error == -1){
		std::cout<<"Yo, that directory isn't fo' realz\n";
	}
}


/**
* Replace homework commands with their real equivalents then send commands to be executed
*@command - command that needs to be processed
**/
void Shell::parseAndReplace(std::string command){
	
	//	split command into individual pipe sections
	std::vector<std::string> pipeList = this->split(command,'|');
	
	//	replace project commands with their real equivalents
	for(int i=0;i<pipeList.size();i++){
		std::vector<std::string> argumentList = this->split(pipeList[i],' ');
		std::string head = argumentList[0];
		if(this->inputMap.find(head) != this->inputMap.end()){
			argumentList[0] = this->inputMap[argumentList[0]];
		}
		pipeList[i] = this->reverseSplit(argumentList," ");
	}
	
	
	//	put the command back together
	std::string fixedCommand = this->reverseSplit(pipeList,"|");
	
	//	execute the command
	this->executeCommand(fixedCommand);
}

/**
* Executes commands after they have been processed into real shell commands
* @command - command to be execute (already processed by shell for correctness)
**/
void Shell::executeCommand(std::string command){
	
	//	create new process to run command
	int pid = fork();
	if(pid==0){
		
		//	execute command
		execl("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL);
	}
	else{
		//	wait for child to finish before executing anymore commands
		wait(NULL);
	}
}

/**
* Helper function that concatenates a list of strings together with a given delimiter between them
* @list   - list of strings to concatenate
* @d 	  - delimiter to connect strings with
* @return - concatenated string
**/
std::string Shell::reverseSplit(std::vector<std::string> list,std::string d){
	
	//	concatenate all strings in list with d as a delimiter
	std::string ans = list[0];
	for(int i=1;i<list.size();i++){
		ans.append(d);
		ans.append(list[i]);
	}
	
	//	return the concatenated string
	return ans;
}

/**
* splits a string into individual strings given a delimiter
* @str 	  - string you want split
* @d	  - delimiter to split with
* @return - list of concatenated strings
**/
std::vector<std::string> Shell::split(std::string str,char d){
	
	//	check all characters for the delimiter
	int start = 0;
	std::vector<std::string> ans;
	for(int i=0;i<str.length();i++){
		
		//	if delimiter found add substring to the list
		if(str[i]==d){
			ans.push_back(str.substr(start,(i-start)));
			start = i+1;
			i++;
		}
	}
	
	//	if the string didn't end with a delimiter add the final substring
	if(start != str.length()-1){
		ans.push_back(str.substr(start,(str.length()-start)));
	}
	
	//	return list of strings
	return ans;
}
