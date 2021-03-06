 /* TVL Generator - generates the template vector library from property files
    Copyright (C) 2020  MorphStore Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    */
    
#ifndef SOURCEGEN_H
#define SOURCEGEN_H

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <confuse.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include <sys/stat.h>

/*!
* This file contains convenience functions for the TVL source code generation.
* The indentation is set automatically if these functions are used.
*/

static int tab_level=0;
//!Call this if you want to write all output to another file
void switchOutputFile(std::string output){
    int fd;
    
    fd = open(output.c_str(), O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("Failed to open file!");
        std::cerr << output.c_str() << "\n";
        exit(1);
    }

    if (dup2(fd, 1) == -1) {
        perror("Failed to switch file descriptor from stdout to file!"); 
        exit(1);
    }
  return;
}

//!Used by other utility functions in this file 
void insert_tabs(){
  for (int i=0;i<tab_level;i++){
    printf("\t");  
  }
}

//!Inserts defines and includes, used for interface header 
void insert_head_of_file(char* class_name){

  printf("#ifndef %s_H\n", class_name);
  printf("#define %s_H\n\n", class_name);

  printf("#include \"../../header/vector_extension_structs.h\"\n\n");
}

void insert_include(std::string file_name){
  if (file_name.find("<") > file_name.size())
  printf("#include \"%s\"\n\n", file_name.c_str());
  else
  printf("#include %s\n\n", file_name.c_str());
}



//!Like insert_head_of_file but with additional header includes for specilization
void insert_head_of_implementation_file(char* class_name, char* isa){
  
  printf("#ifndef %s_%s_H\n", class_name, isa);
  printf("#define %s_%s_H\n\n", class_name, isa);
  
  printf("#include \"../../../header/preprocessor.h\"\n");
  printf("#include \"../../../header/extension_%s.h\"\n", isa);
  printf("#include \"../../primitives/%s.h\"\n", class_name);

  printf("#include <functional>\n");
  printf("\n");


}

//!Adds a line with the force inline macro
void force_inlineTXT(){
  insert_tabs();
  printf("MSV_CXX_ATTRIBUTE_FORCE_INLINE\n");  
  return;
}

//!Inserts the static keyword and a line break
void static_TXT(){
  insert_tabs();
  printf("static\n");  
  return;
}

//!Opens a namespace. Note: You have to close this namespace by yourself using insert_close_bracket()!
void open_namespace(std::string nsp){
  insert_tabs();
  printf("namespace %s{\n", nsp.c_str()) ;
  tab_level++;
}

//!Note: Inserts a function interface and defaults to a deleted function
void insert_function_interface(const char* function_name, int nr_arguments, std::vector<std::string> arguments, const char* return_value="void"){
  insert_tabs();
  printf("%s \n", return_value);
  insert_tabs();
  printf("%s (\n", function_name);
  tab_level++;
  
  for (int i=0; i<nr_arguments;i++){
    insert_tabs();
    if (i==nr_arguments-1){
      printf(" %s\n", arguments[i].c_str());
    }else{
      printf(" %s,\n", arguments[i].c_str());
    }
      
  }
  
  tab_level--;
  insert_tabs();
  printf(") = delete;\n");
  
}

//!Opens a function. Note: You have to close this later using insert_close_bracket(). 
void open_function(const char* function_name, int nr_arguments, std::vector<std::string> arguments, const char* return_value="void", const char* suffix=""){
  insert_tabs();
  printf("%s ", return_value);
  printf("%s%s (\n", function_name, suffix);
  tab_level++;
  
  for (int i=0; i<nr_arguments;i++){
    insert_tabs();
    if (i==nr_arguments-1){
      printf(" %s)\n", arguments[i].c_str());
    }else{
      printf(" %s,\n", arguments[i].c_str());
    }
      
  }
  
  tab_level--;
  insert_tabs();
  printf("{\n");
  tab_level++;
}


//!Opens a struct. A suffix for the name of the struct is an optional parameter. Note: You have to close this struct by yourself using insert_close_bracket()!
void open_struct(char* struct_name){
  insert_tabs();
  printf("struct %s {\n", struct_name);
  tab_level++;
}

void open_struct(const char* struct_name, const char* suffix){
  insert_tabs();
  printf("struct %s%s {\n", struct_name, suffix);
  tab_level++;
}

//!Opens a struct with a template specialization. You have to clos ethis struct with insert_close_bracket(). The template is closed automatically.
void open_struct_w_template(const char* struct_name, int nr_arguments, std::vector<std::string> arguments){
  insert_tabs();
  printf("struct %s <", struct_name);
   
   for (int i=0; i<nr_arguments;i++){
    if (i==nr_arguments-1){
      printf(" %s>", arguments[i].c_str());
    }else{
      printf("%s, ", arguments[i].c_str());
    }
      
  }
  printf(" {");
  tab_level++;
  return;
}

void open_struct_w_template(const char* struct_name, int nr_arguments, std::vector<std::string> arguments, const char* suffix){
  insert_tabs();
  printf("struct %s%s <", struct_name, suffix);
   
   for (int i=0; i<nr_arguments;i++){
    if (i==nr_arguments-1){
      printf(" %s>", arguments[i].c_str());
    }else{
      printf("%s, ", arguments[i].c_str());
    }
      
  }
  printf(" {");
  tab_level++;
  return;
}

//!Inserts a character array and a line break
void insert_characters_w_lb(const char* text){
  insert_tabs();
  printf("%s\n", text);
}

//!Inserts a character array without a line break at the end
void insert_characters_wo_lb(const char* text){
  insert_tabs();
  printf("%s", text);
}

//!Open a template with the template keyword. Close this template with close_template().
void open_template(){
     insert_tabs();
     printf("template<\n");
     tab_level++;
}

//!Opens the template for a specialization, i.e. without the template-keyword
void open_template_spec(){
     insert_tabs();
     printf("<\n");
     tab_level++;
}

//!Closes a template
void close_template(){
     tab_level--;
     insert_tabs();
     printf(">\n");
}

//!Use this function to close a namespace, function, struct, or anything else 
void insert_close_bracket(){
      tab_level--;
      insert_tabs();
      printf("}\n");
}

//!Inserts #endif
void insert_end_of_file(){
  printf("#endif\n");
}

//!Removes spaces at the end of a string
std::string remove_trail_space(std::string s)
{
	auto it = std::find_if (s.rbegin(), s.rend(),
						[](char c) {
							return !std::isspace<char>(c, std::locale::classic());
						});
	s.erase(it.base(), s.end());
	return s;
}
#endif