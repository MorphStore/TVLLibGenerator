
#include "sourceGenFunctions.h"

	
/* Next TODOs: 
* - header defines -> DONE
* - Ebenen zählen (+ Funktion fürs Schließen) -> DONE 
* - Template aus config -> DONE 
* - Einzelne Processing Styles -> DONE 
* - Mapping für PS -> DONE
* - Dateinamen bestimmen  -> DONE
* - Automatisch über alle files iterieren -> DONE
* - Convenience function -> DONE
* - Header in Implementierung -> DONE
* - Template- und Funktionsparameter statt std-Wert in convenience function übergeben (vor existierendem Split: nach "=" suchen und splitten) -> DONE
* - Include in Implementierung auf ISA anpassen -> DONE
* - Bitbreite als Section in Config -> DONE
* - Erstellen von implementation files auslagern
* - ISA aus config file lesen
*/

	int main(void)
	{
  
      mkdir("generated", 0777);
      mkdir("generated/primitives", 0777);
      
	    printf("Read files names\n");
      
      cfg_opt_t primitives_opts[] =
  	    {
  	      
            CFG_STR_LIST("primitives", "{ }", CFGF_NONE),
            CFG_END()
  	    };

      cfg_opt_t class_opts[] =
  	    {
  	      
            CFG_STR_LIST("classes", "{ }", CFGF_NONE),
            CFG_END()
  	    };
        

      cfg_t *primitives_config = cfg_init(primitives_opts, CFGF_NONE);
      
     
      std::vector<char*> config_files;
      std::vector<char*> isa_names;
      isa_names.push_back("sse");
      isa_names.push_back("avx2");
      isa_names.push_back("avx512");
      isa_names.push_back("tsubasa");
      std::vector<std::string> f;
      std::vector<char*> class_names;
      cfg_t *class_config;
      
       
        
      class_config = cfg_init(class_opts, CFGF_NONE);  
      std::cerr << "Try to read class names from configs/classes.conf\n";
  	    if(cfg_parse(class_config, "configs/classes.conf") == CFG_PARSE_ERROR){
  	        std::cerr << "could not open config file\n";
            return 1;
            }
        else
          std::cerr << "Success! Found " << cfg_size(class_config, "classes") << " class names\n";
            
      for (int i=0; i < isa_names.size(); i++){

            config_files.push_back(isa_names[i]);
            std::cerr << "isa " << i << ": " << config_files[i] << "\n";
            mkdir((std::string("generated/")+config_files[i]).c_str(), 0777);
      }
            
      for (int i=0; i<cfg_size(class_config, "classes");i++){
        class_names.push_back(cfg_getnstr(class_config, "classes", i));
        std::cerr << "class: " << class_names[i]<< std::endl;  
        for (int j=0; j< isa_names.size();j++)  mkdir((std::string("generated/")+std::string(config_files[j]) + "/primitives").c_str(), 0777);
      } 
      
      
      
      for (int m =0; m < class_names.size();m++){
        
       
        primitives_config = cfg_init(primitives_opts, CFGF_NONE);
        
        std::cerr << "Try to read primitive names from configs/" <<  class_names[m] << "/primitives.conf\n";
        
  	    if(cfg_parse(primitives_config, (std::string(std::string("configs/") + class_names[m] + std::string("/") + std::string("primitives.conf"))).c_str()) == CFG_PARSE_ERROR){
  	        std::cerr << "could not open config file\n";
            return 1;
            }
        else
          std::cerr << "Success! Found " << cfg_size(primitives_config, "primitives") << " primitive names\n";
        
        std::vector<std::string> primitives;
        for (int k=0; k<cfg_size(primitives_config, "primitives"); k++){
          primitives.push_back(std::string(cfg_getnstr(primitives_config, "primitives", k)));
        }
        //!!!!!!!!!!!!!!!!!!!!
        createInterface(class_names[m], primitives);
        //!!!!!!!!!!!!!!!!!!!!
                
       
        
        for (int k=0; k<config_files.size(); k++){     
          //Start creating implementation file
              
          switchOutputFile(std::string("generated/")+std::string(isa_names[k]) + "/primitives/" + std::string(class_names[m])+"_"+isa_names[k]+".h");
          insert_head_of_implementation_file(class_names[m], isa_names[k]);
          
          open_namespace("vectorlib");
               
          for (int n=0; n<cfg_size(primitives_config, "primitives"); n++)
          {
            ////////
            std::string pname_s = std::string("configs/") + std::string(class_names[m]) + std::string("/") + std::string(cfg_getnstr(primitives_config, "primitives", n)) + std::string("/interface.conf");
            char* pname = new char[pname_s.length()+1];
            strcpy(pname, pname_s.c_str());
                        
            cfg_t *interface_cfg;
      	
      	    interface_cfg = cfg_init(interface_opts, CFGF_NONE);
      	    if(cfg_parse(interface_cfg, pname) == CFG_PARSE_ERROR)
      	        return 1;
      
            std::vector<std::string> function_args;
            
            for (int i=0; i<cfg_size(interface_cfg, "arguments");i++) function_args.push_back(cfg_getnstr(interface_cfg, "arguments", i));
            /////////
            
            std::string name = std::string("configs/") + class_names[m] + std::string("/") + std::string(cfg_getnstr(primitives_config, "primitives", n)) + std::string("/") + (config_files[k]) + std::string(".conf");
            char* fname_ps = new char[name.length()+1];
            strcpy(fname_ps, name.c_str());
            
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
      	    if(cfg_parse(isa_cfg, fname_ps) == CFG_PARSE_ERROR){
      	        std::cerr << "Skip non-existing config file" << fname_ps << "\n";
                break;
                }
                //return 1;
            else
              std::cerr << "Success!\n";
                
            
            
            std::cerr << "Config Size: " << cfg_size(isa_cfg, "register_size") << "\n";
             
            
            for(int l = 0; l < cfg_size(isa_cfg, "register_size"); l++)
    	      {
               
               ps_cfg = cfg_getnsec(isa_cfg, "register_size", l);
      	       std::cerr << "vector size number " << l << ": " << cfg_title(ps_cfg) <<"\n";
               
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
          }
      }
	    return 0;
	}
