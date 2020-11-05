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
* This file contains convenience functions for the TVL source code generation
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

void insert_tabs(){
  for (int i=0;i<tab_level;i++){
    printf("\t");  
  }
}

void insert_head_of_file(char* class_name){

 /*  int size=0;
   for (class_name; *class_name != '\0'; class_name++)
      size++;
   
   class_name-=size;   
   char *upper_case_name = new char[size];
   
   for (int i=0; i<size; i++)
   {
       *upper_case_name = toupper(class_name[i]);
       upper_case_name++;
   }

   upper_case_name-=size;    */
  printf("#ifndef %s_H\n", class_name);
  printf("#define %s_H\n\n", class_name);

  printf("#include \"../vector_extension_structs.h\"\n\n");
}

void insert_head_of_implementation_file(char* class_name, char* isa){
  
  printf("#ifndef %s_%s_H\n", class_name, isa);
  printf("#define %s_%s_H\n\n", class_name, isa);
  
  printf("#include \"../../../preprocessor.h\"\n");
  printf("#include \"../extension_%s.h\n", isa);
  printf("#include \"../../../primitives/%s.h\"\n", class_name);

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

//!Opens a namespace. Note: You have to close this namespace by yourself!
void open_namespace(std::string nsp){
  insert_tabs();
  printf("namespace %s{\n", nsp.c_str()) ;
  tab_level++;
}

//!Note: Inserts a function interface and defaults to a deleted function
void insert_function_interface(char* function_name, int nr_arguments, std::vector<std::string> arguments, char* return_value="void"){
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

void open_function(char* function_name, int nr_arguments, std::vector<std::string> arguments, char* return_value="void"){
  insert_tabs();
  printf("%s ", return_value);
  printf("%s (\n", function_name);
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
//!Opens a struct. Note: You have to close this struct by yourself!
void open_struct(char* struct_name){
  insert_tabs();
  printf("struct %s {\n", struct_name);
  tab_level++;
}

void open_struct_w_template(char* struct_name, int nr_arguments, std::vector<std::string> arguments){
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

//!Inserts a character array and a line break
void insert_characters_w_lb(char* text){
  insert_tabs();
  printf("%s\n", text);
}

//!Inserts a character array without a line break at the end
void insert_characters_wo_lb(char* text){
  insert_tabs();
  printf("%s", text);
}

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

void insert_end_of_file(){
  printf("#endif\n");
}

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