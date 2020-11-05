#ifndef SOURCEGENFUNCTIONS_H
#define SOURCEGENFUNCTIONS_H

#include "sourceGen.h"

static cfg_opt_t interface_opts[] =
{
  CFG_STR("primitive_class", " ", CFGF_NONE),
  CFG_STR("primitive_name", " ", CFGF_NONE),
  CFG_STR_LIST("templates", "{ }", CFGF_NONE),
  CFG_STR("return_type", "void", CFGF_NONE),
  CFG_STR_LIST("arguments", "{ }", CFGF_NONE), 
  CFG_END()
};
void createInterface(char* class_name, std::vector<std::string> primitives){

      

     //Read interface Config file
     printf("Read interface config\n");
             //Start creating interface file
    switchOutputFile(std::string("generated/primitives/")+std::string(class_name)+".h");
        
    insert_head_of_file(class_name);
    open_namespace("vectorlib");    
     for (int i=0; i<primitives.size(); i++){
           
        std::string primitive_name=primitives[i];
        std::string name = std::string("configs/") + std::string(class_name) + std::string("/") +  primitive_name + std::string("/interface.conf");
        char* fname = new char[name.length()+1];
        strcpy(fname, name.c_str());
                   
        //char fname[] = "configs/calc/add/interface.conf";
        
        cfg_t *interface_cfg;
  	
  	    interface_cfg = cfg_init(interface_opts, CFGF_NONE);
  	    if(cfg_parse(interface_cfg, fname) == CFG_PARSE_ERROR)
  	        return;
  
        std::vector<std::string> function_args;
        
        for (int i=0; i<cfg_size(interface_cfg, "arguments");i++) function_args.push_back(cfg_getnstr(interface_cfg, "arguments", i));
        
        
        
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
        
        open_struct(cfg_getstr(interface_cfg, "primitive_name"));
        force_inlineTXT();
        static_TXT();
       // insert_characters_w_lb(cfg_getstr(interface_cfg, "return_type"));
        insert_function_interface("apply", cfg_size(interface_cfg, "arguments"), function_args, cfg_getstr(interface_cfg, "return_type"));
        
        insert_close_bracket();  //close struct
        printf("\n");
        
        //Convenience function
        std::vector<std::string> args_names = function_args;
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
  //        std::cerr << last << "\n";
        }
        
        std::vector<std::string> template_names;
        
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
  //        std::cerr << last << "\n";
        }
        
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
        
        open_function(cfg_getstr(interface_cfg, "primitive_name"), cfg_size(interface_cfg, "arguments"), function_args, cfg_getstr(interface_cfg, "return_type") );
        insert_characters_wo_lb("return ");
        insert_characters_wo_lb(cfg_getstr(interface_cfg, "primitive_name"));
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
      
  return;
}

#endif