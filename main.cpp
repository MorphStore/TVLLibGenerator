
#include "sourceGen.h"
#include <regex>
	
/* Next TODOs: 
* - header defines -> DONE
* - Ebenen zählen (+ Funktion fürs Schließen) -> DONE 
* - Template aus config -> DONE 
* - Einzelne Processing Styles -> DONE 
* - Mapping für PS -> DONE
* - Lesen von Properties auslagern
* - Dateinamen bestimmen 
* - Automatisch über alle files iterieren -> DONE
* - Convenience function -> DONE
* - Header in Implementierung -> DONE
* - Template- und Funktionsparameter statt std-Wert in convenience function übergeben (vor existierendem Split: nach "=" suchen und splitten)
* - Include in Implementierung auf ISA anpassen -> DONE
* - Bitbreite als Section in Config
*/

	int main(void)
	{
	    printf("Read files names\n");
      
      
	    cfg_opt_t opts[] =
	    {
	        CFG_STR("interfaces", " ", CFGF_NONE),
	        CFG_END()
	    };

      cfg_t *cfg;
	
	    cfg = cfg_init(opts, CFGF_NONE);
	    if(cfg_parse(cfg, "folders.conf") == CFG_PARSE_ERROR)
	        return 1;

      printf("Interface Folder: %s\n", cfg_getstr(cfg, "interfaces"));
      
     
     //Read interface Config file
     printf("Read interface config\n");
           
      char fname[] = "configs/calc/add/interface.conf";
	    cfg_opt_t interface_opts[] =
	    {
	        CFG_STR("primitive_class", " ", CFGF_NONE),
          CFG_STR("primitive_name", " ", CFGF_NONE),
          CFG_STR_LIST("templates", "{ }", CFGF_NONE),
          CFG_STR("return_type", "void", CFGF_NONE),
	        CFG_STR_LIST("arguments", "{ }", CFGF_NONE), 
          CFG_END()
	    };

      cfg_t *interface_cfg;
	
	    interface_cfg = cfg_init(interface_opts, CFGF_NONE);
	    if(cfg_parse(interface_cfg, fname) == CFG_PARSE_ERROR)
	        return 1;

      std::vector<std::string> function_args;
      
      for (int i=0; i<cfg_size(interface_cfg, "arguments");i++) function_args.push_back(cfg_getnstr(interface_cfg, "arguments", i));
      
      
      //Start creating interface file
      switchOutputFile(std::string(cfg_getstr(interface_cfg, "primitive_class"))+".h");
      
      insert_head_of_file(cfg_getstr(interface_cfg, "primitive_class"));
      
      open_namespace("vectorlib");
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
      
      insert_close_bracket();  //close namespace
      insert_end_of_file();
      
      
      //Read PS config
      std::cerr << "Read PS config\n";
      
      std::vector<char*> config_files;
      std::vector<char*> isa_names;
      isa_names.push_back("sse");
      std::vector<std::string> f;
      
      for (int i=0; i < isa_names.size(); i++){
        f.push_back("configs/calc/add/" + std::string(isa_names[i]) + ".conf");
        config_files.push_back(&(f[i][0]));
        std::cerr << "file " << i << ": " << f[i] << "\n";
      }
      
      for (int k=0; k<config_files.size(); k++){
      
        char* fname_ps = config_files[k];
      
  	    std::regex e ("ProcessingStyle");
        
        cfg_opt_t ps_section[] = {
           // CFG_STR("isa", " ", CFGF_NONE),
            CFG_STR_LIST("base_types", "{ }", CFGF_NONE),
            CFG_STR_LIST("implementations", "{ }", CFGF_NONE),
            CFG_INT("nr_additional_template_parameters", 0, CFGF_NONE),
            CFG_STR_LIST("template_parameters", "{ }", CFGF_NONE),
            CFG_END()
        };    
        
        cfg_opt_t ps_opts[] =
  	    {
  	      
            CFG_SEC("register_size", ps_section, CFGF_TITLE | CFGF_MULTI),
            //CFG_STR("register_size", " ", CFGF_NONE),
            CFG_END()
  	    };
        
           
        
        cfg_t *ps_cfg, *isa_cfg;
  	
  	    isa_cfg = cfg_init(ps_opts, CFGF_NONE);
        
        std::cerr << "Try to open config file " << fname_ps << "\n";
  	    if(cfg_parse(isa_cfg, fname_ps) == CFG_PARSE_ERROR)
  	        return 1;
        else
          std::cerr << "Success!\n";
            
        //Start creating implementation file
        switchOutputFile(std::string(cfg_getstr(interface_cfg, "primitive_class"))+"_"+isa_names[k]+".h");
        insert_head_of_implementation_file(cfg_getstr(interface_cfg, "primitive_class"), isa_names[k]);
        
        std::cerr << "Config Size: " << cfg_size(isa_cfg, "register_size") << "\n";
         
        open_namespace("vectorlib");
        for(int l = 0; l < cfg_size(isa_cfg, "register_size"); l++)
	      {
           std::cerr << "l: " << l << "\n";
           ps_cfg = cfg_getnsec(isa_cfg, "register_size", l);
  	      
        //loop over all base types and generate an according function
          for (int i=0;i < cfg_size(ps_cfg, "base_types"); i++){
          
            std::string ps(std::string(isa_names[k])+"<v"+std::string(cfg_title(ps_cfg))+"<"+std::string(cfg_getnstr(ps_cfg, "base_types", i)) + ">>");
          
            std::vector<std::string> args_spec= function_args;
            for (int j=0; j<cfg_size(interface_cfg, "arguments");j++){
              args_spec[j] = std::regex_replace(args_spec[j],e,ps);
            }
          
            open_template();
            close_template();
            
            
            std::vector<std::string> template_spec;
            template_spec.push_back(ps);
            for (int j=0; j<cfg_getint(ps_cfg, "nr_additional_template_parameters");j++){
              template_spec.push_back(cfg_getnstr(ps_cfg, "template_parameters", i*cfg_getint(ps_cfg, "nr_additional_template_parameters")+j));
            }
            open_struct_w_template(cfg_getstr(interface_cfg, "primitive_name"),  cfg_getint(ps_cfg, "nr_additional_template_parameters") + 1, template_spec);
            printf("\n");
            force_inlineTXT();
            static_TXT();
            
            open_function("apply", cfg_size(interface_cfg, "arguments"), args_spec, const_cast<char*> ((std::regex_replace(cfg_getstr(interface_cfg, "return_type"),e,ps)).c_str()));
            insert_characters_w_lb(cfg_getnstr(ps_cfg, "implementations", i));
            insert_close_bracket();  //close function
            insert_close_bracket();  //close struct
            
            printf("\n");
          }
        }
      }
      insert_close_bracket();  //close namespace
      insert_end_of_file();
      
	    return 0;
	}
