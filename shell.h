#ifndef SHELL_H
#define SHELL_H
#include <unordered_map>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h> 
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
class Shell {

	public:
	
		/**
		* Builds the shell
		* @argv - list of command line input
		* @argc - amount of command line input
		**/
		Shell(int argc, char **argv);
		
		/**
		* starts the shell, gets input, and runs commands
		**/
		void runShell();
		
	private:
	
		//	command line arguments
		int argc;
		char** argv;
		
		//	map that relates project commands into actual linux commands
		std::unordered_map<std::string,std::string> inputMap;
		
		/**
		* Sets up the map that converts homework commands into real commands
		**/
		void populateInputMap();
		
		/**
		* Split the commands into separate commands using the ';' delimiter then sends them to be run
		* @commands - string of commands with ';'s separating them
		**/
		void splitCommands(std::string commands);
		
		/**
		* Executes commands after they have been processed into real shell commands
		* @command - command to be execute (already processed by shell for correctness)
		**/
		void executeCommand(std::string command);
		
		/**
		* splits a string into individual strings given a delimiter
		* @str 	  - string you want split
		* @d	  - delimiter to split with
		* @return - list of concatenated strings
		**/
		std::vector<std::string> split(std::string str,char d);
		
		/**
		* Check to see if this command needs to be handled as a special case (quit,directory change) 
		* then pass the rest of the commands to be handled normally
		* @command - command to be checked for special case
		**/
		void checkForSpecial(std::string command);
		
		/**
		* Replace homework commands with their real equivalents then send commands to be executed
		*@command - command that needs to be processed
		**/
		void parseAndReplace(std::string command);
		
		/**
		* Helper function that concatenates a list of strings together with a given delimiter between them
		* @list   - list of strings to concatenate
		* @d 	  - delimiter to connect strings with
		* @return - concatenated string
		**/
		std::string reverseSplit(std::vector<std::string> list,std::string d);
		
		/**
		* Runs a list of commands from file if file is given when shell is called
		**/
		void commandsFromFile();
		
		/**
		* Changes the current working directory of the shell
		* @command - name of directory you want to switch to
		**/
		void changeDirectory(std::string command);
		
		/**
		* Helper function to make getting the current working directory less ugly
		* @return - current working directory of shell
		**/
		std::string getcwdAsString();
};
#endif