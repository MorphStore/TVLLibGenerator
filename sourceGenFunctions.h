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
    
#ifndef SOURCEGENFUNCTIONS_H
#define SOURCEGENFUNCTIONS_H

#include "sourceGen.h"


//! Structure of the interface configuration (configs/<primitive_class>/<primitive_name>/interface.conf)
static cfg_opt_t interface_opts[] =
{
  CFG_STR("primitive_class", " ", CFGF_NONE),
  CFG_STR("primitive_name", " ", CFGF_NONE),
  CFG_STR_LIST("templates", "{ }", CFGF_NONE),
  CFG_STR("return_type", "void", CFGF_NONE),
  CFG_STR_LIST("arguments", "{ }", CFGF_NONE), 
  CFG_END()
};

//! The primitive list configuration is just a list of primitive names (configs/<primitive_class>/primitives.conf) and any additional headers, which could be needed
static cfg_opt_t primitives_opts[] =
{
  CFG_STR_LIST("primitives", "{ }", CFGF_NONE),
  CFG_STR_LIST("header", "{ }", CFGF_NONE),
  CFG_END()
};

//! A global configuration file defines the primitive class names and the ISAs (configs/config.conf)
static cfg_opt_t class_opts[] =
{
  CFG_STR_LIST("classes", "{ }", CFGF_NONE),
  CFG_STR_LIST("isa", "{ }", CFGF_NONE),
  CFG_END()
};
  
//! There is one section like this per register size in the primitive configurations (configs/<primitive_class>/<primitive_name>/<isa>.conf)       
static cfg_opt_t ps_section[] = 
{
  CFG_STR_LIST("base_types", "{ }", CFGF_NONE),
  CFG_STR_LIST("implementations", "{ }", CFGF_NONE),
  CFG_INT("nr_additional_template_parameters", 0, CFGF_NONE),
  CFG_STR_LIST("template_parameters", "{ }", CFGF_NONE),
  CFG_END()
};    

//! The primitive configuration defines a section per register size (see @ps_section above for the defintion of its sections)            
static cfg_opt_t ps_opts[] =
{
  CFG_SEC("register_size", ps_section, CFGF_TITLE | CFGF_MULTI),
  CFG_END()
};      


/*! This function creates the implementation files for one class and all defined ISAs. 
* @param primitives_config A configuration file following the @primitives_opt scheme, i.e. this includes a list of the primitives of the primitive class.
* @param isa_names A vector of ISA names. Note that these names must match with the name of the primitive config files, i.e. with <isa> in configs/<primitive_class>/<primitive_name>/<isa>.conf.
* @param class_names A list of all class names (should have been determined by reading the file configs/config.conf)
* @param class_idx The index in @class_names for which the implementation file should be created
*/

int createImplementation( cfg_t *primitives_config, std::vector<char*> isa_names, std::vector<char*> class_names, int class_idx){

  //Loop over all ISAs
  for (int k=0; k<isa_names.size(); k++){     
          
          
          //Create the output file    
          std::cerr << "Create new header file.\n";
          switchOutputFile(std::string("generated/")+std::string(isa_names[k]) + "/primitives/" + std::string(class_names[class_idx])+"_"+isa_names[k]+".h");
          
          std::cerr << "Start Creation of Processing Style Specialization.\n";
          
          insert_head_of_implementation_file(class_names[class_idx], isa_names[k]);//Insert defines and includes
          open_namespace("vectorlib"); //Open the namespace vectorlib
          std::cerr << "Finished writing head of Specilization file.\n";
               
          //Loop over all primitives of the class     
          for (int n=0; n<cfg_size(primitives_config, "primitives"); n++)
          {
            //This is some long and ugly code to retrieve the name of the config file for the current primitive interface configuration
            std::string pname_s = std::string("configs/") + std::string(class_names[class_idx]) + std::string("/") + std::string(cfg_getnstr(primitives_config, "primitives", n)) + std::string("/interface.conf");
            char* pname = new char[pname_s.length()+1];
            strcpy(pname, pname_s.c_str());
                        
            cfg_t *interface_cfg;
      	
      	    interface_cfg = cfg_init(interface_opts, CFGF_NONE);
            //Parse the interface config
      	    if(cfg_parse(interface_cfg, pname) == CFG_PARSE_ERROR)
      	        return 1;
      
            std::vector<std::string> function_args;
            
            //Create a list of the arguments from the interface configuration file
            for (int i=0; i<cfg_size(interface_cfg, "arguments");i++) function_args.push_back(cfg_getnstr(interface_cfg, "arguments", i));
            
            //Another long and ugly code to retrieve the name of the primitive specialization configuration file
            std::string name = std::string("configs/") + class_names[class_idx] + std::string("/") + std::string(cfg_getnstr(primitives_config, "primitives", n)) + std::string("/") + (isa_names[k]) + std::string(".conf");
            char* fname_ps = new char[name.length()+1];
            strcpy(fname_ps, name.c_str());
            
            //We will replace ProcessingStyle from the interface with the actual processing style in teh specialization. To do this, we use a very simple regular expression.
      	    std::regex e ("ProcessingStyle");
            
            cfg_t *ps_cfg, *isa_cfg;
      	    
            //Initialize the configuration, which will hold the information from the specialization configuration (from configs/<primitive_class>/<primitive_name>/<isa>.conf) 
            isa_cfg = cfg_init(ps_opts, CFGF_NONE);
            //Parse the specialization config
            std::cerr << "Try to open config file " << fname_ps << "\n";
      	    if(cfg_parse(isa_cfg, fname_ps) == CFG_PARSE_ERROR){
      	        std::cerr << "Skip non-existing config file" << fname_ps << "\n";
                break;
                }
                
            else
              std::cerr << "Success!\n";
            
            std::cerr << "Config Size: " << cfg_size(isa_cfg, "register_size") << "\n";
             
            //Loop over all register sizes (loop over all sections in configs/<primitive_class>/<primitive_name>/<isa>.conf)
            for(int l = 0; l < cfg_size(isa_cfg, "register_size"); l++)
    	      {
               
               //Get the size of the register
               ps_cfg = cfg_getnsec(isa_cfg, "register_size", l);
      	       std::cerr << "vector size number " << l << ": " << cfg_title(ps_cfg) <<"\n";
               
              //loop over all base types and generate an according function
              for (int i=0;i < cfg_size(ps_cfg, "base_types"); i++){
              
                //Create the name of the processing style
                std::string ps(std::string(isa_names[k])+"<v"+std::string(cfg_title(ps_cfg))+"<"+std::string(cfg_getnstr(ps_cfg, "base_types", i)) + ">>");
              
                //Replace "ProcessingStyle" from the interface with the actual processing style
                std::vector<std::string> args_spec= function_args;
                for (int j=0; j<cfg_size(interface_cfg, "arguments");j++){
                  args_spec[j] = std::regex_replace(args_spec[j],e,ps);
                }
              
                //This empty template is required here to create valid syntax
                open_template();
                close_template();
                
                
                std::vector<std::string> template_spec;
                //Collect the values of the template parameters for the specialization, the firts one is always the processing style
                template_spec.push_back(ps);
                for (int j=0; j<cfg_getint(ps_cfg, "nr_additional_template_parameters");j++){
                  template_spec.push_back(cfg_getnstr(ps_cfg, "template_parameters", i*cfg_getint(ps_cfg, "nr_additional_template_parameters")+j));
                }
                //Open the specialized struct
                open_struct_w_template(cfg_getstr(interface_cfg, "primitive_name"),  cfg_getint(ps_cfg, "nr_additional_template_parameters") + 1, template_spec, "_t");
                printf("\n");
                force_inlineTXT();  //Insert the force inline macro
                static_TXT();    //Insert the static keyword
                
                //Create the apply function (We have to close this later)
                open_function("apply", cfg_size(interface_cfg, "arguments"), args_spec, const_cast<char*> ((std::regex_replace(cfg_getstr(interface_cfg, "return_type"),e,ps)).c_str()));
                //Insert the implementation of this primitive for this processing style
                insert_characters_w_lb(cfg_getnstr(ps_cfg, "implementations", i));
                
                insert_close_bracket();  //close function
                insert_close_bracket();printf(";");  //close struct
                
                printf("\n");
              }
            }
          }
          insert_close_bracket();  //close namespace
          insert_end_of_file();
          }
          std::cerr << "Finish Creation of Processing Style Specialization.\n";
          return 0;
          
}

/*! This function creates the interface file for a defined class
* @param class_name The name of the class
* @param primitives A list of the primitive names of this class
*/

int createInterface(char* class_name, std::vector<std::string> primitives, std::vector<std::string> header){

    std::cerr << "Create new header file.\n";
    switchOutputFile(std::string("generated/primitives/")+std::string(class_name)+".h");
    std::cerr << "Start the Interface Creation\n";
    //Start creating interface file    
    insert_head_of_file(class_name); //Insert defines and includes
    
    //Insert additional headers
    for (int i=0;i<header.size();i++){
       insert_include(header[i]);
    }
    
    open_namespace("vectorlib"); //Open the namespace vectorlib 
    
    //Loop over all primitives of the primitive class  
     for (int i=0; i<primitives.size(); i++){
        
        //Determine the name of the interface config file (configs/<primitive_class>/<primitive_name>/interface.conf)   
        std::string primitive_name=primitives[i];
        std::string name = std::string("configs/") + std::string(class_name) + std::string("/") +  primitive_name + std::string("/interface.conf");
        char* fname = new char[name.length()+1];
        strcpy(fname, name.c_str());
                   
        cfg_t *interface_cfg;
  	
        //Parse the interface config file
  	    interface_cfg = cfg_init(interface_opts, CFGF_NONE);
  	    if(cfg_parse(interface_cfg, fname) == CFG_PARSE_ERROR)
  	        return 1;
  
        //Create a vector with all arguments from the config file
        std::vector<std::string> function_args;
        for (int i=0; i<cfg_size(interface_cfg, "arguments");i++) function_args.push_back(cfg_getnstr(interface_cfg, "arguments", i));
        
        //Write all template parameters
        open_template();
        if (cfg_size(interface_cfg, "templates")>0){
          insert_characters_wo_lb("class ProcessingStyle,\n"); //The first template parameter is always the processing style
        }
        else{
          insert_characters_wo_lb("class ProcessingStyle\n"); //The first template parameter is always the processing style
        }
        
        for (int i=0; i<cfg_size(interface_cfg, "templates"); i++){ //Get all remaining template parameters from the config file, if there are any 
          if (i<cfg_size(interface_cfg, "templates")-1) {
            insert_characters_wo_lb(cfg_getnstr(interface_cfg, "templates", i));
            printf(",\n");
            }
          else insert_characters_w_lb(cfg_getnstr(interface_cfg, "templates", i));
        }
        close_template();
        
        open_struct(cfg_getstr(interface_cfg, "primitive_name"),"_t"); //Create a struct with the name of the primitive (We will have to clos ethis later) 
        force_inlineTXT(); //Insert the force inline macro
        static_TXT(); //Intert the static keyword
       
        //Create the apply function
        insert_function_interface("apply", cfg_size(interface_cfg, "arguments"), function_args, cfg_getstr(interface_cfg, "return_type"));
        
        insert_close_bracket();printf(";");  //close struct
        printf("\n");
        
        //Write the convenience function
        
        std::vector<std::string> args_names = function_args;
        //Loop over all arguments and separate their name from their type and standard values
        for (int i=0;i<cfg_size(interface_cfg, "arguments");i++){
          size_t eq=function_args[i].find_last_of("=");
          args_names[i]=remove_trail_space(function_args[i].substr(0,eq));
        
          size_t last1=args_names[i].find_last_of(" ");
          size_t last2=args_names[i].find_last_of("*");
          size_t last3=args_names[i].find_last_of("&");
          size_t last=0;
          if (last1 == std::string::npos) last1=0;
          if (last2 == std::string::npos) last2=0;
          if (last3 == std::string::npos) last3=0;
          if (last1>last2) last=last1; else last=last2;
          if (last3>last)  last=last3;
          args_names[i]=args_names[i].substr(last+1);
        }
        
        
        
        std::vector<std::string> template_names;
        //Loop over all template parameters and separate their name from their type and standard values  
        for (int i=0;i<cfg_size(interface_cfg, "templates");i++){
          template_names.push_back(cfg_getnstr(interface_cfg, "templates", i));
          
          size_t eq=template_names[i].find_last_of("=");
          template_names[i]=remove_trail_space(template_names[i].substr(0,eq));
          
          size_t last1=template_names[i].find_last_of(" ");
          size_t last2=template_names[i].find_last_of("*");
          size_t last3=template_names[i].find_last_of("&");
          size_t last=0;
          
          if (last1 == std::string::npos) last1=0;
          if (last2 == std::string::npos) last2=0;
          if (last3 == std::string::npos) last3=0;
          if (last1>last2) last=last1; else last=last2;
          if (last3>last)  last=last3;
          template_names[i]=template_names[i].substr(last+1);

        }
        
        //Write the template, force inline macro and static keayword (just like above), but do not open a struct
        open_template();
        insert_characters_wo_lb("class ProcessingStyle,\n"); //The first template parameter is always the processing style
        for (int i=0; i<cfg_size(interface_cfg, "templates"); i++){  
          if (i<cfg_size(interface_cfg, "templates")-1) {
            insert_characters_wo_lb(cfg_getnstr(interface_cfg, "templates", i));
            printf(",\n");
            }
          else insert_characters_w_lb(cfg_getnstr(interface_cfg, "templates", i));
        }
        close_template();
        
        force_inlineTXT();
        static_TXT();
        
        //Open the convenience function (we have to close this function later)
        open_function(cfg_getstr(interface_cfg, "primitive_name"), cfg_size(interface_cfg, "arguments"), function_args, cfg_getstr(interface_cfg, "return_type") );
        insert_characters_wo_lb("return ");
        insert_characters_wo_lb(cfg_getstr(interface_cfg, "primitive_name"));
        printf("_t");
        
        //The template specialization
        open_template_spec();
        insert_characters_wo_lb("ProcessingStyle,\n"); //The first template parameter is always the processing style
        for (int i=0; i<cfg_size(interface_cfg, "templates"); i++){  
          if (i<cfg_size(interface_cfg, "templates")-1) {
            insert_characters_wo_lb(const_cast<char*> (template_names[i].c_str()));
            printf(",\n");
            }
          else insert_characters_w_lb(const_cast<char*> (template_names[i].c_str()));
        }
        close_template();
        insert_characters_wo_lb("::apply(");
        
        //Add the arguments
        for (int i=0;i<cfg_size(interface_cfg, "arguments");i++){
          insert_characters_wo_lb(const_cast<char*> (args_names[i].c_str()));
          if (i<cfg_size(interface_cfg, "arguments")-1)
            printf(", "); 
        }
        
        printf(");\n");
        insert_close_bracket();  //close function
      }
      insert_close_bracket();  //close namespace
      insert_end_of_file();
  std::cerr << "Finish the Interface Creation\n";    
  return 0;
}

#endif